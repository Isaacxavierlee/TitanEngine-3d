#include "TitanEngine/Core/ImGuiLayer.h"
#include "TitanEngine/Renderer/VulkanRenderer.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Titan {

    ImGuiLayer& ImGuiLayer::Get() {
        static ImGuiLayer instance;
        return instance;
    }

    void ImGuiLayer::Init(GLFWwindow* window) {
        if (m_Initialized) return;

        std::cout << "Initializing ImGui with Vulkan..." << std::endl;

        // Initialize Vulkan first
        if (!VulkanRenderer::Get().Init(window)) {
            std::cerr << "Failed to initialize Vulkan for ImGui!" << std::endl;
            return;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        // Setup ImGui for Vulkan
        ImGui_ImplGlfw_InitForVulkan(window, true);

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = reinterpret_cast<VkInstance>(VulkanRenderer::Get().GetInstance());
        initInfo.PhysicalDevice = reinterpret_cast<VkPhysicalDevice>(VulkanRenderer::Get().GetPhysicalDevice());
        initInfo.Device = reinterpret_cast<VkDevice>(VulkanRenderer::Get().GetDevice());
        initInfo.QueueFamily = VulkanRenderer::Get().GetQueueFamily();
        initInfo.Queue = reinterpret_cast<VkQueue>(VulkanRenderer::Get().GetQueue());
        initInfo.DescriptorPool = reinterpret_cast<VkDescriptorPool>(VulkanRenderer::Get().GetDescriptorPool());
        initInfo.RenderPass = reinterpret_cast<VkRenderPass>(VulkanRenderer::Get().GetRenderPass());
        initInfo.MinImageCount = VulkanRenderer::Get().GetMinImageCount();
        initInfo.ImageCount = VulkanRenderer::Get().GetMinImageCount();
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&initInfo);

        // Upload fonts
        ImGui_ImplVulkan_CreateFontsTexture();

        m_Initialized = true;
        std::cout << "ImGui with Vulkan initialized!" << std::endl;
    }

    void ImGuiLayer::Shutdown() {
        if (!m_Initialized) return;

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        VulkanRenderer::Get().Shutdown();

        m_Initialized = false;
    }

    void ImGuiLayer::NewFrame() {
        if (!m_Initialized) return;

        VulkanRenderer::Get().BeginFrame();
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::Render() {
        if (!m_Initialized) return;

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData());

        VulkanRenderer::Get().EndFrame();

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

} // namespace Titan