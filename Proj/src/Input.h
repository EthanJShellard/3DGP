#pragma once
#include <SDL2/SDL_keycode.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

class Input 
{
public:
	Input();

	void Update();

	bool GetKey(SDL_Keycode key);
	glm::vec2 GetMouseDelta();
	glm::vec2 GetMouseWheelScroll();
	bool Mouse1Down();
	bool Mouse2Down();
	bool Mouse3Down();

	bool quit;

private:
	glm::vec2 mousePrevious;
	glm::vec2 mouseDelta;
	glm::vec2 scroll;

	bool mouse1;
	bool mouse2;
	bool mouse3;
	
	std::map<int, bool> keys;
};