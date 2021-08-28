#include "Shader.hpp"

#include "spdlog/spdlog.h"

#include "utils/Macros.hpp"
#include "files/File.hpp"
#include "files/FilesManager.hpp"


namespace re {

    Shader::Shader(const VkDevice& device, const std::string& fileName, const VkShaderStageFlagBits& stage)
            : device(device), stage(stage) {
        createShaderModule(FilesManager::getFile(fileName.c_str()).readBytes());
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

        RE_VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, &module),
                           "Failed to create shader module");
    }

    void Shader::compileShaders() {
        for (auto& filePath : std::filesystem::directory_iterator(FilesManager::getPath("shaders"))) {
            if (filePath.path().extension() != ".spv") {
                compileShader(filePath.path());
            }
        }
    }

    void Shader::compileShader(const std::string &fileName) {
        File file(fileName);

        shaderc_shader_kind kind = getKind(file.getExtension());

        // Preprocessing
        auto preprocessed = preprocessShader(file.getName(), kind, file.read().data());

        // Compiling
        auto binary = compile(file.getName(), kind, preprocessed, true);

        file.setPath(file.getPath() + ".spv");
        file.write(binary);
    }

    std::string Shader::preprocessShader(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source, kind, sourceName.c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            spdlog::error(result.GetErrorMessage());
            return "";
        }

        return {result.cbegin(), result.cend()};
    }

    std::string Shader::compileToAssembly(const std::string &sourceName, shaderc_shader_kind kind, const std::string &source,
                                          bool optimize) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(source, kind, sourceName.c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
            spdlog::error("Failed to compile to Assembly\n{}", result.GetErrorMessage());
            return "";
        }

        return {result.cbegin(), result.cend()};
    }

    std::vector<uint32_t> Shader::compile(const std::string &sourceName, shaderc_shader_kind kind, const std::string &source,
                                          bool optimize) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        if (optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, kind, sourceName.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            spdlog::error("Failed to compile\n{}", module.GetErrorMessage());
            return {};
        }

        return {module.cbegin(), module.cend()};
    }

    shaderc_shader_kind Shader::getKind(const std::string& extension) {
        if (extension == ".vert") {
            return shaderc_glsl_vertex_shader;
        } else if (extension == ".frag") {
            return shaderc_glsl_fragment_shader;
        }

        RE_THROW_EX("Failed to find correct shader kind");
    }

} // namespace re