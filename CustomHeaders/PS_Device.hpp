#pragma once
#include "PS_Window.hpp"
#include <vector>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>

namespace ps {

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else
		const bool enableValidationLayers = true;
	#endif

	class PS_Device {
	public:
		PS_Device() {

		}
		~PS_Device() {
			for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
				vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
			}

			for (size_t i = 0; i < swapChainImageViews.size(); i++) {
				vkDestroyImageView(device, swapChainImageViews[i], nullptr);
			}
			vkDestroySwapchainKHR(device, swapChain, nullptr);
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
				vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
				vkDestroyFence(device, inFlightFences[i], nullptr);
			}
			vkDestroyCommandPool(device, commandPool, nullptr);
			vkDestroyDevice(device, nullptr);

			if (enableValidationLayers) {
				DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			}
			if (surface != VK_NULL_HANDLE) {
				vkDestroySurfaceKHR(instance, surface, nullptr); // ?
			}
			vkDestroyInstance(instance, nullptr);
		}
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		std::vector<VkImageView> swapChainImageViews;

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		void createInstance();
		void pickPhysicalDevice(VkSurfaceKHR surface);
		void createLogicalDevice();

		bool checkValidationLayerSupport();
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		std::vector<const char*> getRequiredExtensions();
		void setupDebugMessenger();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		
		//
		// Swap Chain
		//
		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};
		void createSwapChain(VkSurfaceKHR surface, GLFWwindow* window);
		void createImageViews();
		void createFramebuffers(VkRenderPass renderPass);
		void createCommandPool();
		void createCommandBuffer();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, VkPipeline graphicsPipeline);
		void createSyncObjects();
		void drawFrame(VkRenderPass renderPass, VkPipeline graphicsPipeline, GLFWwindow* window);
		void recreateSwapChain(VkSurfaceKHR surface, GLFWwindow* window, VkRenderPass renderPass);

		//
		// Getters
		//
		VkInstance getInstance() {
			return instance;
		}
		VkDevice getDevice() {
			return device;
		}
		VkPhysicalDevice getPhysicalDevice() {
			return physicalDevice;
		}
		VkFormat getSwapChainImageFormat() {
			return swapChainImageFormat;
		}

		// 
		// Setters
		//
		void setSurface(VkSurfaceKHR surface) {
			this->surface = surface;
		}
		void setFrameBufferResized(boolean b) {
			framebufferResized = b;
		}

	private:
		PS_Logger psLogger{};

		VkQueue graphicsQueue;
		VkQueue presentQueue;
		VkDevice device;
		VkSurfaceKHR surface;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		std::vector<VkCommandBuffer> commandBuffers;
		const int MAX_FRAMES_IN_FLIGHT = 2;
		bool framebufferResized = false;

		//
		// Swap Chain
		//
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

		//
		// Semaphores
		//
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		uint32_t currentFrame = 0;
	};
}