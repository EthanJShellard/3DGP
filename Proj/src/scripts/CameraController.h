#pragma once
#include "../Script.h"

/// <summary>
/// Facilities flying camera movement. Simply add to scene.
/// </summary>
class CameraController : public Script 
{
public:
	void Update(float deltaTs, std::shared_ptr<Input> input) override;
};