#pragma once
#include "PS_Allocator.hpp"
#include "PS_ModelHandler.hpp"
#include "PS_MaterialHandler.hpp"
#include "PS_RenderPass.hpp"
#include "PS_Shader.hpp"
#include "PS_Descriptor.hpp"
#include "PS_Light.hpp"
#include "UI_Widget.hpp"
#include "PS_ColorHandler.hpp"

namespace ps {
	class PS_Pipeline : public PS_Allocator {
	public:
		PS_Pipeline(PS_Window *window, PS_Device *device, PS_SwapChain *chain, 
			std::vector<PS_GameObject*> objects, std::vector<PS_Light*> lights, 
			PS_GameCamera* camera, std::string vertexShader, std::string fragmentShader, bool clear);

		void drawFrame();
		void updateUniformBuffer(uint32_t currentImage);
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		uint32_t MAX_FRAMES_IN_FLIGHT = 2;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

	private:
		void createGraphicsPipeline();
		void createUniformBuffers();
		void createCommandBuffers();
		void createSyncObjects();
		void createViewport(VkCommandBuffer commandBuffer);
		void loadGameObjects();
		void loadLights();
		void renderGameObjects(VkCommandBuffer commandBuffer);

		int uniformLayoutIndex;
		int textureLayoutIndex;

		PS_Window* psWindow;
		PS_Device* psDevice;
		PS_SwapChain *psSwapChain;
		PS_RenderPass *psRenderPass;
		PS_DescriptorSet* psDescriptorSets;
		PS_GameCamera *gameCamera;
		PS_MaterialHandler *basicMaterial;
		UI_Widget* widget;

		std::vector<PS_Light*> pointLights;

		std::string vertexShaderPath;
		std::string fragmentShaderPath;

		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<PS_GameObject*> gameObjects;
		std::vector<PS_ModelHandler*> objectModels;
		std::vector<PS_ModelHandler*> lightModels;
		std::vector<PS_MaterialHandler*> materials;
		std::vector<std::unique_ptr<PS_BufferHandler>> uniformBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		uint32_t currentFrame = 0;
	};
}