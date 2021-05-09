#include "ProjectileSpawner.h"
#include "../LoneQuad.h"
#include "../Light.h"
#include "../Texture.h"

ProjectileSpawner::ProjectileSpawner(std::shared_ptr<Texture> _texture, std::shared_ptr<Shader> _shader)
{
	m_projectileShader = _shader;
	m_projectileTexture = _texture;
	m_timer = 0.0f;
}

void ProjectileSpawner::Update(float _deltaTs, std::shared_ptr<Input> _input)
{

	m_timer += _deltaTs;
	if (m_timer > m_period && m_loneQuads.size() != 0)
	{
		if (auto lckPtr = m_loneQuads.at(m_loneQuads.size() - 1).lock()) 
		{
			//Make projectile active
			m_activeProjectiles.push_back(lckPtr);
			//And remove it from inactive list
			m_loneQuads.pop_back();
			//Get projectile into position
			lckPtr->SetPosition(glm::vec3(std::rand() % 6 - 3.0f, 0.0f, -35.0f));
			//Adjust game speed and reset timer
			m_timer = 0.0f;
			if (m_speed < 30) m_speed *= 1.015f;
			m_period *= .99f;
			
		}
	}

	auto playerPtr = m_player.lock();

	for (int i = 0; i < m_activeProjectiles.size(); i++)
	{
		auto lightPtr = m_lights.at(i).lock();
		auto pro = m_activeProjectiles.at(i).lock();

		if (playerPtr && lightPtr && pro) //Verify weak pointers
		{
			//Move projectile
			pro->Translate(glm::vec3(0.0f, 0.0f, m_speed * _deltaTs));
			//Get distance of projectile from player in z axis
			float zDiff = pro->GetPosition().z - playerPtr->GetPosition().z;

			if (zDiff > 8) //If projectile has gone far enough behind the player
			{
				//Return to projectile bank
				pro->SetPosition(999.0f, 999.0f, 999.0f);
				m_loneQuads.push_back(pro);

				//Refit active projectiles
				for (int j = i; j < m_activeProjectiles.size() - 1; j++)
				{
					m_activeProjectiles.at(j) = m_activeProjectiles.at(j + 1);
				}
				m_activeProjectiles.erase(m_activeProjectiles.end() - 1);
			}
			else if (zDiff > 0 && zDiff < 1)
			{
				if (std::abs(pro->GetPosition().x - playerPtr->GetPosition().x) < (4.5f * playerPtr->GetScale().x)) //PLAYER HIT
				{
					playerPtr->SetScale(playerPtr->GetScale() * (1 - _deltaTs));
				}
			}
			//Make sure respective light is following projectile
			lightPtr->m_transform.m_position = pro->GetPosition();
		}
		
	}

}

void ProjectileSpawner::Start()
{
	auto scPtr = m_scene.lock();

	if (scPtr) 
	{
		//Create pool of 8 projectiles
		for (int i = 0; i < 8; i++)
		{
			std::shared_ptr<LoneQuad> qu = std::make_shared<LoneQuad>(m_projectileTexture, m_projectileShader);
			m_loneQuads.push_back(qu);
			qu->SetScale(0.25f, 0.25f, 0.25f);
			qu->Rotate(90, qu->m_transform.Right());
			qu->SetPosition(999.0f, 999.0f, 999.0f);
			scPtr->AddObject(qu);

			std::shared_ptr<Light> light = std::make_shared<Light>(glm::vec3(999.0f, 999.0f, 999.0f), glm::vec3(1.0f, 1.0f, 1.0f), 4.0f);
			light->m_ID = i;
			m_lights.push_back(light);
			scPtr->AddLight(light);
		}
		m_speed = 4.0f;
		m_period = 2.0f;
		m_player = scPtr->FindObjectByID(1);
	}
	

}
