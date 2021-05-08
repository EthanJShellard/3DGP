#pragma once
#include "../Script.h"
#include "GL/glew.h"
#include <vector>
#include <memory>

class LoneQuad;
class GameObject;
class Shader;
struct Light;
struct Texture;

class ProjectileSpawner : public Script 
{
private:
	std::shared_ptr<Shader> projectileShader;
	std::shared_ptr<Texture> projectileTexture;
	float timer;
	float period;
	float speed;

	std::vector< std::weak_ptr<LoneQuad> > loneQuads;
	std::vector < std::weak_ptr<Light> > lights;
	std::weak_ptr<GameObject> player;

	std::vector < std::weak_ptr<LoneQuad> > activeProjectiles;
public:
	ProjectileSpawner(std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader);
	void Update(float deltaTs, std::shared_ptr<Input> input) override;
	void Start() override;
};