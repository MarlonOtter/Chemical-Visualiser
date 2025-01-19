


//includes to all the other files
#include "Renderer/ObjectArray.h"
#include "Renderer/Grid.h"

#include "ChemicalVis/DrawChemical.h"

#include "ChemicalVis/GUI.h"

// width and height of the window that is going to be created in pixels
int width = 800;
int height = 800;



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


	// create a shader program for bonds and atoms
	// and attach its vertex and fragment shader
	Shader atomShader("Shaders/atom.vert", "Shaders/atom.frag");
	Shader bondShader("Shaders/bond.vert", "Shaders/bond.frag");

	//define the shaders for the grid and axis
	Shader gridShader("Shaders/DebugLine.vert", "Shaders/gridLines.frag");
	Shader axisShader("Shaders/axisLines.vert", "Shaders/axisLines.frag");

	//add the size of the lines
	axisShader.Activate();
	glUniform1f(glGetUniformLocation(axisShader.ID, "size"), 150.0f);

	//define and import the models
	Model atomModel("Resources/Models/Atoms/1x.obj");
	Model bondModel("Resources/Models/Bonds/2x.obj");

	
	// creates the debugGrid with a size of 80 square and and interval of 1.0m
	Grid grid(80, 1.0f);

	//set the colour of the light in the shaderss
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	
	atomShader.Activate();
	glUniform3f(glGetUniformLocation(atomShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	bondShader.Activate();
	glUniform3f(glGetUniformLocation(bondShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);

	// enable camera depth to work properly
	glEnable(GL_DEPTH_TEST);

	// Cull the back of triangles
	// This reduces the number of triangles that need to be draw as if they are facing the wrong way they
	// will be discarded
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	//Initialise GUI
	GUI GUI;
	GUI.Setup(window);

	//create the camera object
	Camera camera(width, height, glm::vec3(1.5f, 1.0f, 2.0f));
	globalClass::camera = &camera;

	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//update the size of the frame that is created depending on the size of the window
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// if the window is minimised. don't draw anything
		if (width == 0 || height == 0 || !glfwGetWindowAttrib(window, GLFW_FOCUSED))
		{
			glfwPollEvents();
			continue;
		}

		//change the colour of the background to the one in the render options
		glClearColor(GUI.renderOptions.bgColour[0], GUI.renderOptions.bgColour[1], GUI.renderOptions.bgColour[2], 1.0f);
		//clears the buffer bits
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

		//debug settings
		//tempory for development
		if (GUI.renderOptions.grid)
		{
			//draw a grid and axis
			grid.Draw(gridShader, axisShader, camera);
		}

		//create a temparary UI to tweak atom size and clear of all atoms
		ImGui::Begin("Atom Settings");
		ImGui::DragFloat("atom scale", &globalClass::atomScale, 0.001f, 0.001f, 100.0f);
		ImGui::DragFloat("bond width", &globalClass::bondRadius, 0.001f, 0.001f, 1000.0f);
		ImGui::DragFloat("bond length", &globalClass::bondLengthMultiplier, 0.001f, 0.001f, 1000.0f);
		ImGui::DragFloat("bond seperation Dist", &globalClass::bondSeperationDist, 0.0001f, 0.001f, 10.0f);
		
		if (ImGui::Button("Clear Chemicals"))
		{
			for (int i = 0; i < globalClass::chemicals.size(); i++)
			{
				globalClass::chemicals[i].atoms.clear();
			}
			globalClass::chemicals.clear();
		}
		ImGui::End();

		if (globalClass::chemicals.size() > 0)
		{
			DrawChemical::Draw(globalClass::chemicals, atomModel, bondModel, camera, atomShader, bondShader);	
		}
		

		//Draw the UI to the screen
		GUI.Draw();

		//swap to the next frame
		glfwSwapBuffers(window);
		
		//check for window events such as closing or resizing
		glfwPollEvents();
	}
	grid.gridLine.Delete();
	grid.axisLine.Delete();
	//delete the shader
	atomShader.Delete();
	bondShader.Delete();

	gridShader.Delete();
	axisShader.Delete();
	
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


