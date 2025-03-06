#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <string>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <iostream>

//Graphics libraries
#include <glad/glad.h>

//returns a string containing all the text in a file
std::string get_file_contents(const char* filename);

class Shader
{
	public:
		//unsigned int
		//this is the id of the shader that is used by openGL to tell the GPU which code to run
		GLuint ID = 0;

		// default constructor
		Shader();
		// deconstructor
		~Shader();

		//constructor that will take in 2 character arrays, one for the vertex shader and one for the fragment shader 
		Shader(const char* vertFile, const char* fragFile);

		//activates the shader
		void Activate();
		//deletes the shader when the program is closed
		void Delete();
};

#endif // !SHADER_CLASS_H
