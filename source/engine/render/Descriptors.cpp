#include "Descriptors.hpp"

#include <cassert>

#include "engine/core/Utils.hpp"


namespace re::Descriptors {

    Manager* Manager::singleton;

    Pool::Pool(VkDevice device, const std::vector<VkDescriptorPoolSize> &sizes, uint32_t maxSetsCount, VkDescriptorPoolCreateFlags flags)
            : device(device), maxSets(maxSetsCount) {
        VkDescriptorPoolCreateInfo descriptorPoolInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
        descriptorPoolInfo.pPoolSizes = sizes.data();
        descriptorPoolInfo.maxSets = maxSets;
        descriptorPoolInfo.flags = flags;

        checkResult(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &pool),
                    "Failed to create descriptor pool");
    }

    Pool::~Pool() {
        vkDestroyDescriptorPool(device, pool, nullptr);
    }

    bool Pool::allocateDescriptor(const VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorSet &descriptor) const {
        VkDescriptorSetAllocateInfo allocInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        allocInfo.descriptorPool = pool;
        allocInfo.pSetLayouts = &descriptorSetLayout;
        allocInfo.descriptorSetCount = 1;

        if (vkAllocateDescriptorSets(device, &allocInfo, &descriptor) != VK_SUCCESS)
            return false;

        return true;
    }

    void Pool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const {
        vkFreeDescriptorSets(device, pool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
    }

    void Pool::resetPool() {
        vkResetDescriptorPool(device, pool, 0);
    }

    Layout::Layout(VkDevice device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
            : device(device), bindings(std::move(bindings)) {
        std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{bindings.size()};

        for (auto kv : bindings)
            setLayoutBindings.push_back(kv.second);

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
        descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

        checkResult(vkCreateDescriptorSetLayout(device, &descriptorSetLayoutInfo, nullptr, &layout),
                    "Failed to create descriptor set layout!");
    }

    Layout::~Layout() {
        vkDestroyDescriptorSetLayout(device, layout, nullptr);
    }

    VkDescriptorSetLayout Layout::getDescriptorSetLayout() const {
        return layout;
    }

    Writer::Writer(Layout &layout, Pool &pool) : pool(pool), layout(layout) {

    }

    Writer &Writer::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) {
        assert(layout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

        auto &bindingDescription = layout.bindings[binding];

        assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pBufferInfo = bufferInfo;
        write.descriptorCount = 1;

        writes.push_back(write);
        return *this;
    }

    Writer &Writer::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) {
        assert(layout.bindings.count(binding) == 1 && "Layout does not contain specified binding");

        auto &bindingDescription = layout.bindings[binding];

        assert(bindingDescription.descriptorCount == 1 && "Binding single descriptor info, but binding expects multiple");

        VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
        write.descriptorType = bindingDescription.descriptorType;
        write.dstBinding = binding;
        write.pImageInfo = imageInfo;
        write.descriptorCount = 1;

        writes.push_back(write);
        return *this;
    }

    bool Writer::build(VkDescriptorSet &set) {
        bool success = pool.allocateDescriptor(layout.getDescriptorSetLayout(), set);

        if (!success) return false;

        overwrite(set);

        return true;
    }

    void Writer::overwrite(VkDescriptorSet &set) {
        for (auto &write : writes)
            write.dstSet = set;

        vkUpdateDescriptorSets(pool.device, writes.size(), writes.data(), 0, nullptr);
    }

    Manager::Manager(VkDevice device) : device(device) {

    }

    Manager::~Manager() = default;

    Manager *Manager::instance() {
        return singleton;
    }

    Pool &Manager::createPool(const std::vector<VkDescriptorPoolSize> &sizes, uint32_t maxSetsCount, VkDescriptorPoolCreateFlags flags) {
        pool = std::make_unique<Pool>(device, sizes, maxSetsCount, flags);
        return *pool;
    }

    Pool &Manager::getPool() {
        return *pool;
    }

} // namespace re::Descriptors
