#pragma once
#include <SDL2/SDL_keycode.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

class Input 
{
public:
	Input();

	//Read input events, updating input state
	void Update();

	//Check if an SDLK is being pressed this frame
	bool GetKey(SDL_Keycode key);

	//Get the change in mouse position from last frame to this frame
	glm::vec2 GetMouseDelta();

	//Get the value of the mouse-wheel's scroll
	glm::vec2 GetMouseWheelScroll();

	//Check for left click
	bool Mouse1Down();

	//Check for right click
	bool Mouse2Down();

	//Check for mouse wheel click
	bool Mouse3Down();

	//Clear the previous mouse position, resetting it to the centre of the screen
	void ClearMousePrevious(int width, int height);

	bool quit;
	float mouseSensitivity;
private:
	glm::vec2 mousePrevious;
	glm::vec2 mousePos;
	glm::vec2 mouseDelta;
	glm::vec2 scroll;

	bool mouse1;
	bool mouse2;
	bool mouse3;
	
	//Unordered map is used as it has O(1) time complexity [std::map has worst case O(log n)]
	std::unordered_map<int, bool> keys;
};