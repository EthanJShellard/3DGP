#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "Input.h"

Input::Input()
{
	mouse3 = false;
	mouse1 = false;
	mouse2 = false;
	quit = false;
}

void Input::Update()
{
	SDL_Event event = SDL_Event{0};
	mouseDelta = glm::vec2(0, 0);
	scroll = glm::vec2(0, 0);

	//Poll inputs
	while (SDL_PollEvent(&event)) 
	{
		if (event.type == SDL_KEYDOWN) 
		{
			if (keys.find(event.key.keysym.sym) == keys.end()) //Key is not present in map
			{
				keys.insert( std::pair<int, bool>(event.key.keysym.sym, true));
			}
			else 
			{
				keys.find(event.key.keysym.sym)->second = true;
			}
		}
		else if (event.type == SDL_KEYUP)
		{
			keys.find(event.key.keysym.sym)->second = false;
		}
		else if (event.type == SDL_MOUSEMOTION) 
		{
			//Use += delta to avoid issues if mouse motion events come in twice
			mouseDelta.x += event.motion.x - mousePrevious.x;
			mouseDelta.y += event.motion.y - mousePrevious.y;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) 
		{
			switch (event.button.button) 
			{
			case SDL_BUTTON_LEFT:
				mouse1 = true;
				break;
			case SDL_BUTTON_RIGHT:
				mouse2 = true;
				break;
			case SDL_BUTTON_MIDDLE:
				mouse3 = true;
				break;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) 
		{
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				mouse1 = false;
				break;
			case SDL_BUTTON_RIGHT:
				mouse2 = false;
				break;
			case SDL_BUTTON_MIDDLE:
				mouse3 = false;
				break;
			}
		}
		else if (event.type == SDL_MOUSEWHEEL)
		{
			scroll.x = event.wheel.x;
			scroll.y = event.wheel.y;
		}
		else if (event.type == SDL_QUIT) 
		{
			quit = true;
		}
	}

	mousePrevious = mouseDelta;
}

bool Input::GetKey(SDL_Keycode key)
{
	std::map<int, bool>::iterator itr = keys.find(key);
	if (itr == keys.end()) return false;
	else return itr->second;

}

glm::vec2 Input::GetMouseDelta()
{
	return mouseDelta;
}

glm::vec2 Input::GetMouseWheelScroll()
{
	return scroll;
}

bool Input::Mouse1Down()
{
	return mouse1;
}

bool Input::Mouse2Down()
{
	return mouse2;
}

bool Input::Mouse3Down()
{
	return mouse3;
}
