#include "SceneLoader.h"
#include "Scene.h"
#include "Shader.h"
#include "OBJModel.h"
#include "GameObjectOBJ.h"
#include "LoneQuad.h"
#include "scripted_objects/SpinningLight.h"
#include "scripts/SpaceshipController.h"

std::shared_ptr<Scene> SceneLoader::LoadShmupScene(std::shared_ptr<Input> input)
{
	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/main/vert.txt", "assets/shaders/main/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");


	std::shared_ptr<OBJModel> spaceShip = std::make_shared<OBJModel>("assets/models/Spaceship/Intergalactic_Spaceship-(Wavefront).obj", program);
	std::shared_ptr<GameObjectOBJ> spaceShipObject = std::make_shared<GameObjectOBJ>();
	spaceShipObject->ID = 1;
	spaceShipObject->SetModel(spaceShip);
	spaceShipObject->SetScale(glm::vec3(0.3,0.3,0.3));
	spaceShipObject->SetPosition(glm::vec3(-0.1,0,-5));
	spaceShipObject->Rotate(180.0f, spaceShipObject->transform.Up());

	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(input);
	mainScene->AddObject(spaceShipObject);
	mainScene->AddScript(std::make_shared<SpaceshipConrtoller>());

	mainScene->mainCamera.transform.SetPosition(glm::vec3(0, 2, 1));
	mainScene->mainCamera.transform.Rotate(-10.0f, mainScene->mainCamera.transform.Right());
	//mainScene->AddLight(std::make_shared<Light>(glm::vec3(-5, 0, 0), glm::vec3(1, 1, 1), 1.0f));


	return mainScene;
}

std::shared_ptr<Scene> SceneLoader::LoadDust2Scene(std::shared_ptr<Input> input)
{
	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/main/vert.txt", "assets/shaders/main/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");

	std::shared_ptr<OBJModel> dust2 = std::make_shared<OBJModel>("assets/models/dust 2/triangulated.obj", program);
	std::shared_ptr<GameObjectOBJ> dust2Obj = std::make_shared<GameObjectOBJ>();
	dust2Obj->SetModel(dust2);
	dust2Obj->Rotate(-90.0f, glm::vec3(1, 0, 0));
	dust2Obj->SetPosition(10.0f, 1.0f, 1.0f);
	dust2Obj->SetScale(0.1f, 0.1f, 0.1f);


	std::shared_ptr<LoneQuad> floorQuad = std::make_shared<LoneQuad>("assets/textures/Potato.jpg", program);
	floorQuad->SetScale(50.0f, 1.0f, 50.0f);
	floorQuad->SetPosition(-25.0f, 0.0f, -25.0f);

	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(input);
	mainScene->AddObject(dust2Obj);
	mainScene->AddObject(floorQuad);
	mainScene->AddObject(std::make_shared<SpinningLight>());
	mainScene->AddLight(std::make_shared<Light>(glm::vec3(10, 3, 0), glm::vec3(1, 1, 1), .45f));

	mainScene->mainCamera.transform.SetPosition(glm::vec3(0, 10, 0));

	return mainScene;
}

std::shared_ptr<Scene> SceneLoader::LoadScene(int index, std::shared_ptr<Input> input)
{
	switch (index) 
	{
	case 0:
		return LoadDust2Scene(input);
		break;
	case 1:
		return LoadShmupScene(input);
		break;
	default:
		return LoadDust2Scene(input);
		break;
	}
}
