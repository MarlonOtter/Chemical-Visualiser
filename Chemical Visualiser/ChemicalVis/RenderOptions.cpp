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
		if (ImGui::DragFloat("Near Plane", &nearPlane, 0.001f, 0.0001f, 10000.0f))
		{
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
		ImGui::ColorEdit3("Background Colour 1", bgColour);
		ImGui::ColorEdit3("Background Colour 2", bgColour2);
		ImGui::TreePop();
	}
		
	if (ImGui::TreeNode("Other Options"))
	{
		ImGui::DragFloat("Camera Speed", &cameraSpeed, 0.1f, 0.001f, 1000.0f);
		ImGui::DragFloat("ortho Scale", &orthoScale, 0.00001f, 0.000001f, 1.0f, "%.5f");
		
		ImGuiIO& io = ImGui::GetIO();
		ImGui::DragFloat("Global Font Size", &io.FontGlobalScale, 0.01f);
		
		ImGui::SliderFloat("Screen Divider", &viewPortDivider, 0.0001f, 1.0f);
		ImGui::TreePop();
	}

	glm::vec3 camPos = globalClass::camera3D->position;
	std::string camPosString = "Position = " + std::to_string(camPos.x) + ", " + std::to_string(camPos.y) + ", " + std::to_string(camPos.z);
	ImGui::Text(camPosString.c_str());
	
	glm::vec3 camOr = globalClass::camera3D->orientation;
	std::string camOrString = "Orientations = " + std::to_string(camOr.x) + ", " + std::to_string(camOr.y) + ", " + std::to_string(camOr.z);
	ImGui::Text(camOrString.c_str());
	
}
