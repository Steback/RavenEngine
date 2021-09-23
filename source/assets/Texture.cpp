#include "Texture.hpp"

#include "stb_image.h"
#include "ktx.h"

#include "utils/Utils.hpp"
#include "render/Device.hpp"
#include "render/buffers/Buffer.hpp"
#include "files/FilesManager.hpp"


namespace re {

    VkSamplerAddressMode Texture::Sampler::getVkWrapMode(int32_t wrapMode) {
        switch (wrapMode) {
            case 10497:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case 33071:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case 33648:
                return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        }

        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }

    VkFilter Texture::Sampler::getVkFilterMode(int32_t filterMode) {
        switch (filterMode) {
            case 9728:
                return VK_FILTER_NEAREST;
            case 9729:
                return VK_FILTER_LINEAR;
            case 9984:
                return VK_FILTER_NEAREST;
            case 9985:
                return VK_FILTER_NEAREST;
            case 9986:
                return VK_FILTER_LINEAR;
            case 9987:
                return VK_FILTER_LINEAR;
        }

        return VK_FILTER_LINEAR;
    }

    /**
     * @brief Construct Texture. Just create the Vulkan Image and allocate memory. Not create Image View and Sampler
     * @param device Pointer to Device
     * @param createInfo Vulkan Image Create Info
     * @param usage VMA(Vulkan Memory Allocator) memory usage
     */
    Texture::Texture(const std::shared_ptr<Device>& device, VkImageCreateInfo createInfo, VmaMemoryUsage usage)
            : Image(device->getDevice(), device->getAllocator(), createInfo, usage) {

    }

    /**
     * @brief Construct Texture. Create Image, Image View, allocate memory and create Sampler.
     * @param device Pointer to Device
     * @param createInfo Vulkan Image Create Info
     * @param usage VMA(Vulkan Memory Allocator) memory usage
     * @param aspectFlagBits Vulkan Image View aspect flags
     * @param sampler Texture Sampler data
     */
    Texture::Texture(const std::shared_ptr<Device>& device, VkImageCreateInfo createInfo, VmaMemoryUsage usage, VkImageAspectFlagBits aspectFlagBits, const Sampler& sampler)
            : Image(device->getDevice(), device->getAllocator(), createInfo, usage, aspectFlagBits) {
        createSampler(sampler);
    }

    Texture::~Texture() {
        vkDestroySampler(device, sampler, nullptr);
    }

    /**
     * @brief Generate Texture mipmap levels
     * @param device_ Pointer to Device
     */
    void Texture::generateMipmaps(const std::shared_ptr<Device> &device_) {
        // Check if image format supports linear blitting
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(device_->getPhysicalDevice(), format, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
            throwEx("texture image format does not support linear blitting!");
        }

        VkCommandBuffer commandBuffer = device_->beginSingleTimeCommands();

        VkImageMemoryBarrier barrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        auto mipWidth = static_cast<int32_t>(extent.width);
        auto mipHeight = static_cast<int32_t>(extent.height);

        for (uint32_t i = 1; i < mipLevels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer,
                           image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1, &blit,
                           VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                                 0, nullptr,
                                 0, nullptr,
                                 1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                             0, nullptr,
                             0, nullptr,
                             1, &barrier);

        device_->endSingleTimeCommands(commandBuffer);
    }

