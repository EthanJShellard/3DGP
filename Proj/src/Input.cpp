#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Input.h"
#include <iostream>

Input::Input()
{
	m_mouse3 = false;
	m_mouse1 = false;
	m_mouse2 = false;
	m_quit = false;
	m_mouseSensitivity = 20.0f;
}

void Input::Update()
{
	SDL_Event event = SDL_Event{0};
	m_mouseDelta = glm::vec2(0, 0);
	m_scroll = glm::vec2(0, 0);

	//Poll inputs
	while (SDL_PollEvent(&event)) 
	{
		if (event.type == SDL_KEYDOWN) 
		{
			std::unordered_map<int, bool>::iterator entry = m_keys.find(event.key.keysym.sym);
			if (entry == m_keys.end()) //Key is not present in map
			{
				m_keys.insert( std::pair<int, bool>(event.key.keysym.sym, true)); //So put it in there
			}
			else 
			{
				entry->second = true;
			}

		}
		else if (event.type == SDL_KEYUP)
		{
			m_keys.find(event.key.keysym.sym)->second = false;
		}
		else if (event.type == SDL_MOUSEMOTION) 
		{
			//Use += delta to avoid issues if mouse motion events come in twice
			m_mouseDelta.x += event.motion.x - m_mousePrevious.x;
			m_mouseDelta.y += event.motion.y - m_mousePrevious.y;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) 
		{
			switch (event.button.button) 
			{
			case SDL_BUTTON_LEFT:
				m_mouse1 = true;
				break;
			case SDL_BUTTON_RIGHT:
				m_mouse2 = true;
				break;
			case SDL_BUTTON_MIDDLE:
				m_mouse3 = true;
				break;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) 
		{
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				m_mouse1 = false;
				break;
			case SDL_BUTTON_RIGHT:
				m_mouse2 = false;
				break;
			case SDL_BUTTON_MIDDLE:
				m_mouse3 = false;
				break;
			}
		}
		else if (event.type == SDL_MOUSEWHEEL)
		{
			m_scroll.x = event.wheel.x;
			m_scroll.y = event.wheel.y;
		}
		else if (event.type == SDL_QUIT) 
		{
			m_quit = true;
		}
	}

	m_mousePrevious += m_mouseDelta;
}

bool Input::GetKey(SDL_Keycode _key)
{
	std::unordered_map<int, bool>::iterator itr = m_keys.find(_key);
	if (itr == m_keys.end()) return false;
	else return itr->second;

}


glm::vec2 Input::GetMouseDelta()
{
	return m_mouseDelta;
}

glm::vec2 Input::GetMouseWheelScroll()
{
	return m_scroll;
}

bool Input::Mouse1Down()
{
	return m_mouse1;
}

bool Input::Mouse2Down()
{
	return m_mouse2;
}

bool Input::Mouse3Down()
{
	return m_mouse3;
}

void Input::ClearMousePrevious(int _width, int _height)
{
	m_mousePrevious = glm::vec2(_width / 2, _height / 2);
}
