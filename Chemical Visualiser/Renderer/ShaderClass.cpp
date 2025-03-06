#include "ShaderClass.h"

//import the shader files
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	//throw an error if the file cannot be opened
	std::cout << "Error: Could not open file: " << filename << std::endl;
	throw(errno);
}

Shader::Shader()
{

}

Shader::~Shader()
{
	//Delete();
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//get the contents of the files as a c++ ctring
	std::string vertCode = get_file_contents(vertexFile);
	std::string fragCode = get_file_contents(fragmentFile);

	//convert to a c string (character array)
	const char* vertexSource = vertCode.c_str();
	const char* fragmentSource = fragCode.c_str();

	//attach the shader code to a shader object and compile
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//Create the Shader Program
	ID = glCreateProgram();

	//attach the shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);

	//delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}