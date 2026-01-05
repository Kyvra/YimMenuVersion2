#include "Weapons.hpp"
#include "core/backend/FiberPool.hpp"
#include "core/backend/ScriptMgr.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/data/Weapons.hpp"
#include "game/gta/Natives.hpp"
#include "game/gta/Scripts.hpp"
#include "game/gta/ScriptFunction.hpp"
#include "types/script/scrThread.hpp"
#include "core/commands/Commands.hpp"
#include "game/features/self/CustomWeapon.hpp"

namespace YimMenu::Submenus
{
	struct WeaponDisplay
	{
		std::string name;
		std::string desc;
		joaat_t hash;
	};

	static void FetchWeaponStats(joaat_t weaponHash, int& kills, int& deaths, float& kd, int& headshots, int& accuracy)
	{
		uint64_t garbage[4]{};
		if (auto id = Scripts::StartScript("mp_weapons"_J, eStackSizes::PAUSE_MENU_SCRIPT, &garbage, 4))
		{
			if (auto thread = Scripts::FindScriptThreadByID(id))
			{
				thread->m_Context.m_State = rage::scrThread::State::PAUSED;

				static ScriptFunction getWeaponKills("mp_weapons"_J, ScriptPointer("GetWeaponKills", "5D ? ? ? 39 0F 38 00").Add(1).Rip());
				static ScriptFunction getWeaponDeaths("mp_weapons"_J, ScriptPointer("GetWeaponDeaths", "5D ? ? ? 39 10").Add(1).Rip());
				static ScriptFunction getWeaponKDRatio("mp_weapons"_J, ScriptPointer("GetWeaponKDRatio", "5D ? ? ? 39 12").Add(1).Rip());
				static ScriptFunction getWeaponHeadshots("mp_weapons"_J, ScriptPointer("GetWeaponHeadshots", "5D ? ? ? 39 11").Add(1).Rip());
				static ScriptFunction getWeaponAccuracy("mp_weapons"_J, ScriptPointer("GetWeaponAccuracy", "2D 01 09 00 00"));

				kills     = getWeaponKills.Call(weaponHash, -1);
				deaths    = getWeaponDeaths.Call(weaponHash, -1);
				kd        = getWeaponKDRatio.Call(weaponHash, -1);
				headshots = getWeaponHeadshots.Call(weaponHash, -1);
				accuracy  = static_cast(getWeaponAccuracy.Call(weaponHash));

				thread->Kill();
				thread->m_Context.m_State = rage::scrThread::State::KILLED;
			}
		}
	}

