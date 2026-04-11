#include "TitanEngine/Renderer/VulkanRenderer.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

namespace Titan {

    VulkanRenderer& VulkanRenderer::Get() {
        static VulkanRenderer instance;
        return instance;
    }

    bool VulkanRenderer::Init(GLFWwindow* window) {
        if (m_Initialized) return true;

        std::cout << "Initializing Vulkan..." << std::endl;

        // Get required extensions from GLFW
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // Create Vulkan instance
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "TitanEngine";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "TitanEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceInfo{};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceInfo.ppEnabledExtensionNames = extensions.data();

        VkInstance instance;
        if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS) {
            std::cerr << "Failed to create Vulkan instance!" << std::endl;
            return false;
        }
        m_Instance = reinterpret_cast<void*>(instance);

        // Create window surface
        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            std::cerr << "Failed to create window surface!" << std::endl;
            return false;
        }
        m_Surface = reinterpret_cast<void*>(surface);

        // Pick physical device
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            std::cerr << "No Vulkan capable GPU found!" << std::endl;
            return false;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        m_PhysicalDevice = reinterpret_cast<void*>(devices[0]);

        // Find graphics queue family
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &queueFamilyCount, queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_QueueFamily = i;
                break;
            }
        }

        // Create logical device
        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueInfo{};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = m_QueueFamily;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;

        VkDeviceCreateInfo deviceInfo{};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.queueCreateInfoCount = 1;
        deviceInfo.pQueueCreateInfos = &queueInfo;

        VkDevice device;
        if (vkCreateDevice(devices[0], &deviceInfo, nullptr, &device) != VK_SUCCESS) {
            std::cerr << "Failed to create logical device!" << std::endl;
            return false;
        }
        m_Device = reinterpret_cast<void*>(device);

        // Get graphics queue
        VkQueue graphicsQueue;
        vkGetDeviceQueue(device, m_QueueFamily, 0, &graphicsQueue);
        m_GraphicsQueue = reinterpret_cast<void*>(graphicsQueue);

        // Create descriptor pool for ImGui
        VkDescriptorPoolSize poolSizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 100},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100}
        };

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 100;
        poolInfo.poolSizeCount = sizeof(poolSizes) / sizeof(poolSizes[0]);
        poolInfo.pPoolSizes = poolSizes;

        VkDescriptorPool descriptorPool;
        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            std::cerr << "Failed to create descriptor pool!" << std::endl;
            return false;
        }
        m_DescriptorPool = reinterpret_cast<void*>(descriptorPool);

        // Create minimal render pass
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        VkRenderPass renderPass;
        if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
            std::cerr << "Failed to create render pass!" << std::endl;
            return false;
        }
        m_RenderPass = reinterpret_cast<void*>(renderPass);

        m_Initialized = true;
        std::cout << "Vulkan initialized successfully!" << std::endl;

        return true;
    }

    void VulkanRenderer::Shutdown() {
        if (!m_Initialized) return;

        std::cout << "Shutting down Vulkan..." << std::endl;

        if (m_DescriptorPool) {
            vkDestroyDescriptorPool(reinterpret_cast<VkDevice>(m_Device),
                reinterpret_cast<VkDescriptorPool>(m_DescriptorPool), nullptr);
            m_DescriptorPool = nullptr;
        }
        if (m_RenderPass) {
            vkDestroyRenderPass(reinterpret_cast<VkDevice>(m_Device),
                reinterpret_cast<VkRenderPass>(m_RenderPass), nullptr);
            m_RenderPass = nullptr;
        }
        if (m_Device) {
            vkDestroyDevice(reinterpret_cast<VkDevice>(m_Device), nullptr);
            m_Device = nullptr;
        }
        if (m_Surface) {
            vkDestroySurfaceKHR(reinterpret_cast<VkInstance>(m_Instance),
                reinterpret_cast<VkSurfaceKHR>(m_Surface), nullptr);
            m_Surface = nullptr;
        }
        if (m_Instance) {
            vkDestroyInstance(reinterpret_cast<VkInstance>(m_Instance), nullptr);
            m_Instance = nullptr;
        }

        m_Initialized = false;
        std::cout << "Vulkan shutdown complete!" << std::endl;
    }

    void VulkanRenderer::NewFrame() {
        // Placeholder for frame start
    }

    void VulkanRenderer::EndFrame() {
        // Placeholder for frame end
    }

    void VulkanRenderer::WaitIdle() {
        if (m_Device) {
            vkDeviceWaitIdle(reinterpret_cast<VkDevice>(m_Device));
        }
    }

} // namespace Titan