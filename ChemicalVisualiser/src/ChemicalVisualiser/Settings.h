#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <Slate/Json.h>
#include <Slate/Math.h>

namespace nlohmann {
	template <>
	struct adl_serializer<glm::u8vec3> {
		static void to_json(json& j, const glm::u8vec3& v) {
			j = json::array({ v.r, v.g, v.b });
		}

		static void from_json(const json& j, glm::u8vec3& v) {
			v = glm::u8vec3{
				j.at(0).get<uint8_t>(),
				j.at(1).get<uint8_t>(),
				j.at(2).get<uint8_t>()
			};
		}
	};
}

namespace ChemicalVisualiser
{
	const uint8_t ElementCount = 118;

	struct SettingValues
	{
		// General Settings
		float FontSize = 1.0f;
		bool DarkMode = true;
		int TargetFPS = 60;
		bool DynamicFramerate = true;

		// 2D Visualisation Settings
		float AtomScale2D = 1.0f;
		float HydrogenScale2D = 0.5f;
		float BondWidth2D = 1.0f;
		float BondSeperation2D = 1.0f;
		int WorldScale2D = 1000;
		bool ShowElementLabels = true;
		float LabelScale = 0.25f;
		bool Background2D = false;
		glm::u8vec3 BackgroundColor2D = { 15, 15, 15 };
		float CameraSmoothing2D = 0.5f;

		// 3D Visualisation Settings
		float AtomScale3D = 1.0f;
		float HydrogenScale3D = 0.5f;
		float BondRadius3D = 1.0f;
		float BondSeperation3D = 1.0f;
		float BondDetail3D = 1.0f;
		bool Background3D = false;
		glm::u8vec3 BackgroundColor3D = { 15, 15, 15 };
		float LookSensitivity3D = 1.0f;
		float PanSensitivity3D = 1.0f;
		float CameraSmoothing3D = 0.8f;

		// Element Colours

