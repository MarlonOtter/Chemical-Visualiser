//libraries
#include <iostream> //input and output to the terminal

//includes to all the other files
#include "Renderer/Model.h"
#include "Renderer/Grid.h"

bool debug = true;

// width and height of the window that is going to be created in pixels
int width = 800;
int height = 800;

// the change in scroll on the scroll wheel
float scrollOffset = 0.0f;

// colour of the background
glm::vec4 bgColour = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

// this is called whenever the user scrolls with their mosue
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	// updates the scroll offset
	scrollOffset += (float)yOffset;
}

// this is called whenever the user changes the size of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// sets the size of the viewport
	glViewport(0, 0, width, height);
}

//function that calculates the time for a frame
double CalcDeltaTime(double& prevTime)
{
	double crntTime = glfwGetTime();
	double deltaTime = crntTime - prevTime;
	prevTime = crntTime;
	return deltaTime;
}


//main function
//runs when the program is first executed
int main()
{
	//initialise GLFW
	glfwInit();
	//tell glfw what version of openGL to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//use the "core profile"
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//allows the window to be resized
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	//Create a window
	//								     ( X px,   Y px,     window title,   monitorID, ???)
	GLFWwindow* window = glfwCreateWindow(width, height, "Chemical Visualiser", NULL, NULL);

	//test that the window has been correctly made
	if (window == NULL)
	{
		//Output Error
		std::cout << "Failed to create a window";
		//terminate glfw
		glfwTerminate();
		//stop the program
		return -1;
	}
	//bring the window to the front
	glfwMakeContextCurrent(window);
	//maximize the window
	glfwMaximizeWindow(window);

	//set the framebuffer callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//get glad to configure OpenGL
	gladLoadGL();

	//limit the FPS to the monitors refresh rate (V-SYNC)
	glfwSwapInterval(1);

	//get the size of the window
	glfwGetFramebufferSize(window, &width, &height);
	//define the viewport
	glViewport(0, 0, width, height);

	//create a shader program and attach its vertex and fragment shader
	Shader shader("Shaders/default.vert", "Shaders/default.frag");

	Shader atomShader("Shaders/default.vert", "Shaders/atom.frag");

	//create the shader program for drawing debug lines
	Shader DebugLineShader("Shaders/DebugLine.vert", "Shaders/DebugLine.frag");

	Shader gridShader("Shaders/DebugLine.vert", "Shaders/gridLines.frag");

	Shader axisShader("Shaders/axisLines.vert", "Shaders/axisLines.frag");
	axisShader.Activate();
	glUniform1f(glGetUniformLocation(axisShader.ID, "size"), 150.0f);

	//define all the textures that are used for the models

	Texture textures[] =
	{
		Texture("Resources/Textures/transCubeTex.png", "diffuse", 0),
		Texture("Resources/Textures/spec.png", "specular", 1)
	};


	//define the vectors of textures
	std::vector<Texture> texts(textures, textures + sizeof(textures) / sizeof(Texture));

	//define and import the models
	Model sphere("Resources/Models/Spheres/1xV2.obj", glm::vec3(0.0f, 0.0f, 0.0f), texts);
	sphere.Scale(glm::vec3(0.5f, 0.5f, 0.5f)); // makes the sphere 1m wide
	sphere.Translate(glm::vec3(0.5f, 0.5f, 0.5f)); //moves sphere to desired position

	Grid grid(80, 1.0f); // creates the debugGrid with a size of 80 square and and interval of 1.0m
	
	//set the colour of the light in the shader
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	shader.Activate();
	glUniform3f(glGetUniformLocation(shader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	atomShader.Activate();
	glUniform3f(glGetUniformLocation(atomShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);

	//enable camera depth to work properly
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	//set camera position
	Camera camera(width, height, glm::vec3(1.5f, 1.0f, 2.0f));
	//scroll to control speed
	glfwSetScrollCallback(window, scroll_callback);
	//set orientation
	camera.orientation = glm::vec3(-0.59f, -0.26f, -0.76f);


	//defined for deltaTime
	double crntTime = 0;
	double prevTime = 0;
	double deltprevTime = 0;
	double timeDiff;
	int FPScounter = 0;
	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//change the colour of the background
		glClearColor(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
		//clears the buffer and applies the colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calculates frame time
		float deltaTime = CalcDeltaTime(deltprevTime);

		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		FPScounter++;
		if (timeDiff >= 1.0 / 30.0)
		{
			std::string FPS = std::to_string((1.0f / timeDiff) * FPScounter);
			std::string title = "Chemical Visualiser : " + FPS.substr(0, FPS.find(".") + 2) + "FPS";
			
			glfwSetWindowTitle(window, title.c_str());

			prevTime = crntTime;
			FPScounter = 0;
		}
	
		//do any inputs
		camera.Inputs(window, deltaTime, scrollOffset);
		//update the size of the window in the camera class
		camera.UpdateSize(window);

		//update any matrices for the camera
		camera.UpdateMatrix(60.0f, 0.1f, 100.0f);

		//draw the models
		sphere.Draw(shader, camera);

		//debug settings
		if (debug) 
		{
			//draw a grid and axis
			grid.Draw(gridShader, axisShader, camera);
		}

		//swap to the next frame
		glfwSwapBuffers(window);

		//check for window events such as closing or resizing
		glfwPollEvents();
	}
	//delete the shader
	shader.Delete();

	//close the window
	glfwDestroyWindow(window);
	//end glfw
	glfwTerminate();

	//this is required to return an int so that the program can notify the user of any issues
	return 0;
}