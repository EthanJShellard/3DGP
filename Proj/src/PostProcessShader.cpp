#include "PostProcessShader.h"
#include "RenderTexture.h"

void PostProcessShader::BindTextures(std::shared_ptr<RenderTexture> tex1, std::shared_ptr<RenderTexture> tex2)
{
	//Bind first texture in texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1->GetTextureID());
	//Bind second texture in texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2->GetTextureID());
	//Bind texture unit 0
	glActiveTexture(GL_TEXTURE0);
}

void PostProcessShader::BindTexture(std::shared_ptr<RenderTexture> tex1)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1->GetTextureID());
}

PostProcessShader::PostProcessShader(const char* vertexPath, const char* fragmentPath)
{
	//Initialise variables
	dirty = true;
	vertID = 0;
	fragID = 0;

	LoadNewVertexShader(vertexPath);
	LoadNewFragmentShader(fragmentPath);

	id = glCreateProgram();

	projectionLoc = glGetUniformLocation(this->GetID(), "u_Projection");
	BindAttribute(0, "a_Position");
	BindAttribute(1, "a_TexCoords");

	//Ensure compilation and linkage before uniform search
	glUseProgram(GetID());

	texture1Loc = glGetUniformLocation(id, "u_TextureA");
	texture2Loc = glGetUniformLocation(id, "u_TextureB");

	//Make sure to set samplers to 0 and 1 respectively
	glUniform1i(texture1Loc, 0);
	glUniform1i(texture2Loc, 1);
}
