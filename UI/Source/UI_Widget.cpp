#include "UI_Widget.hpp"

namespace ps {
	UI_Widget::UI_Widget(PS_Device* device, PS_GameCamera *camera, PS_Window *window) : PS_Allocator(device) {
		psDevice = device;
		gameCamera = camera;
		psWindow = window;
		this->vertices = vertices;
		this->indices = indices;
		createWidget();
		createVertexBuffer();
		createIndexBuffer();
	}

	void UI_Widget::createWidget() {
		GeometryObject2D object;
		object.x = 0;
		object.y = 0;
		object.height = 100;
		object.width = 100;
		object.color = {PS_ColorHandler::makeColor("d8e2dc"), 1};
		renderComponent = object;
	

		Vertex topLeft, topRight, bottomLeft, bottomRight;
		
		topLeft.color = object.color;
		topRight.color = object.color;
		bottomLeft.color = object.color;
		bottomRight.color = object.color;

		vertices.push_back(bottomLeft);
		vertices.push_back(bottomRight);
		vertices.push_back(topRight);
		vertices.push_back(topLeft);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(0);
	}

	void UI_Widget::createVertexBuffer() {
		uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		uint32_t vertexSize = sizeof(vertices[0]);
		
		PS_BufferHandler stagingBuffer{
			psDevice,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertices.data());

		vertexBuffer = std::make_unique<PS_BufferHandler>(
			psDevice,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		copyBuffer(stagingBuffer.getBuffer(), vertexBuffer->getBuffer(), bufferSize);
	}
	void UI_Widget::createIndexBuffer() {
		uint32_t indexCount = static_cast<uint32_t>(indices.size());
		bool hasIndexBuffer = indexCount > 0;
		if (!hasIndexBuffer) {
			return;
		}
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
		uint32_t indexSize = sizeof(indices[0]);

		PS_BufferHandler stagingBuffer{
			psDevice,
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)indices.data());

		indexBuffer = std::make_unique<PS_BufferHandler>(
			psDevice,
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
			);

		copyBuffer(stagingBuffer.getBuffer(), indexBuffer->getBuffer(), bufferSize);
	}

	void UI_Widget::renderWidget(VkCommandBuffer commandBuffer) {
		translateGeometry();
		createVertexBuffer();
		createIndexBuffer();
		VkBuffer vertexBuffers[] = { vertexBuffer->getBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	}

	void UI_Widget::translateGeometry() {
		windowCenter = gameCamera->getEye() + 10.0f * gameCamera->getForwardVector();
		normal = gameCamera->getForwardVector();
		yWindow = gameCamera->getUpVector();
		xWindow = normal;
		xWindow = (-1.0f) * (glm::cross(yWindow, xWindow));
		zWindow = normal;

		float len =  glm::length(10.0f * normal);
		height = len / glm::sqrt(3);
		width = height * psWindow->getSize()[0]/ psWindow->getSize()[1];

		vertices[0].pos = { translateCoord({10, 10}) };
		vertices[1].pos = { translateCoord({100, 10})};
		vertices[2].pos = { translateCoord({100, 50})};
		vertices[3].pos = { translateCoord({10, 50})};

		/*vertices[0].pos = { windowCenter + ((-width) * xWindow + height * yWindow) };
		vertices[1].pos = { windowCenter + (width * xWindow + height * yWindow) };
		vertices[2].pos = { windowCenter + (width * xWindow - height * yWindow) };
		vertices[3].pos = { windowCenter + ((-width) * xWindow - height * yWindow) };*/

		onClick();
	}

	void UI_Widget::onClick() {
		if (PS_MouseHandler::isPressed(GLFW_MOUSE_BUTTON_LEFT) == true) {
			glm::vec2 pos(PS_MouseHandler::getPosition());
			float h = glm::length(normal) / glm::sqrt(3);
			float w = h * psWindow->getSize()[0] / psWindow->getSize()[1];
			glm::vec3 center = gameCamera->getCenter();

			pos.x = (-1) * (width * (psWindow->getSize()[0] / 2 - pos.x) / (psWindow->getSize()[0] / 2));
			pos.y = height * (psWindow->getSize()[1] / 2 - pos.y) / (psWindow->getSize()[1] / 2);

			glm::vec3 mouseDirection(pos.x * xWindow + pos.y * yWindow);
			//mouseDirection = glm::normalize(mouseDirection);
			mouseDirection = normal + mouseDirection;
			//printVector("Mouse direction vec", mouseDirection);

			for (float i = 1.0f; i <= 100.0f; i += 0.01f) {
				mouseDirection *= i;
			}

		}
	}

	glm::vec3 UI_Widget::translateCoord(glm::vec2 pos) {
		pos.x = (-1) * (width * (psWindow->getSize()[0] / 2 - pos.x) / (psWindow->getSize()[0] / 2));
		pos.y = height * (psWindow->getSize()[1] / 2 - pos.y) / (psWindow->getSize()[1] / 2);
		                       
		glm::vec3 viewPortPos(pos, windowCenter[2]);
		viewPortPos = windowCenter + (pos.x * xWindow + pos.y * yWindow);
		//printVector("viewport position", viewPortPos);
		return viewPortPos;
	}

	void UI_Widget::printVector(std::string label, glm::vec3 pos) {
		std::cout << label << ": " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}

}