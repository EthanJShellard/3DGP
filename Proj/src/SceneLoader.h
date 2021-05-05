#pragma once
#include <memory>

class Scene;
class Input;

class SceneLoader
{
private:
	static std::shared_ptr<Scene> LoadShmupScene(std::shared_ptr<Input> input);
	static std::shared_ptr<Scene> LoadDust2Scene(std::shared_ptr<Input> input);

public:
	static std::shared_ptr<Scene> LoadScene(int index, std::shared_ptr<Input> input);

};