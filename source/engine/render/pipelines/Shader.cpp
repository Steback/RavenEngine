#include "Shader.hpp"

#include "shaderc/shaderc.hpp"

#include "engine/core/Utils.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/config/CliConfig.hpp"


namespace re {

    shaderc_shader_kind getKind(const VkShaderStageFlagBits& stage) {
        switch (stage) {
            case VK_SHADER_STAGE_VERTEX_BIT:
                return shaderc_glsl_vertex_shader;
            case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                return shaderc_glsl_tess_control_shader;
            case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                return shaderc_glsl_tess_evaluation_shader;
            case VK_SHADER_STAGE_GEOMETRY_BIT:
                return shaderc_glsl_geometry_shader;
            case VK_SHADER_STAGE_FRAGMENT_BIT:
                return shaderc_glsl_fragment_shader;
            case VK_SHADER_STAGE_COMPUTE_BIT:
                return shaderc_glsl_compute_shader;
        }

        return shaderc_glsl_anyhit_shader;
    }

    Shader::Shader(VkDevice device, const std::string& name) : device(device) {
        auto file = files::getPath("shaders") / name;
        stage = Shader::getStage(file.extension());

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

    void Shader::compileShader(const std::filesystem::path &file) const {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        shaderc_shader_kind kind = getKind(stage);

        const std::vector<char> source = files::File(file).read();
        shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source.data(), source.size(), kind, file.filename().c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
            throwEx(result.GetErrorMessage());

        auto preprocessed = std::string(result.cbegin(), result.cend());
        shaderc::SpvCompilationResult shModule = compiler.CompileGlslToSpv(preprocessed.c_str(),  preprocessed.size(), kind, file.filename().c_str(), options);

        if (shModule.GetCompilationStatus() != shaderc_compilation_status_success)
            throwEx(shModule.GetErrorMessage());

        auto binary = std::vector<uint32_t>(shModule.begin(), shModule.end());

        File spvFile(file.string() + ".spv");
        spvFile.write(binary);
    }

    void Shader::createShaderModule(const std::filesystem::path& file) {
        std::vector<uint32_t> code = File(file).readBytes();

        VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.codeSize = code.size();
        createInfo.pCode = code.data();

        checkResult(vkCreateShaderModule(device, &createInfo, nullptr, &module),
                    "Failed to create shader module");
    }

} // namespace re