    /**
     *
     * @param textureSampler Texture Sampler data
     */
    void Texture::createSampler(const Sampler& textureSampler) {
        VkSamplerCreateInfo samplerInfo{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
        samplerInfo.magFilter = textureSampler.magFilter;
        samplerInfo.minFilter = textureSampler.minFilter;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.addressModeU = textureSampler.addressModeU;
        samplerInfo.addressModeV = textureSampler.addressModeV;
        samplerInfo.addressModeW = textureSampler.addressModeW;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(mipLevels);
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
    }

    // TODO: Remove this and texture descriptor form Texture class
    void Texture::updateDescriptor() {
        descriptor.sampler = sampler;
        descriptor.imageView = view;
        descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    /**
     * @brief Load Texture form image file
     * @param device Pointer to Device
     * @param fileName Image file name
     * @param sampler Texture Sampler data
     * @return Pointer to Texture
     */
    std::unique_ptr<Texture> Texture::loadFromFile(const std::shared_ptr<Device>& device, const std::string& fileName, const Sampler& sampler) {
        int channels, imageWidth, imageHeight;
        stbi_uc* pixels = stbi_load(files::getFile("textures/" + fileName).getPath().c_str(), &imageWidth, &imageHeight, &channels, STBI_rgb_alpha);

        if (!pixels) throwEx("Failed to load image file: " + fileName);

        uint32_t width = imageWidth;
        uint32_t height = imageHeight;
        auto mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height))));
        VkDeviceSize size = width * height * static_cast<int>(STBI_rgb_alpha);

        Buffer stagingBuffer(device->getAllocator(), size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.writeTo(pixels);
        stagingBuffer.unmap();

        stbi_image_free(pixels);

        VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
        VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent = {width, height, 1};
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        std::unique_ptr<Texture> texture = std::make_unique<Texture>(device, imageInfo, VMA_MEMORY_USAGE_CPU_COPY, VK_IMAGE_ASPECT_COLOR_BIT, sampler);

        device->transitionImageLayout(texture->getImage(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
        device->copyBufferToImage(stagingBuffer, *texture);

        texture->generateMipmaps(device);
        texture->updateDescriptor();

        return texture;
    }

    /**
     * @brief Load Texture cube map form .ktx file
     * @param device Pointer to Device
     * @param fileName File name
     * @return Pointer to Texture
     */
    std::unique_ptr<Texture> Texture::loadCubeMap(const std::shared_ptr<Device> &device, const std::string &fileName) {
        ktxTexture* ktxTexture;
        ktxResult result = ktxTexture_CreateFromNamedFile(files::getFile(fileName).getPath().c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &ktxTexture);

        if (result != KTX_SUCCESS)
            throwEx("Failed to open cubemap file: " + fileName);

        uint32_t width = ktxTexture->baseWidth;
        uint32_t height = ktxTexture->baseHeight;
        uint32_t mipLevels = ktxTexture->numLevels;
        ktx_uint8_t *ktxTextureData = ktxTexture_GetData(ktxTexture);
        ktx_size_t ktxTextureSize = ktxTexture_GetDataSize(ktxTexture);

        Buffer stagingBuffer(device->getAllocator(), ktxTextureSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
        stagingBuffer.map();
        stagingBuffer.writeTo(ktxTextureData);
        stagingBuffer.unmap();

        VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
        VkImageCreateInfo imageInfo{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent = {width, height, 1};
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = 6;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        std::unique_ptr<Texture> texture = std::make_unique<Texture>(device, imageInfo, VMA_MEMORY_USAGE_CPU_COPY);

        // Setup uboBuffer copy regions for each face including all of its miplevels
        std::vector<VkBufferImageCopy> bufferCopyRegions;
        uint32_t offset = 0;

        for (uint32_t face = 0; face < 6; face++) {
            for (uint32_t level = 0; level < mipLevels; level++) {
                // Calculate offset into staging uboBuffer for the current mip level and face
                ktx_size_t offset;
                KTX_error_code ret = ktxTexture_GetImageOffset(ktxTexture, level, 0, face, &offset);
                assert(ret == KTX_SUCCESS);
                VkBufferImageCopy bufferCopyRegion = {};
                bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                bufferCopyRegion.imageSubresource.mipLevel = level;
                bufferCopyRegion.imageSubresource.baseArrayLayer = face;
                bufferCopyRegion.imageSubresource.layerCount = 1;
                bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> level;
                bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
                bufferCopyRegion.imageExtent.depth = 1;
                bufferCopyRegion.bufferOffset = offset;
                bufferCopyRegions.push_back(bufferCopyRegion);
            }
        }

        // Image barrier for optimal image (target)
        // Set initial layout for all array layers (faces) of the optimal (target) tiled texture
        VkImageSubresourceRange subresourceRange = {};
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.baseMipLevel = 0;
        subresourceRange.levelCount = mipLevels;
        subresourceRange.layerCount = 6;

        VkCommandBuffer copyCmd = device->beginSingleTimeCommands();
        texture->setLayout(copyCmd, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);

        vkCmdCopyBufferToImage(
                copyCmd,
                stagingBuffer.getBuffer(),
                texture->image,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                static_cast<uint32_t>(bufferCopyRegions.size()),
                bufferCopyRegions.data()
        );

        texture->setLayout(copyCmd, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, subresourceRange);

        device->endSingleTimeCommands(copyCmd);

        Sampler sampler;
        sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        sampler.addressModeW = sampler.addressModeU;
        sampler.addressModeV = sampler.addressModeU;
        texture->createSampler(sampler);

        texture->createView(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE, 6);

        ktxTexture_Destroy(ktxTexture);

        return texture;
    }

} // namespace re
