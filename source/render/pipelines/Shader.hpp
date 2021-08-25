#ifndef RAVENENGINE_SHADER_HPP
#define RAVENENGINE_SHADER_HPP


#include <string>
#include <vector>

#include "vulkan/vulkan.h"

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

    private:
        void createShaderModule(const std::vector<uint32_t>& code);

    private:
        VkDevice device{};
        VkShaderModule module{};
        VkShaderStageFlagBits stage{};
    };

} // namespace re


#endif //RAVENENGINE_SHADER_HPP
