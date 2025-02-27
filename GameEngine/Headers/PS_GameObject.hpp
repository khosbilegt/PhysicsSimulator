#pragma once
#include "PS_GameGeneric.hpp"
#include "PS_GameMesh.hpp"
#include "PS_FileHandler.hpp"
#include "PS_Material.hpp"

namespace ps {
	class PS_GameObject : protected PS_GameMesh {
	public:
		PS_GameObject();
		PS_GameObject(GeometryObject3D shape);
		PS_GameObject(glm::vec3 newLocation, glm::vec3 newRotation, glm::vec3 newScale);
		PS_GameObject(glm::vec3 newLocation, glm::vec3 newRotation, glm::vec3 newScale, GeometryObject3D shape);

		void setLocation(glm::vec3 newLocation);
		void setRotation(glm::vec3 newRotation);
		void setMeshRotation(glm::vec3 newRotation);
		void setScale(glm::vec3 newScale);
		void setModel(std::string newModelPath);
		void setMaterial(PS_Material newMaterial);
		void setName(std::string newName);
		void setAlpha(float newAlpha);
		void setGeometryObject(GeometryObject3D shape);
		void tick();

		glm::vec3 getLocation();
		glm::vec3 getRotation();
		glm::vec3 getMeshRotation();
		glm::vec3 getScale();
		glm::vec3 getForwardVector();
		glm::vec3 getRightVector();
		glm::vec3 getUpVector();
		std::string getModel();
		PS_Material getMaterial();
		std::string getName();
		GeometryObject3D getGeometryObject();
		//std::vector<Vertex> getVertices();
		bool getIsBasicShape();
		float getAlpha();

	private:
		glm::vec3 location;
		glm::vec3 rotation;
		glm::vec3 meshRotation;
		glm::vec3 scale;

		glm::vec3 forwardVector;
		glm::vec3 rightVector;
		glm::vec3 upVector;

		std::string modelPath;
		std::string objectName;
		PS_Material material;

		float alpha;
		bool isBasicShape;
	};
}