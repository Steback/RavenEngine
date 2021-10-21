#include "Shader.hpp"

#include "spdlog/spdlog.h"

#include "engine/utils/Utils.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/config/CliOptions.hpp"


namespace re {

    Shader::Shader(const VkDevice& device, const std::string& fileName, const VkShaderStageFlagBits& stage)
            : device(device), stage(stage) {
        File shader = files::getFile(fileName.c_str());

        // TODO: Improve when how to work with shader and compile it
        if (cli::getFlag("compile-shaders")) {
            Shader::compileShader(shader, stage);
        } else {
            shader.setPath(shader.getPath() + ".spv");
        }

        createShaderModule(shader.readBytes());
    }

    Shader::~Shader() {
        vkDestroyShaderModule(device, module, nullptr);
    };

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

    const VkShaderModule &Shader::getModule() {
        return module;
    }

    VkShaderStageFlagBits Shader::getStage() {
        return stage;
    }

    void Shader::createShaderModule(const std::vector<uint32_t> &code) {
        VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.codeSize = code.size();
        createInfo.pCode = code.data();

        checkResult(vkCreateShaderModule(device, &createInfo, nullptr, &module),
                    "Failed to create shader module");
    }

    void Shader::compileShader(File& file, const VkShaderStageFlagBits& stage) {
        shaderc_shader_kind kind = getKind(stage);

        // Preprocessing
        std::vector<char> code = file.read();
        auto preprocessed = preprocessShader(file.getName(), kind, {code.begin(), code.begin() + code.size()});

        // Compiling
        auto binary = compile(file.getName(), kind, preprocessed, true);

        file.setPath(file.getPath() + ".spv");
        file.write(binary);
    }

    std::string Shader::preprocessShader(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source.data(), source.size(), kind, sourceName.c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            spdlog::error(result.GetErrorMessage());
            return "";
        }

        return {result.cbegin(), result.cend()};
    }

    std::vector<uint32_t> Shader::compile(const std::string &sourceName, shaderc_shader_kind kind, const std::string &source,
                                          bool optimize) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source.c_str(), source.size(), kind,sourceName.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            spdlog::error("Failed to compile\n{}", module.GetErrorMessage());
            return {};
        }

        return {module.cbegin(), module.cend()};
    }

    shaderc_shader_kind Shader::getKind(const VkShaderStageFlagBits& stage) {
        if (stage == VK_SHADER_STAGE_VERTEX_BIT) {
            return shaderc_glsl_vertex_shader;
        } else if (stage == VK_SHADER_STAGE_FRAGMENT_BIT) {
            return shaderc_glsl_fragment_shader;
        }

        throwEx("Failed to find correct shader kind");

        return shaderc_glsl_vertex_shader;
    }

} // namespace re