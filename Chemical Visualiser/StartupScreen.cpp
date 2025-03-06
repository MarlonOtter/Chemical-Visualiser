#include "StartupScreen.h"

std::string StartupScreen::imgAddr = "Resources/Textures/StartupScreen.png";

void StartupScreen::Display(GLFWwindow* window)
{
	Draw(window);
	UpdateScreen(window);
}

void StartupScreen::Draw(GLFWwindow* window)
{
	std::vector<Texture> textures =
	{
		Texture("Resources/Textures/icon.png", "2D", GL_TEXTURE0)
	};
	Model model("Resources/Models/plane.obj", textures);
	Shader shader("Shaders/Startup.vert", "Shaders/Startup.frag");

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	Camera camera(width, height, glm::vec3(0, 0, 1));

	model.Draw(shader, camera);

	model.Delete();
	shader.Delete();
}

void StartupScreen::UpdateScreen(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}
