#pragma once
#include <memory>

class Scene;
class Input;

//Class used in place of loading scenes from files - contains only static functions which return a shared pointer to a scene.
class SceneLoader
{
private:
	static std::shared_ptr<Scene> LoadShmupScene(std::shared_ptr<Input> _input);
	static std::shared_ptr<Scene> LoadDust2Scene(std::shared_ptr<Input> _input);
	static std::shared_ptr<Scene> LoadBloomDemoScene(std::shared_ptr<Input> _input);
	static std::shared_ptr<Scene> LoadPointLightingDemo(std::shared_ptr<Input> _input);
public:
	static std::shared_ptr<Scene> LoadScene(int index, std::shared_ptr<Input> _input);

};