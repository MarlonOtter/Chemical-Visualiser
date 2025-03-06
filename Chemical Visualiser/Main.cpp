
// Note:
// I think my ip has been blocked by pubchem, so i cannot access 
// the service without using a hotspot or vpn

#include <map>

//includes to all the other files

// Startup is slow, so things that need to be done at start-up but not used straight away can be put on anouther thread

#include "Camera2D.h"
#include "3DVisualiser.h"
#include "2DVisualiser.h"

#include "ChemicalVis/GUI.h"
#include "ChemicalDataCache.h"
#include "ChemicalVis/Settings.h"
#include "SettingsUI.h"

#include "StartupScreen.h"

std::future<int> thread;

// The Default window size on startup
const int INITIAL_WINDOW_WIDTH = 800;
const int INITIAL_WINDOW_HEIGHT = 800;


// The different types of models
enum class ModelType {
	Atom3D,
	Bond3D,
	Atom2D,
	Bond2D
};

// The different types of shaders
enum class ShaderType {
	Atom3D,
	Bond3D,
	Atom2D,
	Bond2D
};

// Data about the window
struct windowData
{
	GLFWwindow* window = nullptr;
	int width = 0;
	int height = 0;

	int prevWidth = 0;
	int prevHeight = 0;
};

struct RenderData
{
	std::map<ModelType, Model> models;
	std::map<ShaderType, Shader> shaders;
};

struct Cameras
{
	ArcBallCamera camera3D;
	Camera2D camera2D;

	glm::vec3 bgColour3D;
	glm::vec3 bgColour2D;

	float FOV; 
	float nearPlane;
	float farPlane;
};


void initialiseGLFW();
GLFWwindow* createWindow(int width, int height, const char* title);
void setupOpenGL();

int StartupScreen(windowData& window);

std::map<ModelType, Model> loadModels();
std::map<ShaderType, Shader> loadShaders();
void InitialiseShaderUniforms(std::map<ShaderType, Shader> shaders);

int FrameStart(windowData& window, int swapInterval);
void MainLoop(windowData& window, RenderData renderData, SettingsUI& settingsUI);



void Visualiser3D(Viewport& view, float& screenRatio, windowData& window, RenderData& renderData, Cameras& cameras);
void Visualiser2D(Viewport& view, Viewport& view3D, windowData& window, RenderData& renderData, Cameras& cameras);

void TempararyGUI(char* settingInp, float* num, SettingsUI settingsUI, float& cameraSpeed);

void FrameEnd(windowData& window);

void Cleanup(windowData& window, RenderData renderData);

int main()
{
	// Initialise the window data
	windowData window = 
	{
		nullptr, 		
		INITIAL_WINDOW_WIDTH,
		INITIAL_WINDOW_HEIGHT,
		INITIAL_WINDOW_WIDTH,
		INITIAL_WINDOW_HEIGHT
	};

	// Initialise GLFW and create a window
	initialiseGLFW();
	window.window = createWindow(window.width, window.height, "Chemical Visualiser");
	// Make sure that the window was created properly
	assert(window.window != nullptr && "Window was not created properly");

	// Setup all my openGL settings
	setupOpenGL();

	StartupScreen::Display(window.window);
	//StartupScreen(window);

	// Load all the models and shaders into the program
	RenderData renderData = { 
		loadModels(), 
		loadShaders() 
	};

	InitialiseShaderUniforms(renderData.shaders);

	SettingsUI settingsUI;
	settingsUI.Init();

	// Read the settings from disk when the program boots up
	Settings::Refresh();

	// This takes a long time as the algorithm is poorly implemented
	// I could also make it a-sync as there is a lot of data that slows down boot time
	//settingsUI.Refresh();
	


	// Run the main loop of the program
	MainLoop(window, renderData, settingsUI);

	// Clean up all the resources that were used before closing the program
	Cleanup(window, renderData);

	return 0;
}

