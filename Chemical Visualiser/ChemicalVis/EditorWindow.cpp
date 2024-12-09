#include "EditorWindow.h"
#include <iostream>

void EditorWindow::Display()
{
	//create the window
	ImGui::Begin("Object Editor");

	//Display t
	DisplayTransforms();

	//finish creating the window
	ImGui::End();
}


void EditorWindow::DisplayTransforms()
{
	ImGui::InputText("ID", ObjectID, IM_ARRAYSIZE(ObjectID));

	ImGui::Text("Transforms");

	ImGui::DragFloat3("Position", position, sliderVel);
	ImGui::DragFloat3("Rotation", rotation, sliderVel);
	ImGui::DragFloat3("Scale", scale, sliderVel);
	
}

void EditorWindow::DisplayData()
{

}

void EditorWindow::setAll(std::string ID, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	//Whatever i do it doesn't work and i have no idea why
}
