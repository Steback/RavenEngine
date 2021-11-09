#ifndef RAVENENGINE_DESCRIPTORS_HPP
#define RAVENENGINE_DESCRIPTORS_HPP


#include <vector>
#include <unordered_map>
#include <memory>

#include "vulkan/vulkan.h"

#include "engine/core/NonCopyable.hpp"


namespace re {

    class Engine;

    namespace Descriptors {

        class Pool : re::NonCopyable {
            friend class Writer;

        public:
            Pool(VkDevice device, const std::vector<VkDescriptorPoolSize> &sizes, uint32_t maxSetsCount = 1000, VkDescriptorPoolCreateFlags flags = 0);

            ~Pool() override;

            bool
            allocateDescriptor(const VkDescriptorSetLayout &descriptorSetLayout, VkDescriptorSet &descriptor) const;

            void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

            void resetPool();

        private:
            VkDevice device;
            VkDescriptorPool pool{};
            uint32_t maxSets;
        };

        class Layout : re::NonCopyable {
            friend class Writer;

        public:
            Layout(VkDevice device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

            ~Layout() override;

            [[nodiscard]] VkDescriptorSetLayout getDescriptorSetLayout() const;

        private:
            VkDevice device;
            VkDescriptorSetLayout layout{};
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
        };

        class Writer {
        public:
        public:
            Writer(Layout &layout, Pool &pool);

            Writer &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);

            Writer &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

            bool build(VkDescriptorSet &set);

            void overwrite(VkDescriptorSet &set);

        private:
            Layout &layout;
            Pool &pool;
            std::vector<VkWriteDescriptorSet> writes;
        };

        class Manager : NonCopyable {
            friend re::Engine;

            explicit Manager(VkDevice device);

        public:
            ~Manager() override;

            static Manager* instance();

            Pool& createPool(const std::vector<VkDescriptorPoolSize> &sizes, uint32_t maxSetsCount = 1000, VkDescriptorPoolCreateFlags flags = 0);

            Pool& getPool();

        private:
            static Manager* singleton;
            VkDevice device;
            std::unique_ptr<Pool> pool;
        };

    } // namespace Descriptors

    typedef Descriptors::Manager DescriptorsManager;
    typedef Descriptors::Pool DescriptorsPool;
    typedef Descriptors::Layout DescriptorsLayout;
    typedef Descriptors::Writer DescriptorsWriter;

} // namespace re


#endif //RAVENENGINE_DESCRIPTORS_HPP
