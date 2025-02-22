#include "SettingsUI.h"

SettingsUI::SettingsUI()
{
	Refresh();
}

void SettingsUI::Refresh()
{
	for (int i = 0; i < 118; i++)
	{
		float base[3] = { 0, 1, 0 };
		elementColours.push_back(base);
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
			ImGui::ColorEdit3("Bonds", bondColour);
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

	



	ImGui::End();
}

void SettingsUI::drawElementList()
{
	for (int i = 0; i < elementColours.size(); i++)
	{
		ImGui::ColorEdit3(std::to_string(i).c_str(), elementColours[i]);
	}
}