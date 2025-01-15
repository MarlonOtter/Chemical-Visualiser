//libraries
#include <iostream> //input and output to the terminal

//includes to all the other files
#include "Renderer/ObjectArray.h"
#include "Renderer/Grid.h"

#include "ChemicalVis/GUI.h"

// width and height of the window that is going to be created in pixels
int width = 800;
int height = 800;

bool keyJ = true;

// threading reference : 23/11/2024 : https://stackoverflow.com/questions/42418360/how-to-check-if-thread-has-finished-work-in-c11-and-above


// colour of the background
glm::vec4 bgColour = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);


// this is the code that is needed to make a request to the database with name
//it is temperary how it is 
static glm::vec3 Float3ToVec3(float nums[3])
{
	return glm::vec3(nums[0], nums[1], nums[2]);
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
		std::cout << "Failed to create a window\n";
		//terminate glfw
		glfwTerminate();
		//stop the program
		return -1;
	}
	//bring the window to the front
	glfwMakeContextCurrent(window);
	//maximize the window
	glfwMaximizeWindow(window);

	//get glad to configure OpenGL
	gladLoadGL();

	//limit the FPS to the monitors refresh rate (V-SYNC)
	glfwSwapInterval(1);

	//get the size of the window
	glfwGetFramebufferSize(window, &width, &height);
	//define the viewport
	glViewport(0, 0, width, height);

	
	//Initialise GUI
	chemVisGUI GUI(window);

	//create a shader program and attach its vertex and fragment shader
	Shader shader("Shaders/default.vert", "Shaders/default.frag");

	Shader atomShader("Shaders/default.vert", "Shaders/atom.frag");

	//create the shader program for drawing debug lines
	Shader DebugLineShader("Shaders/DebugLine.vert", "Shaders/DebugLine.frag");

	//grid stuff
	Shader gridShader("Shaders/DebugLine.vert", "Shaders/gridLines.frag");
	Shader axisShader("Shaders/axisLines.vert", "Shaders/axisLines.frag");
	axisShader.Activate();
	glUniform1f(glGetUniformLocation(axisShader.ID, "size"), 150.0f);

	//define all the textures that are used for the models

	Texture textures[] =
	{
		Texture("Resources/Textures/cubeTexture.png", "diffuse", 0),
		Texture("Resources/Textures/spec.png", "specular", 1)
	};


	//define the vectors of textures
	std::vector<Texture> texts(textures, textures + sizeof(textures) / sizeof(Texture));

	//define and import the models
	Model sphereModel("Resources/Models/Spheres/1xV2.obj", texts);
	Model cubeModel("Resources/Models/cube/cube.obj", texts);

	// the use of an object class means that there is only one copy of the vertices
	// in memory, reducing memory usage if there are lots of the same model like in
	// this project. It also doesn't impact other the cost of models

	// This creates the object and adds itself to an object array that loops through and draws each one
	Object atomObj("atom", &sphereModel, &atomShader);
	



	//TODO: rotation does work however it doesn't affect the light so the light sees to come from the wring direction
	
	// creates the debugGrid with a size of 80 square and and interval of 1.0m
	Grid grid(80, 1.0f);

	//set the colour of the light in the shaderss
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	shader.Activate();
	glUniform3f(glGetUniformLocation(shader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	atomShader.Activate();
	glUniform3f(glGetUniformLocation(atomShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);

	// enable camera depth to work properly
	glEnable(GL_DEPTH_TEST);

	// Cull the back of triangles
	// This reduces the number of triangles that need to be draw as if they are facing the wrong way they
	// will be discarded
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	//set camera position
	Camera camera(width, height, glm::vec3(1.5f, 1.0f, 2.0f));

	globalClass::camera = &camera;
	globalClass::atomShader = &shader;
	globalClass::atomModel = &cubeModel;

	float atomScale = 0.3f;

	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//update the size of the frame that is created depending on the size of the window
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// if the window is minimised. don't draw anything
		if (width == 0 && height == 0) 
		{
			glfwPollEvents();
			continue;
		}
		
		
		camera.speed = GUI.renderOptions.cameraSpeed;

		//change the colour of the background to the one in the render options
		glClearColor(GUI.renderOptions.bgColour[0], GUI.renderOptions.bgColour[1], GUI.renderOptions.bgColour[2], 1.0f);
		//clears the buffer and applies the colour
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		//if the GUI is being hovered over ignore user inputs to the visualiser
		if (!GUI.io->WantCaptureMouse && !GUI.io->WantCaptureKeyboard)
		{
			//do any inputs
			camera.Inputs(window, GUI.io->DeltaTime, GUI.io->MouseWheel);
		}
		//update the size of the window in the camera class
		camera.UpdateSize(window);

		//update any matrices for the camera
		camera.UpdateMatrix(GUI.renderOptions.FOV, GUI.renderOptions.nearPlane, GUI.renderOptions.farPlane);
			
		GUI.CreateElements();
		//draw all the models
		//ObjectArray::Draw(camera);

		//debug settings
		if (GUI.renderOptions.grid)
		{
			//draw a grid and axis
			grid.Draw(gridShader, axisShader, camera);
		}

		//create a temparary UI to tweak atom size and clear of all atoms
		ImGui::Begin("Atom Settings");
		ImGui::DragFloat("scale", &atomScale, 0.001f, 0.0f, 100.0f);
		atomObj.scale = glm::vec3(atomScale, atomScale, atomScale);
		if (ImGui::Button("Clear Chemicals"))
		{
			globalClass::chemicals.clear();
		}
		ImGui::End();

		for (int i = 0; i < globalClass::chemicals.size(); i++)
		{
			//TODO: This can be improved by using instancing
			for (int j = 0; j < globalClass::chemicals[i].atoms.size(); j++)
			{
				// check if there is an atom which has no element associated with it
				if (globalClass::chemicals[i].atoms[j].element == 0) {
					std::cout << "ERROR: Atom with no element" << std::endl;
					continue;
				}
				//set the position of the atom object to the correct location
				atomObj.pos = globalClass::chemicals[i].atoms[j].pos + glm::vec3(0.0f, 0.0f, i * 2);
				atomObj.scale = glm::vec3(atomScale, atomScale, atomScale);
				atomObj.scale = atomObj.scale * std::min(1.0f, static_cast<float>(globalClass::chemicals[i].atoms[j].element) / 2.0f);
				//get the colour of the atom to be displayed
				atomObj.shader->Activate();
				glm::vec3 colr = globalClass::chemicals[i].atoms[j].colour;
				glUniform3f(glGetUniformLocation(atomShader.ID, "atomColour"), colr.r, colr.g, colr.b);
				//Draw the atom
				atomObj.Draw(camera);
			}
		}
		
		//Draw the UI to the screen
		GUI.Draw();

		//swap to the next frame
		glfwSwapBuffers(window);
		
		//check for window events such as closing or resizing
		glfwPollEvents();
	}
	//delete the shader
	shader.Delete();
	atomShader.Delete();
	gridShader.Delete();
	axisShader.Delete();
	DebugLineShader.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//close the window
	glfwDestroyWindow(window);
	//end glfw
	glfwTerminate();

	//this is required to return an int so that the program can notify the user of any issues
	return 0;
}


