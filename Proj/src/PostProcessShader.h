#pragma once
#include "Shader.h"
#include <memory>

class RenderTexture;

class PostProcessShader : public Shader
{
private:
	GLint texture1Loc;
	GLint texture2Loc;

public:
	void BindTextures(std::shared_ptr<RenderTexture> tex1, std::shared_ptr<RenderTexture> tex2);
	void BindTexture(std::shared_ptr<RenderTexture> tex1);

	PostProcessShader(const char* vertexPath, const char* fragmentPath);
	GLuint projectionLoc;
};