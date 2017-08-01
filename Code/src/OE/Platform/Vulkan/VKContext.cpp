#include "OE/Platform/Vulkan/VKContext.hpp"

#include <string>
#include <set>
#include <algorithm>

#include "OE/Application/WindowImpl.hpp"
#include "OE/Misc/Log.hpp"

#include "OE/Graphics/API/Shader.hpp"
#include "OE/Platform/Vulkan/VKShader.hpp"

namespace OrbitEngine { namespace Application { namespace priv {
	VKContext::VKContext(WindowImpl* window, VKContext* sharedContext)
		: ContextImpl(window)
	{
		makeCurrent(true);

		loadExtensions();
		loadValidationLayers();
		createInstance();
		pickPhysicalDevice();
		loadDeviceExtensions();
		createSurface(window);
		enumQueueFamilies();
		createLogicalDevice();
		createSwapChain();
		createRenderPass();
		createImageViewsAndFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createSemaphores();

		OE_LOG_DEBUG("VK initialized")
	}

	VKContext::~VKContext()
	{
		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
			vkDestroyImageView(m_Device, m_SwapChainImageViews[i], nullptr);
		vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
		vkDestroyDevice(m_Device, nullptr);
#if OE_DEBUG
		//vkDestroyDebugReportCallbackEXT(m_Instance, m_ReportCallback, nullptr);
#endif
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}

	void VKContext::prepare()
	{
		VkResult err = vkAcquireNextImageKHR(m_Device, m_SwapChain, std::numeric_limits<uint64_t>::max(), m_PresentSemaphore, VK_NULL_HANDLE, &m_SwapchainIndex);

		if ((err == VK_ERROR_OUT_OF_DATE_KHR) || (err == VK_SUBOPTIMAL_KHR)) {
			// TODO Recreate swapchain
			OE_LOG_DEBUG("TODO Recreate swapchain!")
		}
		else if (err != VK_SUCCESS) {
			OE_LOG_FATAL("Couldn't acquire next swap chain image")
			return;
		}

		m_CurrentCommandBuffer = m_CommandBuffers[m_SwapchainIndex];
	}

	void VKContext::present()
	{
		// Submit
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_PresentSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_RenderSemaphore;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CurrentCommandBuffer;

		if (vkQueueSubmit(m_QueueFamilies.graphics.queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
			OE_LOG_FATAL("Couldn't submit draw command buffer");
		}

		// Present
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain;
		presentInfo.pImageIndices = &m_SwapchainIndex;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderSemaphore;

		vkQueuePresentKHR(m_QueueFamilies.present.queue, &presentInfo);
		vkQueueWaitIdle(m_QueueFamilies.present.queue);
	}

	void VKContext::makeCurrent(bool active)
	{
		//
		ContextImpl::makeCurrent(active);
	}

	bool VKContext::isValidationLayerAvailable(const char* name)
	{
		for (const auto& layerProperties : m_ValidationLayers) {
			if (strcmp(name, layerProperties.layerName) == 0)
				return true;
		}
		return false;
	}

	bool VKContext::isExtensionAvailable(const char* name)
	{
		for (const auto& extensionProperties : m_Extensions) {
			if (strcmp(name, extensionProperties.extensionName) == 0)
				return true;
		}
		return false;
	}

	bool VKContext::isDeviceExtensionAvailable(const char* name)
	{
		for (const auto& extensionProperties : m_DeviceExtensions) {
			if (strcmp(name, extensionProperties.extensionName) == 0)
				return true;
		}
		return false;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VKContext::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
	{
		OE_LOG_ERROR("validation layer: " << msg);

		return VK_FALSE;
	}

	void VKContext::loadExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		m_Extensions.resize(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, m_Extensions.data());

		OE_LOG_DEBUG(" -- Available Vulkan instance extensions:")
		for (const auto& extension : m_Extensions)
			OE_LOG_DEBUG(extension.extensionName)
	}

	void VKContext::loadValidationLayers()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		m_ValidationLayers.resize(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, m_ValidationLayers.data());
		
		OE_LOG_DEBUG(" -- Available Vulkan validation layers:")
		for (const auto& validationLayer : m_ValidationLayers)
			OE_LOG_DEBUG(validationLayer.layerName)
	}

