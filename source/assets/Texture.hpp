#ifndef RAVENENGINE_TEXTURE_HPP
#define RAVENENGINE_TEXTURE_HPP


#include "render/Image.hpp"


namespace re {

    /**
     * @brief Texture wrapper class.\n
     * Inheritance of Image class
     */
    class Texture : public Image {
    public:
        Texture(VkDevice device, VmaAllocator allocator, const VkImageCreateInfo& imageInfo, VmaMemoryUsage usage,
                VkImageAspectFlags aspectFlags);

        ~Texture() override;

        void createDescriptorSet(VkDescriptorPool pool, VkDescriptorSetLayout layout);

        [[nodiscard]] VkDescriptorSet getDescriptorSet() const;

    private:
        void createSampler();

    private:
        VkSampler sampler{};
        VkDescriptorSet descriptorSet{};
    };

} // namespace re


#endif //RAVENENGINE_TEXTURE_HPP
