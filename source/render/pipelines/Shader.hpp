#ifndef RAVENENGINE_SHADER_HPP
#define RAVENENGINE_SHADER_HPP


#include <string>
#include <vector>

#include "vulkan/vulkan.h"
#include "shaderc/shaderc.hpp"

#include "files/File.hpp"
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
         * @brief Compile shader and save it in SPIR-V format in "shaders" directory
         * @param code
         * @param stage
         */
        static void compileShader(File& file, const VkShaderStageFlagBits& stage);

        /**
         *
         * @param sourceName
         * @param kind
         * @param source
         * @return GLSL shader source text after preprocessing.
         */
        static std::string preprocessShader(const std::string& sourceName, shaderc_shader_kind kind, const std::string& source);

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
        static shaderc_shader_kind getKind(const VkShaderStageFlagBits& stage);

    private:
        void createShaderModule(const std::vector<uint32_t>& code);

    private:
        VkDevice device{};
        VkShaderModule module{};
        VkShaderStageFlagBits stage{};
    };

} // namespace re


#endif //RAVENENGINE_SHADER_HPP
