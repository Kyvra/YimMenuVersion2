#include "OutfitEditor.hpp"
#include "core/frontend/manager/UIManager.hpp"
#include "game/backend/Self.hpp"
#include "game/frontend/items/Items.hpp"
#include "game/gta/Natives.hpp"
#include <random>
#include "game/backend/Outfit.hpp"
#include <nlohmann/json.hpp>

// TODO: clean up more AI generated junk from this file

namespace YimMenu
{
	static std::string GetOutfitPath()
	{
		const char* appdata = std::getenv("APPDATA");
		if (!appdata)
			return "";
		return std::string(appdata) + "\\YimMenuV2\\Outfits\\";
	}

	void SaveOutfit(int ped, const std::string& name)
	{
		if (ped == 0 || !ENTITY::DOES_ENTITY_EXIST(ped))
			return;

		std::string safeName = name;
		safeName.erase(
		    std::remove_if(
		        safeName.begin(),
		        safeName.end(),
		        [](char c) {
			        return c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|';
		        }),
		    safeName.end());

		if (safeName.empty())
			return;

		OutfitData outfit{};
		outfit.name = safeName;
		for (int i = 0; i < 12; i++)
		{
			OutfitComponent c{};
			c.component = i;
			c.drawable = PED::GET_PED_DRAWABLE_VARIATION(ped, i);
			c.texture = PED::GET_PED_TEXTURE_VARIATION(ped, i);
			c.palette = PED::GET_PED_PALETTE_VARIATION(ped, i);
			outfit.components.push_back(c);
		}
		for (int i = 0; i < 8; i++)
		{
			int drawable = PED::GET_PED_PROP_INDEX(ped, i, 0);
			if (drawable == -1)
				continue;

			OutfitProp p{};
			p.prop = i;
			p.drawable = drawable;
			p.texture = PED::GET_PED_PROP_TEXTURE_INDEX(ped, i);
			outfit.props.push_back(p);
		}
		json j;
		j["name"] = outfit.name;
		j["model"] = ENTITY::GET_ENTITY_MODEL(ped);
		j["components"] = json::array();
		j["props"] = json::array();

		for (auto& c : outfit.components)
		{
			j["components"].push_back({{"slot", c.component},
			    {"drawable", c.drawable},
			    {"texture", c.texture},
			    {"palette", c.palette}});
		}

		for (auto& p : outfit.props)
		{
			j["props"].push_back({{"slot", p.prop},
			    {"drawable", p.drawable},
			    {"texture", p.texture}});
		}
		const std::string folder = GetOutfitPath();
		if (folder.empty())
			return;

		try
		{
			std::filesystem::create_directories(folder);

			std::ofstream file(folder + safeName + ".json", std::ios::trunc);
			if (!file.is_open())
				return;

			file << j.dump(4);
			file.close();
		}
		catch (...)
		{
			//Can add a log here
			//LOG(INFO) << "";
			return;
		}
	}

	void LoadOutfit(int ped, const std::string& name)
	{
		if (ped == 0 || !ENTITY::DOES_ENTITY_EXIST(ped))
			return;

		const std::string filePath = GetOutfitPath() + name + ".json";

		std::ifstream file(filePath);
		if (!file.is_open())
			return;

		json j;
		file >> j;
		file.close();

		if (j.contains("model"))
		{
			Hash currentModel = ENTITY::GET_ENTITY_MODEL(ped);
			Hash savedModel = j["model"].get<Hash>();

			if (currentModel != savedModel)
				return;
		}

		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(ped);
		for (auto& c : j["components"])
		{
			PED::SET_PED_COMPONENT_VARIATION(
			    ped,
			    c["slot"],
			    c["drawable"],
			    c["texture"],
			    c["palette"]);
		}
		for (int i = 0; i < 8; i++)
			PED::CLEAR_PED_PROP(ped, i, 0);

		for (auto& p : j["props"])
		{
			PED::SET_PED_PROP_INDEX(
			    ped,
			    p["slot"],
			    p["drawable"],
			    p["texture"],
			    TRUE,
			    0);
		}
	}

