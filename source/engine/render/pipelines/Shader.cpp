#include "Shader.hpp"

#include "spirv_glsl.hpp"

#include "engine/core/Utils.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/config/CliConfig.hpp"
#include "engine/logs/Logs.hpp"


namespace re {

    Shader::Shader(VkDevice device, const std::string& name) : device(device) {
        auto file = files::getPath("shaders") / name;
        stage = Shader::getStage(file.extension().string());

        if (cli::getFlag("--compile-shaders")) compileShader(file);

        createShaderModule(file.string() + ".spv");
    }

    Shader::~Shader() {
        vkDestroyShaderModule(device, module, nullptr);
    }

    VkPipelineShaderStageCreateInfo Shader::getPipelineStageCreateInfo(uint32_t flags) const {
        return {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            flags,
            stage,
            module,
            "main",
            nullptr
        };
    }

    VkShaderStageFlagBits Shader::getStage(const std::string &ext) {
        if (ext == ".comp")
            return VK_SHADER_STAGE_COMPUTE_BIT;
        if (ext == ".vert")
            return VK_SHADER_STAGE_VERTEX_BIT;
        if (ext == ".tesc")
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        if (ext == ".tese")
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        if (ext == ".geom")
            return VK_SHADER_STAGE_GEOMETRY_BIT;
        if (ext == ".frag")
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        return VK_SHADER_STAGE_ALL;
    }

    void Shader::compileShader(const std::filesystem::path &file) {
#ifdef _WIN64
        std::filesystem::path glslValidator = files::getPath("tools") / "glslangValidator.exe";
#else
        std::filesystem::path glslValidator = files::getPath("bin") / "glslangValidator";
#endif

        std::system((glslValidator.string() + " -V " + file.string() + " -o " + (file.string() + ".spv --auto-map-bindings")).c_str());
    }

    void Shader::createShaderModule(const std::filesystem::path& file) {
        std::vector<uint32_t> code = File(file).readBytes();

        spirv_cross::CompilerGLSL glsl(std::vector<uint32_t>(code.data(), code.data() + code.size() / sizeof(uint32_t)));
        spirv_cross::ShaderResources shaderResources = glsl.get_shader_resources();

        for (auto& resource : shaderResources.uniform_buffers) {
            uint32_t set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32_t binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            resources[resource.name] = {set, binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER};
        }

        for (auto& resource : shaderResources.sampled_images) {
            uint32_t set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            uint32_t binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            log::info(fmt::format("Image {} at set = {}, binging = {}", resource.name, set, binding));
            resources[resource.name] = {set, binding, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER};
        }

        VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.codeSize = code.size();
        createInfo.pCode = code.data();

        checkResult(vkCreateShaderModule(device, &createInfo, nullptr, &module),
                    "Failed to create shader module");
    }

} // namespace re