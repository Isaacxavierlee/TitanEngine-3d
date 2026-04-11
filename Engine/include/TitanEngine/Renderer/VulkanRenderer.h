#pragma once

struct GLFWwindow;

namespace Titan {

    class VulkanRenderer {
    public:
        static VulkanRenderer& Get();

        bool Init(GLFWwindow* window);
        void Shutdown();
        void NewFrame();
        void EndFrame();
        void WaitIdle();

        // Getters for ImGui
        void* GetInstance() const { return m_Instance; }
        void* GetPhysicalDevice() const { return m_PhysicalDevice; }
        void* GetDevice() const { return m_Device; }
        void* GetQueue() const { return m_GraphicsQueue; }
        uint32_t GetQueueFamily() const { return m_QueueFamily; }
        void* GetRenderPass() const { return m_RenderPass; }
        void* GetDescriptorPool() const { return m_DescriptorPool; }
        int GetMinImageCount() const { return 2; }

    private:
        VulkanRenderer() = default;
        ~VulkanRenderer() = default;
        VulkanRenderer(const VulkanRenderer&) = delete;
        VulkanRenderer& operator=(const VulkanRenderer&) = delete;

        void* m_Instance = nullptr;
        void* m_PhysicalDevice = nullptr;
        void* m_Device = nullptr;
        void* m_GraphicsQueue = nullptr;
        void* m_Surface = nullptr;
        void* m_RenderPass = nullptr;
        void* m_DescriptorPool = nullptr;
        uint32_t m_QueueFamily = 0;
        bool m_Initialized = false;
    };

} // namespace Titan