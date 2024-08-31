//libraries
#include <iostream> //input and output to the terminal

//includes to all the other files
#include "Model.h"

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
float CalcDeltaTime(double& prevTime)
{
	double crntTime = glfwGetTime();
	float deltaTime = float(crntTime - prevTime);
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

	//define all the textures that are used for the models
	Texture spec("Resources/Textures/spec.png", "diffuse", 1);
	Texture textures1[] =
	{
		Texture("Resources/Textures/sphereTexture.png", "diffuse", 0),
		spec
	};

	Texture textures2[] =
	{
		Texture("Resources/Textures/cubeTexture.png", "diffuse", 0),
		spec
	};

	//define the vectors of textures
	std::vector<Texture> texts1(textures1, textures1 + sizeof(textures1) / sizeof(Texture));
	std::vector<Texture> texts2(textures2, textures2 + sizeof(textures2) / sizeof(Texture));

	//define and import the models
	Model sphere("Resources/Models/Spheres/1xV2.obj", texts1, glm::vec3(0.0f, 0.0f, 0.0f));
	Model cube("Resources/Models/cube/cube.obj", texts2, glm::vec3(4.0f, 0.0f, 0.0f));
	
	//set the colour of the light in the shader
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	shader.Activate();
	glUniform3f(glGetUniformLocation(shader.ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z);

	//enable camera depth to work properly
	glEnable(GL_DEPTH_TEST);

	//set camera position
	Camera camera(width, height, glm::vec3(1.5f, 1.0f, 2.0f));
	//scroll to control speed
	glfwSetScrollCallback(window, scroll_callback);
	//set orientation
	camera.orientation = glm::vec3(-0.59f, -0.26f, -0.76f);

	//defined for deltaTime
	double prevTime = 0;
	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//change the colour of the background
		glClearColor(bgColour.r, bgColour.g, bgColour.b, bgColour.a);
		//clears the buffer and applies the colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//calculates frame time
		float deltaTime = CalcDeltaTime(prevTime);
	
		//do any inputs
		camera.Inputs(window, deltaTime, scrollOffset);
		//update the size of the window in the camera class
		camera.UpdateSize(window);

		//update any matrices for the camera
		camera.UpdateMatrix(60.0f, 0.1f, 100.0f);

		//draw the models
		sphere.Draw(shader, camera);
		cube.Draw(shader, camera);

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