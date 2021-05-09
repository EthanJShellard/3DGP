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
	std::shared_ptr<Shader> m_projectileShader;
	std::shared_ptr<Texture> m_projectileTexture;
	float m_timer;
	float m_period;
	float m_speed;

	std::vector< std::weak_ptr<LoneQuad> > m_loneQuads;
	std::vector < std::weak_ptr<Light> > m_lights;
	std::weak_ptr<GameObject> m_player;

	std::vector < std::weak_ptr<LoneQuad> > m_activeProjectiles;
public:
	ProjectileSpawner(std::shared_ptr<Texture> m_texture, std::shared_ptr<Shader> m_shader);
	void Update(float m_deltaTs, std::shared_ptr<Input> m_input) override;
	void Start() override;
};