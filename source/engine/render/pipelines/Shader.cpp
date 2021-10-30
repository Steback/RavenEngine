#include "Shader.hpp"

#include "glslang/SPIRV/GlslangToSpv.h"
#include "glslang/Public/ShaderLang.h"

#include "engine/logs/Logs.hpp"
#include "engine/utils/Utils.hpp"
#include "engine/files/FilesManager.hpp"
#include "engine/config/CliConfig.hpp"


namespace re {

    class ShaderIncluder : public glslang::TShader::Includer {
    public:
        IncludeResult* includeLocal(const char* headerName, const char* includeName, size_t inclusionDepth) override {
            auto directory = files::getPath("root") / std::filesystem::path(includeName).parent_path();
            auto fileLoaded = files::getFile(directory / headerName);

            // TODO: Change this when File header will refactored
            if (!std::filesystem::exists(fileLoaded.getPath())) {
                log::error(fmt::format("Shader include could not be loaded: {}", headerName));
                return nullptr;
            }

            auto content = fileLoaded.read();
            return new IncludeResult(headerName, content.data(), content.size(), content.data());
        }

        IncludeResult *includeSystem(const char *headerName, const char *includeName, size_t inclusionDepth) override {
            auto fileLoaded = files::getFile(headerName);

            // TODO: Change this when File header will refactored
            if (!std::filesystem::exists(fileLoaded.getPath())) {
                log::error(fmt::format("Shader include could not be loaded: {}", headerName));
                return nullptr;
            }

            auto content = fileLoaded.read();
            return new IncludeResult(headerName, content.data(), content.size(), content.data());
        }

        void releaseInclude(IncludeResult *result) override {
            if (result) {
                if (result->userData) delete[] static_cast<char *>(result->userData);
                delete result;
            }
        }
    };

    TBuiltInResource getResources() {
        TBuiltInResource resources = {};
        resources.maxLights = 32;
        resources.maxClipPlanes = 6;
        resources.maxTextureUnits = 32;
        resources.maxTextureCoords = 32;
        resources.maxVertexAttribs = 64;
        resources.maxVertexUniformComponents = 4096;
        resources.maxVaryingFloats = 64;
        resources.maxVertexTextureImageUnits = 32;
        resources.maxCombinedTextureImageUnits = 80;
        resources.maxTextureImageUnits = 32;
        resources.maxFragmentUniformComponents = 4096;
        resources.maxDrawBuffers = 32;
        resources.maxVertexUniformVectors = 128;
        resources.maxVaryingVectors = 8;
        resources.maxFragmentUniformVectors = 16;
        resources.maxVertexOutputVectors = 16;
        resources.maxFragmentInputVectors = 15;
        resources.minProgramTexelOffset = -8;
        resources.maxProgramTexelOffset = 7;
        resources.maxClipDistances = 8;
        resources.maxComputeWorkGroupCountX = 65535;
        resources.maxComputeWorkGroupCountY = 65535;
        resources.maxComputeWorkGroupCountZ = 65535;
        resources.maxComputeWorkGroupSizeX = 1024;
        resources.maxComputeWorkGroupSizeY = 1024;
        resources.maxComputeWorkGroupSizeZ = 64;
        resources.maxComputeUniformComponents = 1024;
        resources.maxComputeTextureImageUnits = 16;
        resources.maxComputeImageUniforms = 8;
        resources.maxComputeAtomicCounters = 8;
        resources.maxComputeAtomicCounterBuffers = 1;
        resources.maxVaryingComponents = 60;
        resources.maxVertexOutputComponents = 64;
        resources.maxGeometryInputComponents = 64;
        resources.maxGeometryOutputComponents = 128;
        resources.maxFragmentInputComponents = 128;
        resources.maxImageUnits = 8;
        resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
        resources.maxCombinedShaderOutputResources = 8;
        resources.maxImageSamples = 0;
        resources.maxVertexImageUniforms = 0;
        resources.maxTessControlImageUniforms = 0;
        resources.maxTessEvaluationImageUniforms = 0;
        resources.maxGeometryImageUniforms = 0;
        resources.maxFragmentImageUniforms = 8;
        resources.maxCombinedImageUniforms = 8;
        resources.maxGeometryTextureImageUnits = 16;
        resources.maxGeometryOutputVertices = 256;
        resources.maxGeometryTotalOutputComponents = 1024;
        resources.maxGeometryUniformComponents = 1024;
        resources.maxGeometryVaryingComponents = 64;
        resources.maxTessControlInputComponents = 128;
        resources.maxTessControlOutputComponents = 128;
        resources.maxTessControlTextureImageUnits = 16;
        resources.maxTessControlUniformComponents = 1024;
        resources.maxTessControlTotalOutputComponents = 4096;
        resources.maxTessEvaluationInputComponents = 128;
        resources.maxTessEvaluationOutputComponents = 128;
        resources.maxTessEvaluationTextureImageUnits = 16;
        resources.maxTessEvaluationUniformComponents = 1024;
        resources.maxTessPatchComponents = 120;
        resources.maxPatchVertices = 32;
        resources.maxTessGenLevel = 64;
        resources.maxViewports = 16;
        resources.maxVertexAtomicCounters = 0;
        resources.maxTessControlAtomicCounters = 0;
        resources.maxTessEvaluationAtomicCounters = 0;
        resources.maxGeometryAtomicCounters = 0;
        resources.maxFragmentAtomicCounters = 8;
        resources.maxCombinedAtomicCounters = 8;
        resources.maxAtomicCounterBindings = 1;
        resources.maxVertexAtomicCounterBuffers = 0;
        resources.maxTessControlAtomicCounterBuffers = 0;
        resources.maxTessEvaluationAtomicCounterBuffers = 0;
        resources.maxGeometryAtomicCounterBuffers = 0;
        resources.maxFragmentAtomicCounterBuffers = 1;
        resources.maxCombinedAtomicCounterBuffers = 1;
        resources.maxAtomicCounterBufferSize = 16384;
        resources.maxTransformFeedbackBuffers = 4;
        resources.maxTransformFeedbackInterleavedComponents = 64;
        resources.maxCullDistances = 8;
        resources.maxCombinedClipAndCullDistances = 8;
        resources.maxSamples = 4;
        resources.limits.nonInductiveForLoops = true;
        resources.limits.whileLoops = true;
        resources.limits.doWhileLoops = true;
        resources.limits.generalUniformIndexing = true;
        resources.limits.generalAttributeMatrixVectorIndexing = true;
        resources.limits.generalVaryingIndexing = true;
        resources.limits.generalSamplerIndexing = true;
        resources.limits.generalVariableIndexing = true;
        resources.limits.generalConstantMatrixVectorIndexing = true;
        return resources;
    }

