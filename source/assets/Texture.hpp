#ifndef RAVENENGINE_TEXTURE_HPP
#define RAVENENGINE_TEXTURE_HPP


#include <memory>

#include "tiny_gltf.h"

#include "render/Image.hpp"


namespace re {

    class Device;

    /**
     * @brief Texture wrapper class.\n
     * Inheritance of Image class
     */
    class Texture : public Image {
        friend class AssetsManager;

    public:
        struct Sampler {
            VkFilter magFilter = VK_FILTER_LINEAR;
            VkFilter minFilter = VK_FILTER_LINEAR;
            VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

            static VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);

            static VkFilter getVkFilterMode(int32_t filterMode);
        };

    public:
        Texture(const std::shared_ptr<Device>& device, VkImageCreateInfo createInfo, VmaMemoryUsage usage);

        Texture(const std::shared_ptr<Device>& device, VkImageCreateInfo createInfo, VmaMemoryUsage usage, VkImageAspectFlagBits aspectFlagBits, const Sampler& sampler);

        ~Texture() override;

        void generateMipmaps(const std::shared_ptr<Device>& device_);

        void updateDescriptor();

        void createSampler(const Sampler& sampler);

        /**
         * @brief Load Texture from image file
         */
        static std::unique_ptr<Texture> loadFromFile(const std::shared_ptr<Device>& device, const std::string& fileName, const Sampler& sampler);

        static std::unique_ptr<Texture> loadCubeMap(const std::shared_ptr<Device>& device, const std::string& fileName);

    private:
        VkSampler sampler{};
        VkDescriptorImageInfo descriptor{};
    };

} // namespace re


#endif //RAVENENGINE_TEXTURE_HPP