	void VKContext::createInstance()
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pEngineName = "OrbitEngine";
		appInfo.pApplicationName = OE_APPNAME;
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledLayerCount = 0;
		createInfo.pApplicationInfo = &appInfo;

		#define OE_VK_ADD_EXTENSION_IF_AVAILABLE(x) if(isExtensionAvailable(##x)) { extensions.push_back(##x); }
		std::vector<const char*> extensions;

		OE_VK_ADD_EXTENSION_IF_AVAILABLE(VK_KHR_SURFACE_EXTENSION_NAME)
#if OE_WINDOWS
		OE_VK_ADD_EXTENSION_IF_AVAILABLE(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
#endif

#if OE_DEBUG
		if(isExtensionAvailable(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)){
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

			#define OE_VK_ADD_VALIDATION_LAYER_IF_AVAILABLE(x) if(isValidationLayerAvailable(#x)) { validationLayers.push_back(#x); }
			std::vector<const char*> validationLayers;

			OE_VK_ADD_VALIDATION_LAYER_IF_AVAILABLE("VK_LAYER_LUNARG_standard_validation")

			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
#endif
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		if (result != VK_SUCCESS) {
			OE_LOG_FATAL("Couldn't create the Vulkan instance")
			return;
		}

#if OE_DEBUG
		createDebugCallback();
#endif
	}

	void VKContext::createDebugCallback()
	{
		VkDebugReportCallbackCreateInfoEXT createInfoExt = {};
		createInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfoExt.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
		createInfoExt.pfnCallback = debugCallback;


		auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugReportCallbackEXT");
		if (func(m_Instance, &createInfoExt, nullptr, &m_ReportCallback) != VK_SUCCESS) {
			OE_LOG_ERROR("Couldn't setup report callback!")
		}
	}

	void VKContext::pickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			OE_LOG_FATAL("Couldn't find a suitable GPU")
			return;
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());
		
		// TODO
		// For now, just pick the first card
		m_PhysicalDevice = devices[0];
	}

