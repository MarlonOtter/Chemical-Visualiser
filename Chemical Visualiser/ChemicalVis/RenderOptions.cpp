#include "RenderOptions.h"

void RenderOptionsWindow::Display()
{
	//create the window
	ImGui::Begin("Camera Settings");

	//Display settings
	ListSettings();

	//finish creating the window
	ImGui::End();
}

void RenderOptionsWindow::ListSettings()
{
	if (ImGui::TreeNode("Graphics Settings"))
	{
		if (ImGui::DragFloat("FOV", &FOV, 0.1f, 0.001f) && FOV <= 0.0f)
		{
			FOV = 0.001f;
		}
		if (ImGui::DragFloat("Near Plane", &nearPlane, 0.001f, 0.001f))
		{
			if (nearPlane <= 0.0f) nearPlane = 0.001f;
			if (farPlane <= nearPlane) farPlane = nearPlane + 0.001f;
		}
		if (ImGui::DragFloat("Far Plane", &farPlane, 0.1f, nearPlane + 0.001f) && farPlane <= nearPlane)
		{
			farPlane = nearPlane + 0.001f;
		}

		if (ImGui::Checkbox("Wireframe Mode", &wireFrame))
		{
			//Toggle Wireframe mode
			if (wireFrame) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::Checkbox("Grid Toggle", &grid);
		ImGui::ColorEdit3("Background Colour", bgColour);
		ImGui::TreePop();
	}
		
	if (ImGui::TreeNode("Other Options"))
	{
		if (ImGui::DragFloat("Camera Speed", &cameraSpeed, 0.1f) && cameraSpeed < 0.0f)
		{
			cameraSpeed = 0.0f;
		}
		globalClass::camera->speed = cameraSpeed;
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::DragFloat("Global Font Size", &io.FontGlobalScale, 0.01f))
		{

		}
		std::string objCount = "Object Count : ";// +std::to_string(ObjectArray::size());
		ImGui::Text(objCount.c_str());
		ImGui::TreePop();
	}
	
	
}
