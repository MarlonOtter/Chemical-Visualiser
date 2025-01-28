


//includes to all the other files
#include "Renderer/ObjectArray.h"
#include "Renderer/Grid.h"

#include "ChemicalVis/DrawChemical.h"

#include "ChemicalVis/GUI.h"

#include "ChemicalVis/CameraControls.h"

#include "Viewport.h"

// width and height of the window that is going to be created in pixels

int windowWidth = 800;
int windowHeight = 800;

glm::vec4 viewPort1;
glm::vec4 viewPort2;

bool isResizing(ImVec2 currentSize, ImVec2 prevSize)
{
	return (currentSize.x != prevSize.x || currentSize.y != prevSize.y) &&
		ImGui::IsMouseDragging(0);  // Check for left mouse button drag
		//ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);
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
	glfwSwapInterval(1);

	//get the size of the window
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	//define the viewport first
	glViewport(0, 0, windowWidth/2, windowHeight);


	// create a shader program for bonds and atoms
	// and attach its vertex and fragment shader
	Shader atom3DShader("Shaders/atom.vert", "Shaders/atom3D.frag");
	Shader atom2DShader("Shaders/atom.vert", "Shaders/atom2D.frag");

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
	
	atom3DShader.Activate();
	glUniform3f(glGetUniformLocation(atom3DShader.ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
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
	Camera camera3D(800, 800, glm::vec3(0,0,5));
	
	globalClass::camera3D = &camera3D;

	//second viewport
	glViewport(windowWidth / 2, 0, windowWidth / 2, windowHeight);
	Camera camera2D(800, 800, glm::vec3(8, 2, 10));
	globalClass::camera2D = &camera2D;

	glm::vec4 viewPort1(0, 0, windowWidth * GUI.renderOptions.viewPortDivider, windowHeight);
	glm::vec4 viewPort2(windowWidth * GUI.renderOptions.viewPortDivider, 0, windowWidth * (1 - GUI.renderOptions.viewPortDivider), windowHeight);
	ImVec2 prevSize;
	int ResizingCounter = 0;

	float viewportRatio = 0.5f;

	Viewport viewport1(0, 0, viewPort1.z, viewPort1.w);
	Viewport viewport2(viewPort2.x, 0, viewPort2.z, viewPort2.w);


	//keep the window open
	while (!glfwWindowShouldClose(window))
	{
		//update the size of the frame that is created depending on the size of the window
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
		

		// if the window is minimised. don't draw anything
		if (windowWidth == 0 || windowHeight == 0 || !glfwGetWindowAttrib(window, GLFW_FOCUSED))
		{
			glfwPollEvents();
			continue;
		}

		viewport1.setPos(glm::vec2(0,0));
		viewport1.setSize(glm::vec2(windowWidth * viewportRatio, windowHeight));
		viewport1.Update(GUI.renderOptions.bgColour);

		CameraControls::Update();

		GUI.CreateElements();

		//debug settings
		//tempory for development
		if (GUI.renderOptions.grid)
		{
			//draw a grid and axis
			grid.Draw(gridShader, axisShader, camera3D);
		}

		ImGuiWindowFlags flags = 0;
		flags |= ImGuiWindowFlags_NoBackground;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoTitleBar;

		//Create a window for each viewport so that they can individually be selected
		ImGui::SetNextWindowPos(ImVec2(viewPort1.x, viewPort1.y));
		//ImGui::SetNextWindowSize(ImVec2(viewPort1.z, viewPort1.w), ImGuiCond_FirstUseEver);
		ImGui::Begin("ViewPort 1", 0, flags);
		bool view1Focus = ImGui::IsWindowHovered();
		ImVec2 viewSize = ImGui::GetWindowSize();
		viewportRatio = viewSize.x / static_cast<float>(windowWidth);
		viewPort1.z = viewSize.x;
		if (viewSize.y < viewPort1.w)
		{
			ImGui::SetWindowSize("ViewPort 1", ImVec2(viewSize.x, viewPort1.w));
		}
		ImGui::End();

		
		viewPort2.x = viewPort1.x + viewPort1.z;
		viewPort2.z = windowWidth - viewPort2.x;
		ImGui::SetNextWindowPos(ImVec2(viewPort2.x, viewPort2.y));
		ImGui::SetNextWindowSize(ImVec2(viewPort2.z, viewPort2.w));
		ImGui::Begin("ViewPort 2", 0, flags);
		bool view2Focus = ImGui::IsWindowHovered();
		
		ImGui::End();

		//create a temparary UI to tweak atom size and clear of all atoms
		ImGui::Begin("Atom Settings");
		ImGui::DragFloat("atom scale", &globalClass::atomScale, 0.001f, 0.001f, 100.0f);
		ImGui::DragFloat("bond width", &globalClass::bondRadius, 0.001f, 0.001f, 1000.0f);
		ImGui::DragFloat("bond length", &globalClass::bondLengthMultiplier, 0.001f, 0.001f, 1000.0f);
		ImGui::DragFloat("bond seperation Dist", &globalClass::bondSeperationDist, 0.001f, 0.001f, 10.0f);
		ImGui::DragFloat("chemical seperation Dist", &globalClass::chemicalSeperationDist, 0.001f, 0.001f, 10.0f);
		
		if (ImGui::Button("Clear Chemicals"))
		{
			for (int i = 0; i < globalClass::chemicals.size(); i++)
			{
				globalClass::chemicals[i].atoms.clear();
			}
			globalClass::chemicals.clear();
		}

		ImGui::End();

		if (isResizing(viewSize, prevSize))
		{
			ResizingCounter = 5;
		}
		ResizingCounter--;

		//if the GUI is being hovered over ignore user inputs to the visualiser
		if (view1Focus && !(ResizingCounter > 0))
		{  
			//do any inputs
			if (ImGui::IsKeyDown(ImGuiKey_T))
			{
				camera3D.Inputs(window, GUI.io->DeltaTime, GUI.io->MouseWheel);
			}
			else
			{
				CameraControls::Inputs3D(camera3D, *GUI.io);
			}
			
		}
		prevSize = viewSize;
		//update the size of the window in the camera class
		camera3D.UpdateSize(viewport1.getPos(), viewport1.getSize());


		//update any matrices for the camera
		camera3D.UpdateMatrix(GUI.renderOptions.FOV, GUI.renderOptions.nearPlane, GUI.renderOptions.farPlane);

		



		if (globalClass::chemicals.size() > 0)
		{
			DrawChemical::Draw(globalClass::chemicals, atomModel, bondModel, camera3D, atom3DShader, bondShader, Chemical::_3D);	
		}
		
		viewport2.setPos(glm::vec2(viewPort2.x, 0));
		viewport2.setSize(glm::vec2(windowWidth * (1.0f - viewportRatio), windowHeight));
		viewport2.Update(GUI.renderOptions.bgColour2);

		if (view2Focus)
		{
			//do any inputs
			if (ImGui::IsKeyDown(ImGuiKey_T))
			{
				camera2D.Inputs(window, GUI.io->DeltaTime, GUI.io->MouseWheel);
			}
			else
			{
				CameraControls::Inputs2D(camera2D, *GUI.io);
			}
			
		}

		//update the size of the window in the camera class
		camera2D.UpdateSize(viewport2.getPos(), viewport2.getSize());

		//update any matrices for the camera
		camera2D.UpdateMatrix(GUI.renderOptions.FOV, GUI.renderOptions.nearPlane, GUI.renderOptions.farPlane, 1);

		if (GUI.renderOptions.grid)
		{
			//draw a grid and axis
			grid.Draw(gridShader, axisShader, camera2D);
		}

		DrawChemical::Draw(globalClass::chemicals, atomModel, bondModel, camera2D, atom2DShader, bondShader, Chemical::_2D);

		//Draw the UI to the screen (ontop of everything)
		GUI.Draw();


		//swap to the next frame
		glfwSwapBuffers(window);
		
		//check for window events such as closing or resizing
		glfwPollEvents();
	}
	grid.gridLine.Delete();
	grid.axisLine.Delete();
	//delete the shader
	atom3DShader.Delete();
	atom2DShader.Delete();
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




