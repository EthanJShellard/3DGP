#pragma once
#include "../Script.h"
#include "GL/glew.h"
#include <vector>
#include <memory>

class LoneQuad;
class Shader;
struct Light;

class ProjectileSpawner : public Script 
{
private:
	std::shared_ptr<Shader> projectileShader;
	GLuint projectileTexture;
	float timer;

	std::vector< std::shared_ptr<LoneQuad> > loneQuads;
	std::vector < std::shared_ptr<Light> > lights;

	std::vector < std::shared_ptr<LoneQuad> > activeProjectiles;
public:
	ProjectileSpawner(GLuint texture, std::shared_ptr<Shader> shader);
	void Update(float deltaTs, std::shared_ptr<Input> input) override;
	void Start() override;
};