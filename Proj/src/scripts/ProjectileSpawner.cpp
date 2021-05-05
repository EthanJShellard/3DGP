#include "ProjectileSpawner.h"
#include "../LoneQuad.h"
#include "../Light.h"

ProjectileSpawner::ProjectileSpawner(GLuint texture, std::shared_ptr<Shader> shader)
{
	projectileShader = shader;
	projectileTexture = texture;
	timer = 0.0f;
}

void ProjectileSpawner::Update(float deltaTs, std::shared_ptr<Input> input)
{
	timer += deltaTs;
	if (timer > 2.0f && loneQuads.size() != 0) 
	{
		std::shared_ptr<LoneQuad> pro = loneQuads.at(loneQuads.size() - 1);
		activeProjectiles.push_back(pro);
		loneQuads.pop_back();

		pro->SetPosition(glm::vec3(std::rand() % 6 - 3.0f, 0.0f , -35.0f));
		timer = 0.0f;
	}

	for (int i = 0; i < activeProjectiles.size(); i++) 
	{
		std::shared_ptr<LoneQuad> pro = activeProjectiles.at(i);
		pro->Translate(glm::vec3(0.0f, 0.0f, 4.0f * deltaTs));
		lights.at(i)->transform->position = pro->GetPosition();
	}

}

void ProjectileSpawner::Start()
{
	//Create pool of 8 projectiles
	for (int i = 0; i < 8; i++)
	{
		std::shared_ptr<LoneQuad> qu = std::make_shared<LoneQuad>(projectileTexture, projectileShader);
 		loneQuads.push_back(qu);
		qu->SetScale(0.25f, 0.25f, 0.25f);
		qu->Rotate(90, qu->transform.Right());
		scene->AddObject(loneQuads.at(i));
		lights.push_back(std::make_shared<Light>(glm::vec3(999.0f, 999.0f, 999.0f), glm::vec3(1.0f, 0.0f, 1.0f), 5.0f));
		scene->AddLight(lights.at(i));
		lights.at(i)->ID = i;
	}

}
