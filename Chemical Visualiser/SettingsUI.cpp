#include "SettingsUI.h"

SettingsUI::SettingsUI()
{
	
}

void SettingsUI::Init()
{
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		elementColours[i] = elementData[i].data();
	}
	bondPointer = bondColour.data();
}

void SettingsUI::Refresh()
{
	// This is really inefficient
	// it goes over the same elements like 117 times, unnecessarily
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		glm::vec3 elementColour = std::get<glm::vec3>(Settings::GetValue("element_" + std::to_string(i + 1) + "_colour").data);
		for (int j = 0; j < 3; j++)
		{
			elementData[i][j] = elementColour[j];
		}
	}
}

void SettingsUI::Draw()
{
	ImGui::Begin("Settings");

	// Accessibility
	if (ImGui::TreeNode("Accessibility"))
	{
		ImGui::TreePop();
	}

	// Camera Settings
	if (ImGui::TreeNode("Camera Settings"))
	{
		ImGui::TreePop();
	}


	// Visualiser Settings
	//     Colours
	if (ImGui::TreeNode("Visualiser Tweaks"))
	{
		if (ImGui::TreeNode("Colours"))
		{
			if (ImGui::TreeNode("Elements"))
			{
				drawElementList();
				ImGui::TreePop();
			}
			
			ImGui::ColorEdit3("Bonds", bondPointer);

			ImGui::TreePop();
		}


		ImGui::TreePop();
	}


	// Controls
	//     Keyboard + mouse
	if (ImGui::TreeNode("Controls"))
	{
		if (ImGui::TreeNode("Keyboard and Mouse"))
		{
			ImGui::TreePop();
		}


		ImGui::TreePop();
	}

	

	if (ImGui::Button("Save"))
	{
		Save();
	}

	ImGui::End();
}

void SettingsUI::Save()
{
	for (int i = 0; i < NUM_ELEMENTS; i++)
	{
		glm::vec3 val = toVec3(elementColours[i]);
		Settings::Set("element_" + std::to_string(i + 1) + "_colour", val);
	}
	Settings::Save();
}


void SettingsUI::drawElementList()
{
	for (int i = 0; i < elementColours.size(); i++)
	{
		ImGui::ColorEdit3(std::to_string(i + 1).c_str(), elementColours[i]);
	}
}

glm::vec3 SettingsUI::toVec3(float* col)
{
	glm::vec3 vec = 
	{
		col[0],
		col[1],
		col[2]
	};
	return vec;
}