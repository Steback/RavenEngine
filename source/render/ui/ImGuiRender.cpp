#include "ImGuiRender.hpp"

#include "render/Instance.hpp"
#include "render/Device.hpp"
#include "render/Window.hpp"
#include "render/SwapChain.hpp"
#include "files/FilesManager.hpp"
#include "files/File.hpp"


namespace re::ui {

    ImGuiRender::ImGuiRender(const std::shared_ptr<Device> &device, const SwapChain &swapChain, const std::shared_ptr<Window> &window) {
        logicalDevice = device->getDevice();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->AddFontFromFileTTF(files::getFile("fonts/Roboto-Medium.ttf").getPath().c_str(), 16.0f);

        createDescriptorPool();

        ImGui_ImplGlfw_InitForVulkan(window->getHandle(), true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = device->getInstance()->getInstance();
        init_info.PhysicalDevice = device->getPhysicalDevice();
        init_info.Device = logicalDevice;
        init_info.QueueFamily = device->getQueueFamilyIndices().graphics;
        init_info.Queue = device->getQueue();
        init_info.DescriptorPool = descriptorPool;
        init_info.MinImageCount = swapChain.getImageCount();
        init_info.ImageCount = swapChain.getImageCount();
        // TODO: Check allocation in ImGui
        init_info.Allocator = VK_NULL_HANDLE;
        ImGui_ImplVulkan_Init(&init_info, swapChain.getRenderPass());

        VkCommandPool commandPool;
        device->createCommandPool(commandPool, device->getQueueFamilyIndices().graphics);

        VkCommandBuffer commandBuffer = device->beginSingleTimeCommands(commandPool);
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        device->endSingleTimeCommands(commandBuffer, device->getQueue(), commandPool);
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
    }

    ImGuiRender::~ImGuiRender() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        vkDestroyDescriptorPool(logicalDevice, descriptorPool, nullptr);
    }

    void ImGuiRender::resize(const SwapChain &swapChain) {
        ImGui_ImplVulkan_SetMinImageCount(swapChain.getImageCount());
    }

    void ImGuiRender::newFrame() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRender::render(VkCommandBuffer commandBuffer) {
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    void ImGuiRender::createDescriptorPool() {
        VkDescriptorPoolSize pool_sizes[] = {
                {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
        };

        VkDescriptorPoolCreateInfo pool_info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        RE_VK_CHECK_RESULT(vkCreateDescriptorPool(logicalDevice, &pool_info, nullptr, &descriptorPool),
                           "failed to set up descriptor pool for ImGui");
    }

} // namespace re::ui