void initialiseGLFW()
{
	//initialise GLFW
	glfwInit();
	//tell glfw what version of openGL to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//use the "core profile"
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return;
}

GLFWwindow* createWindow(int width, int height, const char* title)
{
	//allows the window to be resized
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	//Create a window
	//								     ( X px,   Y px,     window title,   monitorID, ???)
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

	//test that the window has been correctly made
	if (window == NULL)
	{
		//Output Error
		std::cout << "Failed to create a window\n";
		//terminate glfw
		glfwTerminate();
		return nullptr;
	}
	
	// Attach the icon to the window.
	GLFWimage images[1];
	// Import image
	images[0].pixels = stbi_load("Resources/Textures/icon.png", &images[0].width, &images[0].height, 0, 4);
	// Make sure the image was loaded properly
	assert((images[0].height != 0 && images[0].width != 0) && "The image didn't load properly; check file exists: icon.png");

	// Apply the icon to the window
	glfwSetWindowIcon(window, 1, images);
	// Free the image data
	stbi_image_free(images[0].pixels);

	//bring the window to the front
	glfwMakeContextCurrent(window);
	//maximize the window
	glfwMaximizeWindow(window);
	
	// Return the window to the main function to be used
	return window;
}

void setupOpenGL()
{
	//get glad to configure OpenGL
	gladLoadGL();

	// enable camera depth to work properly
	glEnable(GL_DEPTH_TEST);

	// Cull the back of triangles
	// This reduces the number of triangles that need to be draw as if they are facing the wrong way they
	// will be discarded
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

}

int StartupScreen(windowData& window)
{
	glfwGetFramebufferSize(window.window, &window.width, &window.height);

	// Create Viewport
	Viewport view(0, 0, window.width, window.height, true, true);

	// Set the background colour
	view.Update(glm::vec3(0, 0, 0));
	
	// Display the startup screen
	glfwSwapBuffers(window.window);
	return 0;
}

std::map<ModelType, Model> loadModels()
{
	// Import all the models into the program
	// This could probably be done better to allow for more models to be added easily
	std::map<ModelType, Model> models;
	models[ModelType::Atom3D] = Model("Resources/Models/Atoms/Atom3D.obj");
	models[ModelType::Bond3D] = Model("Resources/Models/Bonds/Bond3D.obj");

	models[ModelType::Atom2D] = Model("Resources/Models/Atoms/Atom2D.obj"); 
	models[ModelType::Bond2D] = Model("Resources/Models/Bonds/Bond3D.obj"); // Bond cannot be 2D as some are rotated and cannot be seen

	// Validate the models loaded correctly
	for (auto& model : models)
	{
		assert(model.second.meshes.size() > 0 && "ERROR: MESHES DIDN'T LOAD CORRECTLY");
	}

	return models;
}

std::map<ShaderType, Shader> loadShaders()
{
	// Create all the shaders for the program
	// This could probably be done better to allow for more shaders to be added easily
	std::map<ShaderType, Shader> shaders;
	shaders[ShaderType::Atom3D] = Shader("Shaders/atom.vert", "Shaders/atom3D.frag");
	shaders[ShaderType::Bond3D] = Shader("Shaders/bond.vert", "Shaders/bond.frag");

	shaders[ShaderType::Atom2D] = Shader("Shaders/atom.vert", "Shaders/atom2D.frag");
	shaders[ShaderType::Bond2D] = Shader("Shaders/bond.vert", "Shaders/bond.frag");

	// Validate the shaders loaded correctly
	for (auto& shader : shaders)
	{
		assert(shader.second.ID != 0 && "ERROR: SHADER DIDN'T LOAD CORRECTLY");
	}

	return shaders;
}

