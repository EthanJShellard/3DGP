#include "SceneLoader.h"
#include "Scene.h"
#include "Shader.h"
#include "OBJModel.h"
#include "Texture.h"
#include "GameObjectOBJ.h"
#include "LoneQuad.h"
#include "scripts/SpaceshipController.h"
#include "scripts/ProjectileSpawner.h"
#include "scripts/CameraController.h"
#include "scripts/PulsingLight.h"
#include "scripts/Spinner.h"

std::shared_ptr<Scene> SceneLoader::LoadShmupScene(std::shared_ptr<Input> _input)
{
	//Create Shader programs
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/main/vert.txt", "assets/shaders/main/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");
	std::shared_ptr<Shader> fullbrightShader = std::make_shared<Shader>("assets/shaders/FullbrightVert.txt", "assets/shaders/FullbrightFrag.txt");
	////////////////////////

	//Create SpaceShip game object
	std::shared_ptr<OBJModel> spaceShip = std::make_shared<OBJModel>("assets/models/Spaceship/Intergalactic_Spaceship-(Wavefront).obj", program);
	std::shared_ptr<GameObjectOBJ> spaceShipObject = std::make_shared<GameObjectOBJ>();
	spaceShipObject->m_ID = 1;
	spaceShipObject->SetModel(spaceShip);
	spaceShipObject->SetScale(glm::vec3(0.3,0.3,0.3));
	spaceShipObject->SetPosition(glm::vec3(-0.1,0,-5));
	spaceShipObject->Rotate(180.0f, spaceShipObject->m_transform.Up());
	//////////////////////////////

	std::shared_ptr<Texture> texture = std::make_shared<Texture>("assets/textures/corcle.png");

	//Set up Scene
	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(_input);
	mainScene->AddObject(spaceShipObject);
	mainScene->AddScript(std::make_shared<SpaceshipConrtoller>());
	mainScene->AddScript(std::make_shared<ProjectileSpawner>(texture, fullbrightShader));
	mainScene->m_mainCamera.m_transform.SetPosition(glm::vec3(0, 2, 1));
	mainScene->m_mainCamera.m_transform.Rotate(-10.0f, mainScene->m_mainCamera.m_transform.Right());


	return mainScene;
}

std::shared_ptr<Scene> SceneLoader::LoadDust2Scene(std::shared_ptr<Input> _input)
{
	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/main/vert.txt", "assets/shaders/main/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");
	///////////////////////

	//Create Dust 2 game object
	std::shared_ptr<OBJModel> dust2 = std::make_shared<OBJModel>("assets/models/dust 2/triangulated.obj", program);
	std::shared_ptr<GameObjectOBJ> dust2Obj = std::make_shared<GameObjectOBJ>();
	dust2Obj->SetModel(dust2);
	dust2Obj->Rotate(-90.0f, glm::vec3(1, 0, 0));
	dust2Obj->SetPosition(0.0f, 1.0f, -2.0f);
	dust2Obj->SetScale(0.3f, 0.3f, 0.3f);
	///////////////////////////

	//Set up scene
	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(_input);
	mainScene->AddObject(dust2Obj);
	mainScene->AddLight(std::make_shared<Light>(glm::vec3(0, 20, -2.0f), glm::vec3(1, 1, 1), -.6f));
	mainScene->AddScript(std::make_shared<CameraController>());
	mainScene->m_mainCamera.m_transform.Rotate(-45.0f, mainScene->m_mainCamera.m_transform.Right());
	mainScene->m_mainCamera.m_transform.SetPosition(glm::vec3(0, 10, 0));
	mainScene->SetAmbientBrightness(0.1f);

	return mainScene;
}

std::shared_ptr<Scene> SceneLoader::LoadBloomDemoScene(std::shared_ptr<Input> _input)
{
	//Create Shader programs
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/main/vert.txt", "assets/shaders/main/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");
	std::shared_ptr<Shader> fullbrightShader = std::make_shared<Shader>("assets/shaders/FullbrightVert.txt", "assets/shaders/FullbrightFrag.txt");
	////////////////////////

	//Create game objects
	std::shared_ptr<LoneQuad> navigationPanel = std::make_shared<LoneQuad>("assets/textures/Navigation.png", fullbrightShader);
	navigationPanel->SetPosition(-0.7, 1, -5.2);
	navigationPanel->Rotate(90.0f, glm::vec3(1, 0, 0));
	navigationPanel->SetScale(glm::vec3(2, 1, 1));

	std::shared_ptr<OBJModel> demo = std::make_shared<OBJModel>("assets/models/Demo/untitled.obj", program);
	std::shared_ptr<GameObjectOBJ> demoStage = std::make_shared<GameObjectOBJ>();
	demoStage->Translate(glm::vec3(0, -2, -5));
	demoStage->SetModel(demo);

	std::shared_ptr<OBJModel> spinnerCubeModel = std::make_shared<OBJModel>("assets/models/Demo/spinner_cube.obj", program);
	std::shared_ptr<GameObjectOBJ> spinnerCube = std::make_shared<GameObjectOBJ>();
	spinnerCube->SetModel(spinnerCubeModel);
	spinnerCube->Translate(glm::vec3(0,-0.25,-4));
	spinnerCube->SetScale(0.1f,0.2f,0.1f);
	spinnerCube->Rotate(45.0f, glm::vec3(0,0,-1));
	spinnerCube->m_model->m_meshes.at(0)->m_material->m_emissiveColour = glm::vec3(0,1,0);
	spinnerCube->m_model->m_meshes.at(0)->m_material->m_dissolve = 0.6f;
	spinnerCube->m_ID = 1;
	////////////////////////

	//Set up scene
	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(_input);
	mainScene->AddObject(demoStage);
	mainScene->AddObject(navigationPanel);
	mainScene->AddObject(spinnerCube);
	mainScene->AddLight(std::make_shared<Light>(glm::vec3(0, 20, 5), glm::vec3(1, 1, 1), -.5f));
	mainScene->AddScript(std::make_shared<CameraController>());
	mainScene->AddScript(std::make_shared<Spinner>(90, glm::vec3(spinnerCube->m_transform.Right()), 1));
	mainScene->AddScript(std::make_shared<Spinner>(-45, glm::vec3(spinnerCube->m_transform.Up()), 1));

	return mainScene;
}

