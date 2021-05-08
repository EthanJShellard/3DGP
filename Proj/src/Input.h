#pragma once
#include <SDL2/SDL_keycode.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>

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