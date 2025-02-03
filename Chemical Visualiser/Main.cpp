
//includes to all the other files

// I can probably remove this and the Object Class
// As i won't Use it in the project
#include "Renderer/ObjectArray.h"
// Can remove this aswell
#include "Renderer/Grid.h"

// The 2 Different Types of Camera
#include "Camera2D.h"
#include "ArcBallCamera.h"

#include "ChemicalVis/DrawChemical.h"
#include "ChemicalVis/GUI.h"

#include "Viewport.h"

// width and height of the window that is going to be created in pixels
int windowWidth = 800;
int windowHeight = 800;
glm::vec2 prevWindowSize(800, 800);


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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Chemical Visualiser", NULL, NULL);

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
	//should probably be a setting
	glfwSwapInterval(1);

	// create a shader program for bonds and atoms
	// and attach its vertex and fragment shader
	Shader atom3DShader("Shaders/atom.vert", "Shaders/atom3D.frag");
	Shader atom2DShader("Shaders/atom.vert", "Shaders/atom2D.frag");

	Shader bondShader("Shaders/bond.vert", "Shaders/bond.frag");

	//define and import the models
	Model atomModel3D("Resources/Models/Atoms/Atom3D.obj");
	Model bondModel3D("Resources/Models/Bonds/Bond3D.obj");

	Model atomModel2D("Resources/Models/Atoms/Atom2D.obj");
	
	// Bond cannot be 2D as some are rotated and cannot be seen
	//Model bondModel2D("Resources/Models/Bonds/Bond2D.obj");

	//set the colour of the light in the shaderss
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	
	atom3DShader.Activate();
	// Add the colour and the source vector to the atom 3d shader
	glUniform3f(glGetUniformLocation(atom3DShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	glUniform3f(glGetUniformLocation(atom3DShader.ID, "lightSource"), 0, 1, 0);
	bondShader.Activate();
	glUniform3f(glGetUniformLocation(bondShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	glUniform3f(glGetUniformLocation(bondShader.ID, "lightSource"), 0, 1, 0);
	glUniform3f(glGetUniformLocation(bondShader.ID, "bondColour"), 0,0,0);

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

	//create the arcball camera object
	//the Z position has to be <0
	ArcBallCamera camera3D(800, 800, glm::vec3(0,0, -10));
	
	globalClass::camera3D = &camera3D;

	Camera2D camera2D(800, 800, glm::vec3(8, 2, 10));
	globalClass::camera2D = &camera2D;

	glm::vec2 viewportRatio = glm::vec2(0.5f, 1.0f);

	Viewport visualiser3D(0, 0, 0, 0, false, true);
	Viewport visualiser2D(0, 0, 0, 0, false, true);

	int frameInterval = 1;

	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//update the size of the frame that is created depending on the size of the window
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		
		glfwSwapInterval(frameInterval);
		frameInterval = 2;
		// if the window is minimised/unfocussed. don't draw anything
		if (windowWidth == 0 || windowHeight == 0 || !glfwGetWindowAttrib(window, GLFW_FOCUSED))
		{
			glfwPollEvents();

			//reduce background usage by running the main loop 15times/sec instead of 60
			frameInterval = 4;
			continue;
		}


		//if the glfw window was just resized, update the window size ratio
		if (prevWindowSize.x == windowWidth && prevWindowSize.y == windowHeight)
		{
			viewportRatio = visualiser3D.calculateRelativeSize(windowWidth, windowHeight);
		}

		visualiser3D.setPos(0, 0);
		visualiser3D.setSize(viewportRatio, glm::vec2(windowWidth, windowHeight));
		visualiser3D.Update(GUI.renderOptions.bgColour);

		GUI.CreateElements();

		//atomModel.Draw(atom3DShader, camera3D, camera3D.position);
		visualiser3D.AttachWindow("3D Visualiser");
		

		//create a temparary UI to tweak atom size and clear of all atoms
		ImGui::Begin("Atom Settings");
		ImGui::DragFloat("atom scale", &globalClass::atomScale, 0.001f, 0.001f, 100.0f);
		ImGui::DragFloat("bond width", &globalClass::bondRadius, 0.001f, 0.001f, 1000.0f);
		ImGui::DragFloat("bond length", &globalClass::bondLengthMultiplier, 0.001f, 0.001f, 1000.0f);
		ImGui::DragFloat("bond seperation Dist", &globalClass::bondSeperationDist, 0.001f, 0.001f, 10.0f);
		ImGui::DragFloat("chemical seperation Dist", &globalClass::chemicalSeperationDist, 0.001f, 0.001f, 10.0f);

		//trying to get a value that can alter the speed of rotation
		ImGui::DragFloat("cameraSpeed", &camera3D.speed, 0.01f, 0.0001f, 10.0f);
		
		if (ImGui::Button("Clear Chemicals"))
		{
			for (int i = 0; i < globalClass::chemicals.size(); i++)
			{
				globalClass::chemicals[i].atoms.clear();
			}
			globalClass::chemicals.clear();
		}
		ImGui::End();

		// If the viewport is being hovered over
		if (visualiser3D.getHovered())
		{  
			// Do mouse Inputs 
			camera3D.MouseInputs(*GUI.io);
		}
		// If the viewport is focussed (clicked on)
		if (visualiser3D.getFocussed())
		{
			// Do Keyboard Inputs
			camera3D.KeyInputs(*GUI.io);
		}
		// Apply the rotation to the camera using the inputs
		camera3D.ApplyInputs();

		//update the size of the window in the camera class
		camera3D.UpdateSize(visualiser3D.getPos(), visualiser3D.getSize());
		

		//update any matrices for the camera
		camera3D.UpdateMatrix(GUI.renderOptions.FOV, GUI.renderOptions.nearPlane, GUI.renderOptions.farPlane);
		
		//update the light source direction so that the atoms are always lit
		glm::vec3 lightSourceDir = camera3D.getPos();
		atom3DShader.Activate();
		glUniform3f(glGetUniformLocation(atom3DShader.ID, "lightSource"), lightSourceDir.x, lightSourceDir.y, lightSourceDir.z);

		if (globalClass::chemicals.size() > 0)
		{
			// Passing in the arcballCamera as the base class Camera, is bad practice as it can cause issues
			// However I can't think of anouther solution that wouldn't be more work.
			DrawChemical::Draw(globalClass::chemicals, atomModel3D, bondModel3D, camera3D, atom3DShader, bondShader, Chemical::_3D);	
		}
		
		// Give it a window so that detecting when it should be active is easier
		visualiser2D.AttachWindow("2D Visualiser");
		
		// Set the size + position of the viewport based on the other 
		visualiser2D.setPos(visualiser3D.getPos().x + visualiser3D.getSize().x, 0);
		visualiser2D.setSize(windowWidth - (visualiser3D.getPos().x + visualiser3D.getSize().x), windowHeight);
		visualiser2D.Update(GUI.renderOptions.bgColour2);

		// If the viewport is being hovered on
		if (visualiser2D.getHovered())
		{
			// Do mouse Inputs
			camera2D.MouseInputs(*GUI.io);
		}
		// If it is focussed
		if (visualiser2D.getFocussed())
		{
			// Do Key Inputs
			camera2D.KeyInputs(*GUI.io);
		}
		

		//update the size of the window in the camera class
		camera2D.UpdateSize(visualiser2D.getPos(), visualiser2D.getSize());

		//update any matrices for the camera
		camera2D.UpdateMatrix(GUI.renderOptions.nearPlane, GUI.renderOptions.farPlane);

		DrawChemical::Draw(globalClass::chemicals, atomModel2D, bondModel3D, camera2D, atom2DShader, bondShader, Chemical::_2D);

		//Draw the UI to the screen (ontop of everything)
		GUI.Draw();


		//swap to the next frame
		glfwSwapBuffers(window);
		
		//check for window events such as closing or resizing
		glfwPollEvents();
		prevWindowSize = glm::vec2(windowWidth, windowHeight);
	}

	//delete the shaders
	atom3DShader.Delete();
	atom2DShader.Delete();
	bondShader.Delete();


	//Delete the models (not really necessary)
	atomModel2D.Delete();
	atomModel3D.Delete();

	//bondModel2D.Delete();
	bondModel3D.Delete();

	
	// Shutdown the ImGui stuff
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