	void VKContext::loadDeviceExtensions()
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);
		m_DeviceExtensions.resize(extensionCount);
		vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, m_DeviceExtensions.data());

		OE_LOG_DEBUG(" -- Available Vulkan device extensions:")
			for (const auto& extension : m_DeviceExtensions)
				OE_LOG_DEBUG(extension.extensionName)
	}
	
	void VKContext::createSurface(WindowImpl* window)
	{
		auto CreateSurfaceKHR =
#if OE_WINDOWS
		(PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(m_Instance, "vkCreateWin32SurfaceKHR");
#else
			0;
#endif

#if OE_WINDOWS
		VkWin32SurfaceCreateInfoKHR createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = window->getWindowNativeHandle();
		createInfo.hinstance = GetModuleHandle(nullptr);
#endif

		if (CreateSurfaceKHR) {
			VkResult result = CreateSurfaceKHR(m_Instance, &createInfo, nullptr, &m_Surface);
			if (result == VK_SUCCESS)
				return;
		}
		OE_LOG_FATAL("Couldn't create the VK Surface")
	}

	void VKContext::enumQueueFamilies()
	{
		m_QueueFamilies = {};

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if(queueFamily.queueCount > 0){
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					m_QueueFamilies.graphics.index = i;

				VkBool32 presentationQueue = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, m_Surface, &presentationQueue);
				if (presentationQueue)
					m_QueueFamilies.present.index = i;
			}
		}
	}

	void VKContext::createLogicalDevice()
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { // Eliminate duplicates
			m_QueueFamilies.graphics.index,
			m_QueueFamilies.present.index,
		};

		float queuePriority = 1.0f;
		for(int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledLayerCount = 0;
		
		std::vector<const char*> deviceExtensions;
		#define OE_VK_ADD_DEVICE_EXTENSION_IF_AVAILABLE(x) if(isDeviceExtensionAvailable(##x)) { deviceExtensions.push_back(##x); }
		
		OE_VK_ADD_DEVICE_EXTENSION_IF_AVAILABLE(VK_KHR_SWAPCHAIN_EXTENSION_NAME)

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (isExtensionAvailable(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
			std::vector<const char*> validationLayers;

			OE_VK_ADD_VALIDATION_LAYER_IF_AVAILABLE("VK_LAYER_LUNARG_standard_validation")

			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS) {
			OE_LOG_FATAL("Failed to create logical device!")
			return;
		}

		vkGetDeviceQueue(m_Device, m_QueueFamilies.graphics.index, 0, &m_QueueFamilies.graphics.queue);
		vkGetDeviceQueue(m_Device, m_QueueFamilies.present.index, 0, &m_QueueFamilies.present.queue);
	}

	void VKContext::createSwapChain()
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		/* Retrieve surface capabilities */
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, m_Surface, &capabilities);

		/* Enum formats */
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, nullptr);
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, m_Surface, &formatCount, formats.data());

		/* Enum present modes */
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, nullptr);
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, m_Surface, &presentModeCount, presentModes.data());

		/* Create the swap chain */
		if (formatCount == 0 || presentModeCount == 0) {
			OE_LOG_FATAL("VkSwapChain not supported")
			return;
		}

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(presentModes);
		VkExtent2D extent = ChooseSwapExtent(capabilities);

		// Create it
		uint32_t imageCount = capabilities.minImageCount + 1;
		if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
			imageCount = capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = { // Eliminate duplicates
			m_QueueFamilies.graphics.index,
			m_QueueFamilies.present.index,
		};

		if (m_QueueFamilies.graphics.index != m_QueueFamilies.present.index) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
			OE_LOG_FATAL("Couldn't create the swap chain!")
			return;
		}

		/* Retrive swap chain images */
		// TODO Change this in the future
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
		m_SwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

		m_SwapChainImageFormat = surfaceFormat.format;
		m_SwapChainExtent = extent;
	}

	void VKContext::createRenderPass()
	{
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_SwapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
			OE_LOG_FATAL("Couldn't create render pass")
		}
	}

	void VKContext::createImageViewsAndFramebuffers()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());
		m_SwapChainFramebuffers.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
			// Image View
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_SwapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_SwapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS) {
				OE_LOG_FATAL("Couldn't create image view")
			}

			// Framebuffer
			VkImageView attachments[] = {
				m_SwapChainImageViews[i]
			};
			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			framebufferInfo.layers = 1;
			if (vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
				OE_LOG_FATAL("Couldn't create framebuffer")
			}
		}
	}
	
	void VKContext::createCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = m_QueueFamilies.graphics.index;

		if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS) {
			OE_LOG_FATAL("Couldn't create the command pool")
		}
	}

	void VKContext::createCommandBuffers()
	{
		m_CommandBuffers.resize(m_SwapChainFramebuffers.size());
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();
		if (vkAllocateCommandBuffers(m_Device, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
		for (size_t i = 0; i < m_CommandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo);
			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_RenderPass;
			renderPassInfo.framebuffer = m_SwapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_SwapChainExtent;
			VkClearValue clearColor = { 0.25f, 0.5f, 1.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			//vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

			//vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_CommandBuffers[i]);

			if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS) {
				OE_LOG_FATAL("Couldn't record command buffer")
			}
		}
	}

	void VKContext::createSemaphores()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_PresentSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderSemaphore) != VK_SUCCESS) {
				OE_LOG_FATAL("Couldn't create the semaphores")
		}
	}

	VkSurfaceFormatKHR VKContext::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VKContext::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	VkExtent2D VKContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { 800, 600 };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}
} } }