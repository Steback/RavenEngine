#include "Instance.hpp"

#include "GLFW/glfw3.h"

#include "logs/Logs.hpp"
#include "utils/Utils.hpp"


#ifdef RE_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
    re::logs::error(pCallbackData->pMessage);

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;  // Optional
}
#endif


namespace re {

    Instance::Instance(const char* appName, const std::vector<const char*>& layers) {
#ifdef RE_DEBUG
        if (!checkLayersSupport(layers)) throwEx("Validation layers requested, but not available!");
#endif

        VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
        appInfo.pApplicationName = appName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Raven Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo instanceInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledLayerCount = layers.size();
        instanceInfo.ppEnabledLayerNames = layers.data();

        auto extensions = getRequiredExtensions();
        instanceInfo.enabledExtensionCount = extensions.size();
        instanceInfo.ppEnabledExtensionNames = extensions.data();

        checkResult(vkCreateInstance(&instanceInfo, nullptr, &instance),
                    "Failed to create Instance");

#ifdef RE_DEBUG
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo{};
        populateDebugMessengerCreateInfo(debugUtilsMessengerInfo);

        checkResult(CreateDebugUtilsMessengerEXT(instance, &debugUtilsMessengerInfo, nullptr, &debugMessenger),
                    "Failed to create debug utils messenger");
#endif
    }

    Instance::~Instance() {
#ifdef RE_DEBUG
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif

        vkDestroyInstance(instance, nullptr);
    }

    VkPhysicalDevice Instance::pickPhysicalDevice(const std::vector<const char *> &extensions) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) throwEx("Failed to find GPUs with Vulkan support!");

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

        for (auto& device : physicalDevices)
            if (checkExtensionsSupport(device, extensions)) return device;

        throwEx("Failed to find a suitable GPU!");
    }

    bool Instance::checkExtensionsSupport(VkPhysicalDevice device, const std::vector<const char *> &extensions) {
        uint32_t extensionsCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, nullptr);

        std::vector<VkExtensionProperties> properties(extensionsCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionsCount, properties.data());

        return std::all_of(extensions.begin(), extensions.end(), [&properties](const char* name){
            return std::find_if(properties.begin(), properties.end(), [&name](const VkExtensionProperties& property){
                return std::strcmp(property.extensionName, name) == 0;
            }) != properties.end();
        });
    }

    VkInstance Instance::getInstance() const {
        return instance;
    }

    std::vector<const char *> Instance::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef RE_DEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    bool Instance::checkLayersSupport(const std::vector<const char*>& layers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        return std::all_of(layers.begin(), layers.end(), [&availableLayers](const char* name){
            return std::find_if(availableLayers.begin(), availableLayers.end(), [&name](const VkLayerProperties& property){
                return std::strcmp(property.layerName, name) == 0;
            }) != availableLayers.end();
        });
    }

} // namespace re