	static std::vector<std::string> GetSavedOutfits()
	{
		std::vector<std::string> names;
		const std::string folder = GetOutfitPath();

		for (auto& e : std::filesystem::directory_iterator(folder))
		{
			if (e.path().extension() == ".json")
				names.push_back(e.path().stem().string());
		}

		return names;
	}

	int GetMaxDrawable(int slot)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot);
	}

	int GetMaxTexture(int slot, int drawable)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot, drawable);
	}

	int GetMaxPropDrawable(int slot)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot);
	}

	int GetMaxPropTexture(int slot, int drawable)
	{
		auto ped = Self::GetPed();
		return PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(static_cast<int>(ped.GetHandle()), slot, drawable);
	}

	void GetOutfitSlot(int slot, int& drawable, int& texture)
	{
		auto ped = Self::GetPed();
		drawable = PED::GET_PED_DRAWABLE_VARIATION(static_cast<int>(ped.GetHandle()), slot);
		texture = PED::GET_PED_TEXTURE_VARIATION(static_cast<int>(ped.GetHandle()), slot);
	}

	void GetPropSlot(int slot, int& drawable, int& texture)
	{
		auto ped = Self::GetPed();
		drawable = PED::GET_PED_PROP_INDEX(static_cast<int>(ped.GetHandle()), slot, 0);
		if (drawable == -1)
		{
			drawable = 0;
			texture = 0;
			return;
		}
		texture = PED::GET_PED_PROP_TEXTURE_INDEX(static_cast<int>(ped.GetHandle()), slot);
	}

	void SetOutfitSlot(int slot, int drawable, int texture)
	{
		auto ped = Self::GetPed();
		PED::SET_PED_COMPONENT_VARIATION(static_cast<int>(ped.GetHandle()), slot, drawable, texture, 0);
	}

	void SetPropSlot(int slot, int drawable, int texture)
	{
		auto ped = Self::GetPed();
		PED::SET_PED_PROP_INDEX(static_cast<int>(ped.GetHandle()), slot, drawable, texture, true, 0);
	}

	// Helper function for underlined text
	static void TextUnderlined(const char* text)
	{
		ImGui::Text("%s", text);
		ImVec2 min = ImGui::GetItemRectMin();
		ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(min, max, ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_Text]));
	}

	static void TextUnderlinedAt(const char* text, float y)
	{
		auto old_cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPosY(y);
		TextUnderlined(text);
		ImGui::SetCursorPos(old_cursor);
	}

	std::shared_ptr<Category> CreateOutfitsMenu()
	{
		auto category = std::make_shared<Category>("Outfit Editor");

		category->AddItem(std::make_shared<ImGuiItem>([] {
			if (!NativeInvoker::AreHandlersCached())
				return ImGui::TextDisabled("Natives not cached yet");



			auto ped = Self::GetPed();

			if (!ped)
				return ImGui::TextDisabled("Player ped not found");

			// Create two columns layout
			const float windowWidth = ImGui::GetContentRegionAvail().x;
			const float columnWidth = windowWidth * 0.5f;
			const float inputWidth = 120.0f; // Minimal width for number input

			ImGui::Columns(2, "OutfitColumns", false);
			ImGui::SetColumnWidth(0, columnWidth);

			// Components section (Left column)

			float header_y = ImGui::GetCursorPosY();

			TextUnderlined("Components");
			const struct
			{
				const char* name;
				int slot;
			} componentSlots[] = {{"Top", 11}, {"Undershirt", 8}, {"Legs", 4}, {"Feet", 6}, {"Accessories", 7}, {"Bags", 5}, {"Mask", 1}, {"Gloves", 3}, {"Decals", 10}, {"Armor", 9}};

			bool first_iter = true;
			for (const auto& component : componentSlots)
			{
				ImGui::PushID(component.slot);

				int drawable, texture;
				GetOutfitSlot(component.slot, drawable, texture);

				ImGui::Text("%s", component.name);
				ImGui::SameLine();

				ImGui::SetCursorPosX(columnWidth - inputWidth * 2 - 10);

				ImGui::PushItemWidth(inputWidth);
				if (first_iter)
					TextUnderlinedAt("Drawable", header_y);
				if (ImGui::InputInt("##{}drawable", &drawable))
				{
					drawable = std::clamp(drawable, 0, GetMaxDrawable(component.slot) - 1);
					SetOutfitSlot(component.slot, drawable, texture);
				}
				ImGui::SameLine();
				if (first_iter)
					TextUnderlinedAt("Texture", header_y); // TODO: this heading is slightly misaligned and I'm not sure why (caused by the above SameLine?)
				if (ImGui::InputInt("##{}texture", &texture))
				{
					texture = std::clamp(texture, 0, GetMaxTexture(component.slot, drawable) - 1);
					SetOutfitSlot(component.slot, drawable, texture);
				}
				ImGui::PopItemWidth();
				ImGui::PopID();

				first_iter = false;
			}

			// Props section (Right column)
			ImGui::NextColumn();
			TextUnderlined("Props");

			const struct
			{
				const char* name;
				int slot;
			} propSlots[] = {{"Hats", 0}, {"Glasses", 1}, {"Ears", 2}, {"Watches", 6}};

			first_iter = true;
			for (const auto& prop : propSlots)
			{
				ImGui::PushID(prop.slot);

				int drawable, texture;
				GetPropSlot(prop.slot, drawable, texture);

				ImGui::Text("%s", prop.name);
				ImGui::SameLine();

				ImGui::SetCursorPosX(columnWidth + (columnWidth - inputWidth * 2 - 10));

				ImGui::PushItemWidth(inputWidth);
				if (first_iter)
					TextUnderlinedAt("Drawable", header_y);
				if (ImGui::InputInt("##pdrawable", &drawable))
				{
					drawable = std::clamp(drawable, 0, GetMaxPropDrawable(prop.slot) - 1);
					SetPropSlot(prop.slot, drawable, texture);
				}
				ImGui::SameLine();
				if (first_iter)
					TextUnderlinedAt("Texture", header_y);
				if (ImGui::InputInt("##ptexture", &texture))
				{
					texture = std::clamp(texture, 0, GetMaxPropTexture(prop.slot, drawable) - 1);
					SetPropSlot(prop.slot, drawable, texture);
				}
				ImGui::PopItemWidth();
				ImGui::PopID();

				first_iter = false;
			}

			ImGui::Columns(1);

			if (ImGui::Button("Randomize Outfit"))
			{
				std::random_device rd;
				std::mt19937 gen(rd());

				// Randomize components
				for (int i = 0; i < 12; ++i)
				{
					int maxDrawable = GetMaxDrawable(i);
					if (maxDrawable > 0)
					{
						int drawable = std::uniform_int_distribution<>(0, maxDrawable - 1)(gen);
						int maxTexture = GetMaxTexture(i, drawable);
						int texture = maxTexture > 0 ? std::uniform_int_distribution<>(0, maxTexture - 1)(gen) : 0;
						SetOutfitSlot(i, drawable, texture);
					}
				}

				// Randomize props
				for (int i : {0, 1, 2, 6, 7})
				{
					int maxDrawable = GetMaxPropDrawable(i);
					if (maxDrawable > 0)
					{
						int drawable = std::uniform_int_distribution<>(0, maxDrawable - 1)(gen);
						int maxTexture = GetMaxPropTexture(i, drawable);
						int texture = maxTexture > 0 ? std::uniform_int_distribution<>(0, maxTexture - 1)(gen) : 0;
						SetPropSlot(i, drawable, texture);
					}
				}
			}
			ImGui::Separator();
			ImGui::Spacing();

			TextUnderlined("Outfit Files");
			static char outfitName[32] = "default";
			static std::vector<std::string> outfits = GetSavedOutfits();

			ImGui::InputText("Outfit Name", outfitName, IM_ARRAYSIZE(outfitName));

			if (ImGui::Button("Save Outfit"))
			{
				int ped = PLAYER::PLAYER_PED_ID();
				SaveOutfit(ped, outfitName);
				outfits = GetSavedOutfits();
			}

			ImGui::Separator();
			ImGui::Text("Saved Outfits:");

			for (auto& name : outfits)
			{
				if (ImGui::Selectable(name.c_str()))
				{
					int ped = PLAYER::PLAYER_PED_ID();
					LoadOutfit(ped, name);
				}
			}

		}));
		return category;
	}
}
