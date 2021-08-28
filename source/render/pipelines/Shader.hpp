#ifndef RAVENENGINE_SHADER_HPP
#define RAVENENGINE_SHADER_HPP


#include <string>
#include <vector>

#include "vulkan/vulkan.h"
#include "shaderc/shaderc.hpp"

#include "utils/NonCopyable.hpp"


namespace re {

    /**
     * @brief Shader wrapper class
     */
    class Shader : NonCopyable {
    public:
        Shader() = default;

        /**
         *
         * @param device Vulkan raw device
         * @param fileName Shader file name
         * @param stage Shader use stage
         */
        Shader(const VkDevice& device, const std::string& fileName, const VkShaderStageFlagBits& stage);

        ~Shader() override;

        /**
         *
         * @param flags Shader stages flags. By default is 0
         * @return Vulkan Pipeline shader stage create info of this Shader
         */
        [[nodiscard]] VkPipelineShaderStageCreateInfo getPipelineStageCreateInfo(uint32_t flags = 0) const;

        const VkShaderModule& getModule();

        VkShaderStageFlagBits getStage();

        /**
         * @brief Read all the shaders in "shaders" directory and compile it to SPIR-V
         */
        static void compileShaders();

        /**
         * @brief Compile shader and save it in SPIR-V format in "shaders" directory
         * @param fileName
         */
        static void compileShader(const std::string& fileName);

        /**
         *
         * @param sourceName
         * @param kind
         * @param source
         * @return GLSL shader source text after preprocessing.
         */
        static std::string preprocessShader(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source);

        /**
         * @brief Compiles a shader to SPIR-V assembly
         * @param sourceName
         * @param kind
         * @param source
         * @param optimize
         * @return Assembly text as a string.
         */
        static std::string compileToAssembly(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source, bool optimize = false);

        /**
         * @brief Compiles a shader to a SPIR-V binary
         * @param sourceName
         * @param kind
         * @param source
         * @param optimize
         * @return Binary as a vector of 32-bit words.
         */
        static std::vector<uint32_t> compile(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source, bool optimize = false);

        /**
         *
         * @param extension
         * @return Shaderc shader kind for compile
         */
        static shaderc_shader_kind getKind(const std::string& extension);

    private:
        void createShaderModule(const std::vector<uint32_t>& code);

    private:
        VkDevice device{};
        VkShaderModule module{};
        VkShaderStageFlagBits stage{};
    };

} // namespace re


#endif //RAVENENGINE_SHADER_HPP
