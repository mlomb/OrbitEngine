#ifndef GRAPHICS_VKCONTEXT_HPP
#define GRAPHICS_VKCONTEXT_HPP

#include "OE/Platform/Vulkan/Vulkan.hpp"
#include "OE/Application/ContextImpl.hpp"

#include <vector>

namespace OrbitEngine { namespace Application { namespace priv {
	struct VKQueueFamily {
		uint32_t index = -1u;
		VkQueue queue = VK_NULL_HANDLE;
	};

	struct VKQueueFamilies {
		VKQueueFamily graphics = {};
		VKQueueFamily present = {};
	};

	class VKContext : public ContextImpl {
	public:
		VKContext(WindowImpl* window, VKContext* sharedContext);
		~VKContext();
		
		void prepare();
		void present();

		void makeCurrent(bool active);

		bool isValidationLayerAvailable(const char* name);
		bool isExtensionAvailable(const char* name);
		bool isDeviceExtensionAvailable(const char* name);

		const VkDevice getDevice() { return m_Device; }
		const VkExtent2D getExtent() { return m_SwapChainExtent; }
		const VkRenderPass getRenderPass() { return m_RenderPass; }

		const RenderAPI getAPI() override { return RenderAPI::VULKAN; };
		const std::string getName() override { return "Vulkan"; }

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

		static VKContext* GetCurrent() { return (VKContext*)ContextImpl::GetCurrent(); };
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	protected:

	private:
		void loadExtensions();
		void loadValidationLayers();
		void createInstance();
		void createDebugCallback();
		void pickPhysicalDevice();
		void loadDeviceExtensions();
		void createSurface(WindowImpl* window);
		void enumQueueFamilies();
		void createLogicalDevice();
		void createSwapChain();
		void createRenderPass();
		void createImageViewsAndFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSemaphores();

		// TODO Temporal
		VkRenderPass m_RenderPass;
		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CurrentCommandBuffer;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		// Vulkan
		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;
		VkSurfaceKHR m_Surface;
		VKQueueFamilies m_QueueFamilies;

		// Debug
		VkDebugReportCallbackEXT m_ReportCallback;
		
		// Swapchain
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		uint32_t m_SwapchainIndex;

		// Semaphores
		VkSemaphore m_PresentSemaphore;
		VkSemaphore m_RenderSemaphore;

		// Features
		std::vector<VkExtensionProperties> m_Extensions;
		std::vector<VkExtensionProperties> m_DeviceExtensions;
		std::vector<VkLayerProperties> m_ValidationLayers;
	};
} } }

#endif