void InitialiseShaderUniforms(std::map<ShaderType, Shader> shaders)
{
	//set the colour of the light in the shaderss
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);


	// Set the light source colour and position as uniforms in the shaders
	shaders[ShaderType::Atom3D].Activate();
	glUniform3f(glGetUniformLocation(shaders[ShaderType::Atom3D].ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	glUniform3f(glGetUniformLocation(shaders[ShaderType::Atom3D].ID, "lightSource"), 0, 1, 0);
	shaders[ShaderType::Bond3D].Activate();
	glUniform3f(glGetUniformLocation(shaders[ShaderType::Bond3D].ID, "lightColour"), lightColour.r, lightColour.g, lightColour.b);
	glUniform3f(glGetUniformLocation(shaders[ShaderType::Bond3D].ID, "lightSource"), 0, 1, 0);
	glUniform3f(glGetUniformLocation(shaders[ShaderType::Bond3D].ID, "bondColour"), 0, 0, 0);
}

int FrameStart(windowData& window, int swapInterval)
{
	// Run code that needs to be ran at the start of the frame

	//update the size of the frame that is created depending on the size of the window
	glfwGetFramebufferSize(window.window, &window.width, &window.height);

	if (swapInterval < 0 || swapInterval > 4) swapInterval = 1;
	glfwSwapInterval(swapInterval); // 0 : unlimitedFPS, 1 : 60FPS, 2 : 30FPS, 3 : 20FPS, 4 : 15FPS
	// if the window is minimised/unfocussed. don't draw anything
	if (window.width == 0 || window.height == 0 || !glfwGetWindowAttrib(window.window, GLFW_FOCUSED))
	{
		glfwPollEvents();

		// Reduce background usage by running the main loop at a fraction of its original speed
		glfwSwapInterval(4);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		// tell the program to not continue the entire main loop
		return 1;
	}
	// Continue main loop like normal
	return 0;
}

void MainLoop(windowData& window, RenderData renderData, SettingsUI& settingsUI)
{
	// Run the main loop of the program

	GUI::Setup(window.window);

	
	// Store all the information about the cameras in a struct
	// This is done so that the cameras can be passed around easily
	Cameras cameras = {
		//create the arcball camera object
		//the Z position has to be <0
		ArcBallCamera(800, 800, glm::vec3(0, 0, -10)),
		//create the 2D camera object
		Camera2D(800, 800, glm::vec3(8, 2, 10)),

		// Background colours for the 3D and 2D visualisers
		glm::vec3(0.15, 0.15, 0.15),
		glm::vec3(0.85, 0.85, 0.85),
		
		// FOV, Near Plane and Far Plane for both cameras where it applies
		60.0f,
		0.1f,
		100.0f
	};

	// Add the cameras to the global class so that they can be accessed from anywhere
	// I don't remember if i even use it anywhere
	globalClass::camera3D = &cameras.camera3D.camera;
	globalClass::camera2D = &cameras.camera2D.camera;

	// Percentage of the screen that the 3D visualiser will take up
	float screenRatio = 0.5f;

	// Define the 2 viewports for the 3D and 2D visualisers
	Viewport visualiser3D(0, 0, 0, 0, false, true);
	Viewport visualiser2D(0, 0, 0, 0, false, true);

	// These are just temparary for testing the Setings class
	char settingInp[128] = "Near Plane";
	float num = 0;

	int swapInterval = 1;

	bool threadDone = false;

	while (!glfwWindowShouldClose(window.window))
	{
		// If the window is minimised, don't run the main loop
		if (FrameStart(window, swapInterval) == 1) continue;

		// Update any variables
		// Only update the variables if the settings have changed
		// Otherwise the performance tanks because of my inefficient settings code
		if (Settings::changed)
		{
			cameras.bgColour3D = std::get<glm::vec3>(Settings::GetValue("3D_background_colour").data);
			cameras.bgColour2D = std::get<glm::vec3>(Settings::GetValue("2D_background_colour").data);
			cameras.FOV = std::get<float>(Settings::GetValue("camera_fov").data);
			cameras.nearPlane = std::get<float>(Settings::GetValue("camera_near").data);
			cameras.farPlane = std::get<float>(Settings::GetValue("camera_far").data);
			swapInterval = std::get<int>(Settings::GetValue("window_swap_interval").data);
		}


		//! if the glfw window was just resized, don't update the window size ratio
		if (window.prevWidth == window.width && window.prevHeight == window.height)
		{
			screenRatio = visualiser3D.calculateRelativeSize(window.width, window.height).x;
		}

		GUI::CreateElements();

		// Draw the 3D Visualiser
		Visualiser3D(visualiser3D, screenRatio, window, renderData, cameras);
		Visualiser2D(visualiser2D, visualiser3D, window, renderData, cameras);

		threadDone = true;
		Settings::changed = false;
		if (threadDone)
		{
			TempararyGUI(settingInp, &num, settingsUI, cameras.camera3D.speed);
		}

		


		FrameEnd(window);
	}
	
	return;
}

void Visualiser3D(Viewport& view, float& screenRatio, windowData& window, RenderData& renderData, Cameras& cameras)
{
	view.setPos(0, 0);
	view.setSize(glm::vec2(screenRatio, 1.0f), glm::vec2(window.width, window.height));


	view.Update(cameras.bgColour3D);

	view.AttachWindow("3D Visualiser");

	

	// If the viewport is being hovered over
	if (view.getHovered())
	{
		// Do mouse Inputs 
		cameras.camera3D.MouseInputs(*GUI::io);
	}
	// If the viewport is focussed (clicked on)
	if (view.getFocussed())
	{
		// Do Keyboard Inputs
		cameras.camera3D.KeyInputs(*GUI::io);
	}
	// Apply the rotation to the camera using the inputs
	cameras.camera3D.ApplyInputs();

	//update the size of the window in the camera class
	cameras.camera3D.camera.UpdateSize(view.getPos(), view.getSize());


	//update any matrices for the camera
	cameras.camera3D.UpdateMatrix(cameras.FOV, cameras.nearPlane, cameras.farPlane);

	// Update the light source direction so that the atoms are always lit
	glm::vec3 lightSourceDir = cameras.camera3D.getPos();
	renderData.shaders[ShaderType::Atom3D].Activate();
	glUniform3f(glGetUniformLocation(renderData.shaders[ShaderType::Atom3D].ID, "lightSource"), lightSourceDir.x, lightSourceDir.y, lightSourceDir.z);

	if (globalClass::chemicals.size() > 0)
	{
		DrawChemical::Draw(
			globalClass::chemicals,
			renderData.models[ModelType::Atom3D],
			renderData.models[ModelType::Bond3D], 
			cameras.camera3D.camera, 
			renderData.shaders[ShaderType::Atom3D],
			renderData.shaders[ShaderType::Bond3D],
			Chemical::_3D);
	}
}
void Visualiser2D(Viewport& view, Viewport& view3D, windowData& window, RenderData& renderData, Cameras& cameras)
{
	// Give it a window so that detecting when it should be active is easier
	view.AttachWindow("2D Visualiser");

	// Set the size + position of the viewport based on the other 
	view.setPos(static_cast<int>(view3D.getPos().x + view3D.getSize().x), 0);
	view.setSize(static_cast<int>(window.width - (view3D.getPos().x + view3D.getSize().x)), window.height);

	view.Update(cameras.bgColour2D);

	// If the viewport is being hovered on
	if (view.getHovered())
	{
		// Do mouse Inputs
		cameras.camera2D.MouseInputs(*GUI::io);
	}
	// If it is focussed
	if (view.getFocussed())
	{
		// Do Key Inputs
		cameras.camera2D.KeyInputs(*GUI::io);
	}

	//update the size of the window in the camera class
	cameras.camera2D.camera.UpdateSize(view.getPos(), view.getSize());

	//update any matrices for the camera
	cameras.camera2D.UpdateMatrix(cameras.nearPlane, cameras.farPlane);

	DrawChemical::Draw(
		globalClass::chemicals,
		renderData.models[ModelType::Atom2D], 
		renderData.models[ModelType::Bond2D],
		cameras.camera2D.camera, 
		renderData.shaders[ShaderType::Atom2D],
		renderData.shaders[ShaderType::Bond2D],
		Chemical::_2D
	);

}

void TempararyGUI(char* settingInp, float* num, SettingsUI settingsUI, float& cameraSpeed)
{
	if (ImGui::Begin("Settings Class Testing"))
	{

		if (ImGui::Button("Read File"))
		{
			Settings::Refresh();
		}

		if (ImGui::Button("Save to File"))
		{
			Settings::Save();
		}

		ImGui::InputText("Setting", settingInp, 128);

		if (ImGui::Button("Find"))
		{
			double startTime = glfwGetTime();
			Setting setting = Settings::GetAll(std::string(settingInp));

			std::cout << "Duration: " << (glfwGetTime() - startTime) * 1000.0 << "ms\n";
			std::cout << "Type: " << setting.type << "\nName: " << setting.name << "\n";
		}
		ImGui::SliderFloat("Number", num, 0.001f, 100.0f);
		if (ImGui::Button("Set"))
		{
			std::cout << Settings::Set(std::string(settingInp), *num) << "\n";
		}

		if (ImGui::Button("List Children"))
		{
			auto vec = Settings::getChildren("/" + std::string(settingInp));
			std::cout << "Found " << vec.size() << " Children Objects\n";
		}
		ImGui::Checkbox("Settings Changed", &Settings::changed);

	}
	ImGui::End();

	settingsUI.Draw();



	//create a temparary UI to tweak atom size and clear of all atoms
	ImGui::Begin("Atom Settings");
	ImGui::DragFloat("atom scale", &globalClass::atomScale, 0.001f, 0.001f, 100.0f);
	ImGui::DragFloat("bond width", &globalClass::bondRadius, 0.001f, 0.001f, 1000.0f);
	ImGui::DragFloat("bond length", &globalClass::bondLengthMultiplier, 0.001f, 0.001f, 1000.0f);
	ImGui::DragFloat("bond seperation Dist", &globalClass::bondSeperationDist, 0.001f, 0.001f, 10.0f);
	ImGui::DragFloat("chemical seperation Dist", &globalClass::chemicalSeperationDist, 0.001f, 0.001f, 10.0f);

	//trying to get a value that can alter the speed of rotation
	ImGui::DragFloat("cameraSpeed", &cameraSpeed, 0.01f, 0.0001f, 10.0f);

	if (ImGui::Button("Clear Chemicals"))
	{
		for (int i = 0; i < globalClass::chemicals.size(); i++)
		{
			globalClass::chemicals[i].atoms.clear();
		}
		globalClass::chemicals.clear();
	}
	ImGui::End();



}

void FrameEnd(windowData& window)
{
	// Run code that needs to be ran at the end of the frame
	// Draw the UI to the screen (ontop of everything)
	GUI::Draw();

	// swap to the next frame
	glfwSwapBuffers(window.window);

	// check for window events such as closing or resizing
	glfwPollEvents();

	// update the previous window size
	// It is done here so that the window size is only updated after the frame has been drawn
	window.prevWidth = window.width;
	window.prevHeight = window.height;

	return;
}

void Cleanup(windowData& window, RenderData renderData)
{
	// Deletes any data and cleans up the program, when it is closing

	// Delete all the shaders
	for (auto it = renderData.shaders.begin(); it != renderData.shaders.end(); it++)
	{
		it->second.Delete();
	}	

	// Deletes all the models
	for (auto it = renderData.models.begin(); it != renderData.models.end(); it++)
	{
		it->second.Delete();
	}


	// Shutdown the ImGui stuff
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//close the window
	glfwDestroyWindow(window.window);
	//end glfw
	glfwTerminate();

	return;
}