	static void RenderAmmuNationMenu()
	{
		static std::vector weaponDisplays;
		static std::string selectedWeapon{"Select"};
		static joaat_t selectedWeaponHash{};
		static char searchWeapon[64];

		static int kills{};
		static int deaths{};
		static float kdRatio{};
		static int headshots{};
		static int accuracy{};

		static bool init = [] {
			FiberPool::Push([] {
				while (Scripts::IsScriptActive("startup"_J))
					ScriptMgr::Yield();

				uint64_t garbage[4]{};
				if (auto id = Scripts::StartScript("mp_weapons"_J, eStackSizes::PAUSE_MENU_SCRIPT, &garbage, 4))
				{
					if (auto thread = Scripts::FindScriptThreadByID(id))
					{
						thread->m_Context.m_State = rage::scrThread::State::PAUSED;

						for (const auto& weap : g_WeaponHashes)
						{
							static ScriptFunction getWeaponNameLabel("mp_weapons"_J, ScriptPointer("GetWeaponNameLabel", "2D 02 2B 00 00"));
							static ScriptFunction getWeaponDescLabel("mp_weapons"_J, ScriptPointer("GetWeaponDescLabel", "2D 02 A0 00 00"));

							std::string nameGxt = getWeaponNameLabel.Call(weap, false); // second arg is for uppercase
							std::string descGxt = getWeaponDescLabel.Call(weap, false);

							std::string nameDisplay = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(nameGxt.c_str());
							std::string descDisplay = HUD::GET_FILENAME_FOR_AUDIO_CONVERSATION(descGxt.c_str());

							weaponDisplays.push_back({((nameDisplay.empty() || nameDisplay == "NULL" || nameDisplay == "Invalid") ? "" : nameDisplay), ((descDisplay.empty() || descDisplay == "NULL" || descDisplay == "Invalid") ? "" : descDisplay), weap});
						}

						thread->Kill();
						thread->m_Context.m_State = rage::scrThread::State::KILLED;
					}
				}
			});
			return true;
		}();

		ImGui::BeginCombo("Weapons", selectedWeapon.c_str());
		if (ImGui::IsItemActive() && !ImGui::IsPopupOpen("##weaponspopup"))
		{
			ImGui::OpenPopup("##weaponspopup");
			memset(searchWeapon, 0, sizeof(searchWeapon));
		}
		if (ImGui::BeginPopup("##weaponspopup", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::Text("Search:");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(250.f);
			ImGui::InputText("##searchweapon", searchWeapon, sizeof(searchWeapon));

			std::string searchLower = searchWeapon;
			std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
			for (const auto& weap : weaponDisplays)
			{
				if (weap.name.empty())
					continue;

				std::string weaponLower = weap.name;
				std::transform(weaponLower.begin(), weaponLower.end(), weaponLower.begin(), ::tolower);

				if (weaponLower.find(searchLower) != std::string::npos)
				{
					ImGui::PushID(weap.hash);
					if (ImGui::Selectable(weap.name.c_str()))
					{
						FiberPool::Push([weap] {
							selectedWeapon = weap.name;
							selectedWeaponHash = weap.hash;
							FetchWeaponStats(selectedWeaponHash, kills, deaths, kdRatio, headshots, accuracy);
						});
					}
					ImGui::PopID();
					if (ImGui::IsItemHovered() && !weap.desc.empty())
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35);
						ImGui::TextUnformatted(weap.desc.c_str());
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}
				}
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Give Weapon"))
		{
			FiberPool::Push([] {
				Self::GetPed().GiveWeapon(selectedWeaponHash, true);
			});
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Weapon"))
		{
			FiberPool::Push([] {
				Self::GetPed().RemoveWeapon(selectedWeaponHash);
			});
		}

		if (*Pointers.IsSessionStarted && selectedWeaponHash != 0)
		{
			ImGui::Text("Kills With: %d", kills);
			ImGui::Text("Deaths By: %d", deaths);
			ImGui::Text("K/D Ratio: %.2f", kdRatio);
			ImGui::Text("Headshots: %d", headshots);
			ImGui::Text("Accuracy: %d%%", accuracy);
		}
	}

	static std::shared_ptr RenderCustomWeaponsMenu()
	{
		auto customWeaponsGroup = std::make_shared("Custom Weapons");

		auto cutomWeaponTypes = std::make_shared("", 1);
		auto customWeapons = std::make_shared("");
		auto paintGunGroup = std::make_shared("");

		auto cmd = Commands::GetCommand("customweapontype"_J);
		
		auto isGravityGunEnabled = [cmd] {
			return static_cast(cmd->GetState()) == Features::CustomWeapons::GRAVITY_GUN;
		};

		auto isVehicleGunEnabled = [cmd] {
			return static_cast(cmd->GetState()) == Features::CustomWeapons::VEHICLE_GUN;
		};

		auto isPaintGunEnabled = [cmd] {
			return static_cast(cmd->GetState()) == Features::CustomWeapons::PAINT_GUN;
		};

		cutomWeaponTypes->AddItem(std::make_shared("customweapontype"_J));
		cutomWeaponTypes->AddItem(std::make_shared(isGravityGunEnabled, std::make_shared("gravitygunlaunchonrelease"_J)));
		cutomWeaponTypes->AddItem(std::make_shared(isVehicleGunEnabled, std::make_shared("vehiclegunmodel"_J)));
		cutomWeaponTypes->AddItem(std::make_shared(isPaintGunEnabled, std::make_shared("paintgunrainbowcolorenabled"_J, std::make_shared("paintguncolor"_J), true)));

		paintGunGroup->AddItem(std::make_shared("paintgunrainbowcolorenabled"_J));
		paintGunGroup->AddItem(std::make_shared("paintgunrainbowcolorenabled"_J, std::make_shared("paintgunrainbowcolorstyle"_J)));
		paintGunGroup->AddItem(std::make_shared("paintgunrainbowcolorenabled"_J, std::make_shared("paintgunrainbowcolorspeed"_J)));

		customWeapons->AddItem(std::make_shared("customweaponenabledonweaponout"_J));
		customWeapons->AddItem(std::move(cutomWeaponTypes));
		customWeapons->AddItem(std::make_shared(isPaintGunEnabled, std::move(paintGunGroup)));

		customWeaponsGroup->AddItem(std::make_shared("customweapon"_J));
		customWeaponsGroup->AddItem(std::make_shared("customweapon"_J, std::move(customWeapons)));

		return customWeaponsGroup;
	}

	std::shared_ptr BuildWeaponsMenu()
	{
		auto weapons = std::make_shared("Weapons");

		auto weaponsGlobalsGroup = std::make_shared("Globals", 12);
		auto weaponsToolsGroup = std::make_shared("Tools", 1);
		auto weaponsAmmuNationGroup = std::make_shared("Ammu-Nation");
		auto weaponsAimbotGroup = std::make_shared("Aimbot", 1);

		weaponsGlobalsGroup->AddItem(std::make_shared("infiniteammo"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("infiniteclip"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("rapidfire"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("infiniteparachutes"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("ExplosiveAmmo"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("ExplosiveAmmo"_J, std::make_shared("selectedexplosion"_J)));
		weaponsGlobalsGroup->AddItem(std::make_shared("ExplosiveAmmo"_J, std::make_shared("explosiondamage"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared("ExplosiveAmmo"_J, std::make_shared("explosioncamerashake"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared("weapondamage"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("weapondamage"_J, std::make_shared("weapondamagescale"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared("meleedamage"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("meleedamage"_J, std::make_shared("meleedamagescale"_J, std::nullopt, false)));
		weaponsGlobalsGroup->AddItem(std::make_shared("explosionradius"_J));
		weaponsGlobalsGroup->AddItem(std::make_shared("explosionradius"_J, std::make_shared("explosionradiusscale"_J, std::nullopt, false)));

		weaponsToolsGroup->AddItem(std::make_shared("giveallweapons"_J));
		weaponsToolsGroup->AddItem(std::make_shared("givemaxammo"_J));
		weaponsToolsGroup->AddItem(std::make_shared("opengunlocker"_J));

		weaponsAmmuNationGroup->AddItem(std::make_shared([] {
			RenderAmmuNationMenu();
		}));

		weaponsAimbotGroup->AddItem(std::make_shared("aimbot"_J));
		weaponsAimbotGroup->AddItem(std::make_shared("aimbot"_J, std::make_shared("aimbotaimforhead"_J)));
		weaponsAimbotGroup->AddItem(std::make_shared("aimbot"_J, std::make_shared("aimbottargetdrivers"_J)));
		weaponsAimbotGroup->AddItem(std::make_shared("aimbot"_J, std::make_shared("aimbotreleasedeadped"_J)));

		weapons->AddItem(weaponsGlobalsGroup);
		weapons->AddItem(weaponsToolsGroup);
		weapons->AddItem(weaponsAmmuNationGroup);
		weapons->AddItem(weaponsAimbotGroup);
		weapons->AddItem(RenderCustomWeaponsMenu());
		return weapons;
	}
}
