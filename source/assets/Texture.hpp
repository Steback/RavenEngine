#ifndef RAVENENGINE_TEXTURE_HPP
#define RAVENENGINE_TEXTURE_HPP


#include <memory>

#include "Asset.hpp"
#include "render/Image.hpp"


namespace re {

    class Device;

    class Texture : public Image, public Asset {
        friend class AssetsManager;

    public:
        struct Sampler {
            VkFilter magFilter{VK_FILTER_LINEAR};
            VkFilter minFilter{VK_FILTER_LINEAR};
            VkSamplerAddressMode addressModeU{VK_SAMPLER_ADDRESS_MODE_REPEAT};
            VkSamplerAddressMode addressModeV{VK_SAMPLER_ADDRESS_MODE_REPEAT};
            VkSamplerAddressMode addressModeW{VK_SAMPLER_ADDRESS_MODE_REPEAT};

            static VkSamplerAddressMode getVkWrapMode(int32_t wrapMode);

            static VkFilter getVkFilterMode(int32_t filterMode);
        };

    public:
        Texture(std::string name, const std::shared_ptr<Device>& device, const std::string& fileName, const Sampler& sampler, bool cubeMap = false);

        ~Texture() override;

        void generateMipmaps(const std::shared_ptr<Device>& device_);

        void updateDescriptor();

        void createSampler(const Sampler& sampler);

        VkDescriptorImageInfo descriptor{};

    private:
        void loadFromFile(const std::string& fileName, const std::shared_ptr<Device>& device, const Sampler& sampler);

        void loadCubeMap(const std::string& fileName, const std::shared_ptr<Device>& device);

        VkSampler sampler{};
    };

} // namespace re


#endif //RAVENENGINE_TEXTURE_HPP