    EShLanguage getEshLanguage(VkShaderStageFlags stage) {
        switch (stage) {
            case VK_SHADER_STAGE_COMPUTE_BIT:
                return EShLangCompute;
            case VK_SHADER_STAGE_VERTEX_BIT:
                return EShLangVertex;
            case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
                return EShLangTessControl;
            case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
                return EShLangTessEvaluation;
            case VK_SHADER_STAGE_GEOMETRY_BIT:
                return EShLangGeometry;
            case VK_SHADER_STAGE_FRAGMENT_BIT:
                return EShLangFragment;
            default:
                return EShLangCount;
        }
    }

    Shader::Shader(const VkDevice& device, const std::string& fileName, const VkShaderStageFlagBits& stage)
            : device(device), stage(stage) {
        File shader = files::getFile(fileName);

        if (cli::getFlag("--compile-shaders")) {
            log::info("Compile Shaders");
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

    void Shader::compileShader(File& file, const VkShaderStageFlagBits& stage) {
        log::info(fmt::format("Compile shader: {}", file.getName()));

        // Starts converting GLSL to SPIR-V.
        auto language = getEshLanguage(stage);
        glslang::TProgram program;
        glslang::TShader shader(language);
        auto resources = getResources();

        // Enable SPIR-V and Vulkan rules when parsing GLSL.
        auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#ifdef RE_DEBUG
        messages = static_cast<EShMessages>(messages | EShMsgDebugInfo);
#endif

        auto shaderName = file.getName();
        auto shaderNameCstr = shaderName.c_str();
        auto code = file.read();
        auto shaderSource = code.data();
        shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, &shaderNameCstr, 1);

        auto defaultVersion = glslang::EShTargetVulkan_1_2;
        shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 450);
        shader.setEnvClient(glslang::EShClientVulkan, defaultVersion);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

        ShaderIncluder includer;
        std::string str;

        if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &str, includer)) {
            log::error(shader.getInfoLog());
            log::error(shader.getInfoDebugLog());
            log::error("SPRIV shader preprocess failed!");
        }

        if (!shader.parse(&resources, defaultVersion, true, messages, includer)) {
            log::error(shader.getInfoLog());
            log::error(shader.getInfoDebugLog());
            log::error("SPRIV shader parse failed!");
        }

        program.addShader(&shader);

        if (!program.link(messages) || !program.mapIO())
            log::error("Error while linking shader program.\n");

        program.buildReflection();

        glslang::SpvOptions spvOptions;
#ifdef RE_DEBUG
        spvOptions.generateDebugInfo = true;
        spvOptions.disableOptimizer = true;
        spvOptions.optimizeSize = false;
#else
        spvOptions.generateDebugInfo = false;
        spvOptions.disableOptimizer = false;
        spvOptions.optimizeSize = true;
#endif
        spv::SpvBuildLogger logger;
        std::vector<uint32_t> spirv;
        GlslangToSpv(*program.getIntermediate(static_cast<EShLanguage>(language)), spirv, &logger, &spvOptions);

        createShaderModule(spirv);
    }

    void Shader::createShaderModule(const std::vector<uint32_t> &code) {
        VkShaderModuleCreateInfo createInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
        createInfo.codeSize = code.size();
        createInfo.pCode = code.data();

        checkResult(vkCreateShaderModule(device, &createInfo, nullptr, &module),
                    "Failed to create shader module");
    }

} // namespace re