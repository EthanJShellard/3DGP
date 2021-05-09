#include "PostProcessShader.h"
#include "RenderTexture.h"

void PostProcessShader::BindTextures(std::shared_ptr<RenderTexture> _tex1, std::shared_ptr<RenderTexture> _tex2)
{
	//Bind first texture in texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex1->GetTextureID());
	//Bind second texture in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _tex2->GetTextureID());
	//Bind texture unit 0
	glActiveTexture(GL_TEXTURE0);
}

void PostProcessShader::BindTexture(std::shared_ptr<RenderTexture> _tex1)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex1->GetTextureID());
}

PostProcessShader::PostProcessShader(const char* _vertexPath, const char* _fragmentPath)
{
	//Initialise variables
	m_dirty = true;
	m_vertID = 0;
	m_fragID = 0;

	LoadNewVertexShader(_vertexPath);
	LoadNewFragmentShader(_fragmentPath);

	m_id = glCreateProgram();

	m_projectionLoc = glGetUniformLocation(this->GetID(), "u_Projection");
	BindAttribute(0, "a_Position");
	BindAttribute(1, "a_TexCoords");

	//Ensure compilation and linkage before uniform search
	glUseProgram(GetID());

	m_texture1Loc = glGetUniformLocation(m_id, "u_TextureA");
	m_texture2Loc = glGetUniformLocation(m_id, "u_TextureB");

	//Make sure to set samplers to 0 and 1 respectively
	glUniform1i(m_texture1Loc, 0);
	glUniform1i(m_texture2Loc, 1);
}
