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
	bool GetKey(SDL_Keycode _key);

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
	void ClearMousePrevious(int _width, int _height);

	bool m_quit;
	float m_mouseSensitivity;
private:
	glm::vec2 m_mousePrevious;
	glm::vec2 m_mousePos;
	glm::vec2 m_mouseDelta;
	glm::vec2 m_scroll;

	bool m_mouse1;
	bool m_mouse2;
	bool m_mouse3;
	
	//Unordered map is used as it has O(1) time complexity [std::map has worst case O(log n)]
	std::unordered_map<int, bool> m_keys;
};