std::shared_ptr<Scene> SceneLoader::LoadPointLightingDemo(std::shared_ptr<Input> _input)
{
	//Create Shader program
	std::shared_ptr<Shader> program = std::make_shared<Shader>("assets/shaders/main/vert.txt", "assets/shaders/main/frag.txt");
	program->BindAttribute(0, "a_Position");
	program->BindAttribute(1, "a_TexCoord");
	program->BindAttribute(2, "a_Normal");
	///////////////////////

	//Create game objects
	std::shared_ptr<LoneQuad> potatoFloor = std::make_shared<LoneQuad>("assets/textures/Potato.jpg", program);
	potatoFloor->SetScale(glm::vec3(20,20,20));
	potatoFloor->SetPosition(glm::vec3(-10, 0.5f,-10));

	std::shared_ptr<OBJModel> curuthersModel = std::make_shared<OBJModel>("assets/models/curuthers/triangulated.obj", program);
	std::shared_ptr<GameObjectOBJ> curuthers = std::make_shared<GameObjectOBJ>();
	std::shared_ptr<GameObjectOBJ> giantCuruthers = std::make_shared<GameObjectOBJ>();
	curuthers->SetModel(curuthersModel);
	giantCuruthers->SetModel(curuthersModel);
	curuthers->SetPosition(glm::vec3(1,1,-5));
	giantCuruthers->SetPosition(glm::vec3(-1,3,-20));
	curuthers->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));

	std::shared_ptr<OBJModel> streetLamp = std::make_shared<OBJModel>("assets/models/StreetLamp/Triangulated.obj", program);
	std::shared_ptr<GameObjectOBJ> lamp1 = std::make_shared<GameObjectOBJ>();
	std::shared_ptr<GameObjectOBJ> lamp2 = std::make_shared<GameObjectOBJ>();
	lamp1->SetModel(streetLamp);
	lamp2->SetModel(streetLamp);
	lamp1->SetPosition(glm::vec3(2.5,0.5f, -5));
	lamp2->SetPosition(glm::vec3(-2.5, 0.5f, -5));
	lamp1->SetScale(glm::vec3(0.2f,0.2f,0.2f));
	lamp2->SetScale(glm::vec3(0.2f,0.2f,0.2f));
	/////////////////////

	//Create lights
	std::shared_ptr<Light> light1 = std::make_shared<Light>(lamp1->GetPosition() + glm::vec3(0, 1.25f,0), glm::vec3(1,0.6,0), 1.0f);
	std::shared_ptr<Light> light2 = std::make_shared<Light>(lamp2->GetPosition() + glm::vec3(0, 1.25f, 0), glm::vec3(1, 0.6, 0), 1.0f);

	//Set up scene
	std::shared_ptr<Scene> mainScene = std::make_shared<Scene>(_input);
	mainScene->AddScript(std::make_shared<CameraController>());
	mainScene->AddScript(std::make_shared<PulsingLight>(.0f, 10.0f, 2.0f, glm::vec3(4,2,-20), glm::vec3(0.8,0,0)));
	mainScene->AddObject(lamp1);
	mainScene->AddObject(lamp2);
	mainScene->AddLight(light1);
	mainScene->AddLight(light2);
	mainScene->AddObject(potatoFloor);
	mainScene->AddObject(curuthers);
	mainScene->AddObject(giantCuruthers);
	mainScene->SetAmbientBrightness(0.0f);
	mainScene->m_mainCamera.m_transform.Translate(glm::vec3(0,2,0));

	return mainScene;
}

std::shared_ptr<Scene> SceneLoader::LoadScene(int _index, std::shared_ptr<Input> _input)
{
	switch (_index) 
	{
	case 0:
		return LoadBloomDemoScene(_input);
		break;
	case 1:
		return LoadDust2Scene(_input);
		break;
	case 2:
		return LoadShmupScene(_input);
		break;
	case 3:
		return LoadPointLightingDemo(_input);
		break;
	default:
		return LoadDust2Scene(_input);
		break;
	}
}
