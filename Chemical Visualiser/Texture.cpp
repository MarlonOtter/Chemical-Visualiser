#include "Texture.h"


Texture::Texture(const char* image, const char* texType, GLenum slot)
{
	//sets the type of texture (1D, 2D, 3D, etc)
	type = texType;

	//creates variables for the image width, image height, and number of colour channels
	int widthImg, heightImg, numColCh;
	//flip the image so that it is the correct way up
	stbi_set_flip_vertically_on_load(true);

	//load the image
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	//create the texture variables
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	Bind();

	//set parameters (filter type)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//set the method for image wraping (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// If you use GL_CLAMP_TO_BORDER
	//float flatColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColour);.

	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else throw std::invalid_argument("Automatic Texture Type recognition failed"); //something wrong here

	// create the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//delete the data of "bytes"
	stbi_image_free(bytes);
	//unbind the texture
	Unbind();
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
	//get the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	//activate the shaderPrgram
	shader.Activate();
	//set the uniform value
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	//bind the texture
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	//unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	//delete the texture
	glDeleteTextures(1, &ID);
}