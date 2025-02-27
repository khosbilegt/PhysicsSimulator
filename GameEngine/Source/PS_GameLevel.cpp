#include "PS_GameLevel.hpp"

namespace ps {
	PS_GameLevel::PS_GameLevel(std::string path) {
		loadMap(path);
	}

	void PS_GameLevel::loadMap(std::string path) {
		levelPath = path;

		std::ifstream file(path);
		json data = json::parse(file);
		levelName = data["name"];
		for (auto& elem : data["game_objects"]) {
			readCameras(elem);
			readLights(elem);
			readObjects(elem);
		}
	}

	glm::vec3 PS_GameLevel::read3DVector(json data, std::string field) {
		glm::vec3 vector;
		vector.x = data[field][0];
		vector.y = data[field][1];
		vector.z = data[field][2];
		return vector;
	}

	glm::vec4 PS_GameLevel::read4DVector(json data, std::string field) {
		glm::vec4 vector;
		vector.x = data[field][0];
		vector.y = data[field][1];
		vector.z = data[field][2];
		vector.a = data[field][3];
		return vector;
	}

	void PS_GameLevel::tick() {
		switchPipeline();
	}

	void PS_GameLevel::switchPipeline() {
		PS_KeyboardHandler::setNumberEnabled(true);
		if (PS_KeyboardHandler::isPressed('1')) {
			currentPipeline = UNLIT_PIPELINE;
		}
		else if (PS_KeyboardHandler::isPressed('2')) {
			currentPipeline = LIT_PIPELINE;
		}
	}

	void PS_GameLevel::readLights(json elem) {
		bool isDirectional;
		if (elem["type"] == "PointLight") {
			isDirectional = false;
		}
		else if (elem["type"] == "DirectionalLight") {
			isDirectional = true;
		}
		else {
			return;
		}
		PS_Light* gameLight = new PS_Light();
		gameLight->setName(elem["name"]);
		gameLight->setLightColor(read3DVector(elem, "color"));
		gameLight->setIntensity(elem["intensity"]);
		gameLight->setLocation(read3DVector(elem, "location"));
		if (isDirectional) {
			gameLight->setDirectional(true);
			gameLight->setRotation(read3DVector(elem, "rotation"));
		}
		else {
			gameLight->setDirectional(false);
		}
		gameLights.push_back(gameLight);

	}
	void PS_GameLevel::readCameras(json elem) {
		if (elem["type"] == "EditorCamera") {
			PS_GameCamera* camera = new PS_GameCamera();
			camera->setLocation(read3DVector(elem, "location"));
			camera->setRotation(read3DVector(elem, "rotation"));
			setCamera(camera);
		}
	}
	void PS_GameLevel::readObjects(json elem) {
		if (elem["type"] == "GameObject") {
			PS_GameObject* gameObject = new PS_GameObject();
			gameObject->setLocation(read3DVector(elem, "location"));
			gameObject->setRotation(read3DVector(elem, "rotation"));
			gameObject->setScale(read3DVector(elem, "scale"));
			gameObject->setModel(elem["model_path"]);
			gameObject->setName(elem["name"]);
			gameObject->setAlpha(elem["alpha"]);

			PS_Material material = readMaterial(elem["material"]);
			gameObject->setMaterial(material);
			gameObjects.push_back(gameObject);
		}
	}
	PS_Material PS_GameLevel::readMaterial(json elem) {
		PS_Material material;
		if (elem.contains("baseColor")) {
			MaterialComponent baseColor = readMaterialComponent(elem["baseColor"]);
			material.setColor(baseColor);
		}
		if (elem.contains("emissive")) {
			MaterialComponent emissive = readMaterialComponent(elem["emissive"]);
			material.setEmissive(emissive);
		}
		return material;
	}

	MaterialComponent PS_GameLevel::readMaterialComponent(json elem) {
		MaterialComponent component;
		if (elem.contains("path")) {
			component.isTexture = true;
			component.texturePath = elem["path"];
			return component;
		}
		component.isTexture = false;
		component.color = read4DVector(elem, "color");
		return component;
	}
}