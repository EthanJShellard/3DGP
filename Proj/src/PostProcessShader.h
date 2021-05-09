#pragma once
#include "Shader.h"
#include <memory>

class RenderTexture;

//Wrapper for shader class which takes into account multitexturing used in postprocessing
class PostProcessShader : public Shader
{
private:
	GLint m_texture1Loc;
	GLint m_texture2Loc;

public:
	void BindTextures(std::shared_ptr<RenderTexture> _tex1, std::shared_ptr<RenderTexture> _tex2);
	void BindTexture(std::shared_ptr<RenderTexture> _tex1);

	PostProcessShader(const char* _vertexPath, const char* _fragmentPath);
	GLuint m_projectionLoc;
};