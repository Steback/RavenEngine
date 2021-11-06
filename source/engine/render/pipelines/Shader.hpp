#ifndef RAVENENGINE_SHADER_HPP
#define RAVENENGINE_SHADER_HPP


#include <string>
#include <vector>

#include "vulkan/vulkan.h"

#include "engine/files/File.hpp"
#include "engine/core/NonCopyable.hpp"


namespace re {

    /**
     * @brief Shader wrapper class
     */
    class Shader : NonCopyable {
    public:
        Shader(VkDevice device, const std::string& name);

        ~Shader() override;

        [[nodiscard]] VkPipelineShaderStageCreateInfo getPipelineStageCreateInfo(uint32_t flags = 0) const;

        static VkShaderStageFlagBits getStage(const std::string& ext);

        static void compileShader(const std::filesystem::path& file) ;

        void createShaderModule(const std::filesystem::path& file);

        VkShaderStageFlagBits stage;
        VkShaderModule module{};

    private:
        VkDevice device;
    };

} // namespace re


#endif //RAVENENGINE_SHADER_HPP
