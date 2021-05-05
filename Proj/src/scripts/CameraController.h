#pragma once
#include "../Script.h"

class CameraController : public Script 
{
public:
	void Update(float deltaTs, std::shared_ptr<Input> input) override;
};