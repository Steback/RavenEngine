#ifndef RAVENENGINE_TEXTURE_HPP
#define RAVENENGINE_TEXTURE_HPP


#include <memory>

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
        struct Info {
            VkImageCreateInfo& createInfo;
            VmaMemoryUsage usage;
            VkImageAspectFlagBits aspectFlagBits;
        };

        struct Sampler {
            VkFilter magFilter;
            VkFilter minFilter;
            VkSamplerAddressMode addressModeU;
            VkSamplerAddressMode addressModeV;
            VkSamplerAddressMode addressModeW;

            static VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);

            static VkFilter getVkFilterMode(int32_t filterMode);
        };

    public:
        Texture(const std::shared_ptr<Device>& device, const Info& info, const Sampler& sampler);

        ~Texture() override;

        void generateMipmaps(const std::shared_ptr<Device>& device_);

        void updateDescriptor();

    private:
        void createSampler(const Sampler& sampler);

    private:
        VkSampler sampler{};
        VkDescriptorImageInfo descriptor{};
    };

} // namespace re


#endif //RAVENENGINE_TEXTURE_HPP