		// RGB for each element by atomic number
		std::vector<glm::u8vec3> ElementColors = {
			glm::u8vec3(255, 255, 255),
			glm::u8vec3(204, 128, 255),
			glm::u8vec3(217, 255, 255),
			glm::u8vec3(194, 255, 0),
			glm::u8vec3(255, 181, 181),
			glm::u8vec3(144, 144, 144),
			glm::u8vec3(48, 80, 248),
			glm::u8vec3(255, 13, 13),
			glm::u8vec3(144, 224, 80),
			glm::u8vec3(224, 255, 255),
			glm::u8vec3(171, 92, 242),
			glm::u8vec3(138, 255, 0),
			glm::u8vec3(191, 166, 166),
			glm::u8vec3(240, 200, 160),
			glm::u8vec3(255, 128, 0),
			glm::u8vec3(255, 255, 48),
			glm::u8vec3(31, 240, 31),
			glm::u8vec3(128, 209, 227),
			glm::u8vec3(143, 64, 212),
			glm::u8vec3(61, 255, 0),
			glm::u8vec3(230, 230, 230),
			glm::u8vec3(191, 194, 199),
			glm::u8vec3(166, 166, 171),
			glm::u8vec3(138, 153, 199),
			glm::u8vec3(156, 122, 199),
			glm::u8vec3(224, 102, 51),
			glm::u8vec3(240, 144, 160),
			glm::u8vec3(80, 208, 80),
			glm::u8vec3(200, 128, 51),
			glm::u8vec3(125, 128, 176),
			glm::u8vec3(194, 143, 143),
			glm::u8vec3(102, 143, 143),
			glm::u8vec3(189, 128, 227),
			glm::u8vec3(255, 161, 0),
			glm::u8vec3(166, 41, 41),
			glm::u8vec3(92, 184, 209),
			glm::u8vec3(112, 46, 176),
			glm::u8vec3(0, 255, 0),
			glm::u8vec3(148, 255, 255),
			glm::u8vec3(148, 224, 224),
			glm::u8vec3(115, 194, 201),
			glm::u8vec3(84, 181, 181),
			glm::u8vec3(59, 158, 158),
			glm::u8vec3(36, 143, 143),
			glm::u8vec3(10, 125, 140),
			glm::u8vec3(0, 105, 133),
			glm::u8vec3(192, 192, 192),
			glm::u8vec3(255, 217, 143),
			glm::u8vec3(166, 117, 115),
			glm::u8vec3(102, 128, 128),
			glm::u8vec3(158, 99, 181),
			glm::u8vec3(212, 122, 0),
			glm::u8vec3(148, 0, 148),
			glm::u8vec3(66, 158, 176),
			glm::u8vec3(87, 23, 143),
			glm::u8vec3(0, 201, 0),
			glm::u8vec3(112, 212, 255),
			glm::u8vec3(255, 255, 199),
			glm::u8vec3(217, 255, 199),
			glm::u8vec3(199, 255, 199),
			glm::u8vec3(163, 255, 199),
			glm::u8vec3(143, 255, 199),
			glm::u8vec3(97, 255, 199),
			glm::u8vec3(69, 255, 199),
			glm::u8vec3(48, 255, 199),
			glm::u8vec3(31, 255, 199),
			glm::u8vec3(0, 255, 156),
			glm::u8vec3(0, 230, 117),
			glm::u8vec3(0, 212, 82),
			glm::u8vec3(0, 191, 56),
			glm::u8vec3(0, 171, 36),
			glm::u8vec3(77, 194, 255),
			glm::u8vec3(77, 166, 255),
			glm::u8vec3(33, 148, 214),
			glm::u8vec3(38, 125, 171),
			glm::u8vec3(38, 102, 150),
			glm::u8vec3(23, 84, 135),
			glm::u8vec3(208, 208, 224),
			glm::u8vec3(255, 209, 35),
			glm::u8vec3(184, 184, 208),
			glm::u8vec3(166, 84, 77),
			glm::u8vec3(87, 89, 97),
			glm::u8vec3(158, 79, 181),
			glm::u8vec3(171, 92, 0),
			glm::u8vec3(117, 79, 69),
			glm::u8vec3(66, 130, 150),
			glm::u8vec3(66, 0, 102),
			glm::u8vec3(0, 125, 0),
			glm::u8vec3(112, 171, 250),
			glm::u8vec3(0, 186, 255),
			glm::u8vec3(0, 161, 255),
			glm::u8vec3(0, 143, 255),
			glm::u8vec3(0, 128, 255),
			glm::u8vec3(0, 107, 255),
			glm::u8vec3(84, 92, 242),
			glm::u8vec3(120, 92, 227),
			glm::u8vec3(138, 79, 227),
			glm::u8vec3(161, 54, 212),
			glm::u8vec3(179, 31, 212),
			glm::u8vec3(179, 31, 186),
			glm::u8vec3(179, 13, 166),
			glm::u8vec3(189, 13, 135),
			glm::u8vec3(199, 0, 102),
			glm::u8vec3(204, 0, 89),
			glm::u8vec3(209, 0, 79),
			glm::u8vec3(217, 0, 69),
			glm::u8vec3(224, 0, 56),
			glm::u8vec3(230, 0, 46),
			glm::u8vec3(235, 0, 38),
			glm::u8vec3(235, 0, 0),
			glm::u8vec3(225, 0, 0),
			glm::u8vec3(215, 0, 0),
			glm::u8vec3(205, 0, 0),
			glm::u8vec3(195, 0, 0),
			glm::u8vec3(185, 0, 0),
			glm::u8vec3(175, 0, 0),
			glm::u8vec3(165, 0, 0),
			glm::u8vec3(155, 0, 0),
		};
	};

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(SettingValues,
		FontSize, DarkMode, TargetFPS, DynamicFramerate, AtomScale2D, HydrogenScale2D, BondWidth2D, BondSeperation2D,
		WorldScale2D, ShowElementLabels, LabelScale, Background2D, BackgroundColor2D, CameraSmoothing2D, AtomScale3D,
		HydrogenScale3D, BondRadius3D, BondSeperation3D, BondDetail3D, Background3D, BackgroundColor3D, LookSensitivity3D,
		PanSensitivity3D, CameraSmoothing3D, ElementColors
	);


	class Settings
	{
	public:
		Settings();

		SettingValues& Values() { return m_Values; }
		std::string Dump() const;

		void Save();
		void QueueRevert();
		void Revert();

		void Reset();
		void SaveToDisk();

		void MakeChange() { m_Changed = true; }
		bool isSaveQueued() const { return m_SaveQueued; }
		bool isRevertQueued() const { return m_RevertQueued; }
		bool HasChanged() const { return m_Changed; }

	private:
		void ResetToDefaults();
		void ReadFromDisk();
		void ParseFromString(std::string data);

	private:
		const std::filesystem::path m_Path = "Cache/Settings.cfg";

		bool m_SaveQueued = false;
		bool m_RevertQueued = false;

		bool m_Changed = false;
		SettingValues m_Values;

	};
}