#include "Menu.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "core/commands/Commands.hpp"
#include "core/frontend/manager/UIManager.hpp"
#include "core/renderer/Renderer.hpp"
#include "core/backend/FiberPool.hpp"
#include "core/backend/ScriptMgr.hpp"
#include "game/frontend/fonts/Fonts.hpp"
#include "game/pointers/Pointers.hpp"
#include "submenus/Self.hpp"
#include "submenus/Vehicle.hpp"
#include "submenus/Teleport.hpp"
#include "submenus/Network.hpp"
#include "submenus/Players.hpp"
#include "submenus/Recovery.hpp"
#include "submenus/Settings.hpp"
#include "submenus/Debug.hpp"
#include "submenus/World.hpp"
#include "core/filemgr/FileMgr.hpp"
#include "core/memory/ModuleMgr.hpp"
#include "Onboarding.hpp"
#include "game/gta/Natives.hpp"
namespace YimMenu
{
	static bool g_ShowThemePicker = true;
	static int g_SelectedTheme = -1;
	void ApplyTheme(int theme)
	{
		auto& style = ImGui::GetStyle();
		if (theme == 0) // Gold Style
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4* colors = style.Colors;
			style.Alpha = 1.0f;
			style.WindowRounding = 8.0f;
			style.FrameRounding = 8.0f;
			style.PopupRounding = 0.0f;
			style.ScrollbarRounding = 9.0f;
			style.GrabRounding = 8.0f;
			style.ScrollbarSize = 14.0f;
			style.WindowPadding = ImVec2(8, 8);
			style.FramePadding = ImVec2(4, 3);
			style.ItemSpacing = ImVec2(8, 4);
			style.ItemInnerSpacing = ImVec2(4, 4);
			colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.55f, 0.65f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.0588f, 0.0588f, 0.0588f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.14f, 0.22f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.15f, 0.25f, 0.98f);
			colors[ImGuiCol_Border] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.85f, 0.70f, 0.25f, 0.15f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.18f, 0.30f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.18f, 0.28f, 0.45f, 1.00f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.35f, 0.55f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.22f, 0.35f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.20f, 0.35f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.12f, 0.20f, 0.75f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.18f, 0.30f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.12f, 0.20f, 0.80f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.95f, 0.88f, 0.65f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.65f, 0.50f, 0.15f, 0.80f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.65f, 0.50f, 0.15f, 0.60f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.85f, 0.70f, 0.25f, 0.80f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.95f, 0.88f, 0.65f, 1.00f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.65f, 0.50f, 0.15f, 0.40f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.85f, 0.70f, 0.25f, 0.80f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.22f, 0.35f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.65f, 0.50f, 0.15f, 1.00f);
			colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.18f, 0.30f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.35f, 0.55f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.95f, 0.88f, 0.65f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.85f, 0.70f, 0.25f, 1.00f);
			colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
			colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
			colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
			colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(0.85f, 0.70f, 0.25f, 0.35f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.85f, 0.35f, 1.00f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.95f, 0.88f, 0.65f, 1.00f);
		}
		else if (theme == 1) // Red, White, Blue
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4* colors = style.Colors;
			style.Alpha = 1.0f;
			style.WindowRounding = 8.0f;
			style.FrameRounding = 8.0f;
			style.PopupRounding = 0.0f;
			style.ScrollbarRounding = 9.0f;
			style.GrabRounding = 8.0f;
			style.ScrollbarSize = 14.0f;
			style.WindowPadding = ImVec2(8, 8);
			style.FramePadding = ImVec2(4, 3);
			style.ItemSpacing = ImVec2(8, 4);
			style.ItemInnerSpacing = ImVec2(4, 4);
			colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.0588f, 0.0588f, 0.0588f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.03f, 0.06f, 0.12f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.02f, 0.04f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.20f, 0.30f, 0.50f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.15f, 0.30f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.25f, 0.40f, 0.40f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.30f, 0.50f, 0.67f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.10f, 0.20f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.12f, 0.25f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.10f, 0.20f, 0.80f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.90f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.80f, 0.10f, 0.10f, 0.40f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.90f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.10f, 0.20f, 0.80f, 0.31f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.30f, 0.90f, 0.80f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.25f, 0.85f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_Tab] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.90f, 0.20f, 0.20f, 0.80f);
			colors[ImGuiCol_TabActive] = ImVec4(0.80f, 0.10f, 0.10f, 0.86f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.85f, 0.15f, 0.15f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.068f, 0.102f, 0.148f, 0.9724f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.136f, 0.262f, 0.424f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
			colors[ImGuiCol_TableHeaderBg] = ImVec4(0.90f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_TableBorderStrong] = ImVec4(0.10f, 0.20f, 0.80f, 1.00f);
			colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.30f, 0.90f, 1.00f);
			colors[ImGuiCol_TableRowBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
			colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		}
	}

    void DrawStartupThemePicker()
	{
		if (!g_ShowThemePicker)
			return;
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;
		io.WantCaptureMouse = true;
		io.WantCaptureKeyboard = true;
		const ImVec2 window_size(420.f, 220.f);
		ImVec2 center = ImVec2(
		    io.DisplaySize.x * 0.5f,
		    io.DisplaySize.y * 0.5f);
		ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

		ImGui::Begin(
		    "##ThemePicker",
		    nullptr,
		    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

		ImGui::Text("Choose Your New Year Theme");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		static const char* themes[] = {
		    "Gold Style",
		    "USA Style"};
		ImGui::SetNextItemWidth(-1);
		ImGui::Combo("Theme", &g_SelectedTheme, themes, IM_ARRAYSIZE(themes));
		ImGui::Spacing();
		ImGui::Spacing();
		if (g_SelectedTheme >= 0)
		{
			if (ImGui::Button("Apply Theme", ImVec2(-1.f, 40.f)))
			{
				ApplyTheme(g_SelectedTheme);
				g_ShowThemePicker = false;
			}
		}
		ImGui::End();
		PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
		PAD::DISABLE_ALL_CONTROL_ACTIONS(1);
		PAD::DISABLE_ALL_CONTROL_ACTIONS(2);
	}





	void Menu::Init()
	{
		// Arguably the only place this file should be edited at for more menus
		UIManager::AddSubmenu(std::make_shared<Submenus::Self>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Vehicle>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Teleport>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Network>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Players>());
		UIManager::AddSubmenu(std::make_shared<Submenus::World>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Recovery>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Settings>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Debug>());

		Renderer::AddRendererCallBack(
		    [&] {
			    ProcessOnboarding();
			    if (!GUI::IsOpen())
				    return;

			    ImGui::PushFont(Menu::Font::g_DefaultFont);
			    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImU32(ImColor(15, 15, 15)));
			    if (!g_ShowThemePicker && g_SelectedTheme >= 0)
			    {
				    UIManager::Draw();
			    }
			    ImGui::PopStyleColor();
			    ImGui::PopFont();
			    if (g_ShowThemePicker || g_SelectedTheme < 0)
			    {
				    DrawStartupThemePicker();
			    }
		    },
		    -1);
	}
	void Menu::SetupStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		
		style.GrabRounding = style.FrameRounding = style.ChildRounding = style.WindowRounding = 8.0f;
	}
	static const ImWchar* GetGlyphRangesCyrillicOnly()
	{
		static const ImWchar ranges[] =
		    {
		        0x0400,
		        0x052F, // Cyrillic + Cyrillic Supplement
		        0x2DE0,
		        0x2DFF, // Cyrillic Extended-A
		        0xA640,
		        0xA69F, // Cyrillic Extended-B
		        0,
		    };
		return &ranges[0];
	}

	static ImFont* CreateFontWithCyrillicSupport(ImGuiIO& io, float size)
	{
		ImFontConfig FontCfg{};
		FontCfg.FontDataOwnedByAtlas = false;

		auto font = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::MainFont), sizeof(Fonts::MainFont), size, &FontCfg, io.Fonts->GetGlyphRangesDefault());

		// just use Arial for Cyrillic

		FontCfg.MergeMode = true;
		io.Fonts->AddFontFromFileTTF((std::filesystem::path(std::getenv("SYSTEMROOT")) / "Fonts" / "arial.ttf").string().c_str(), size, &FontCfg, GetGlyphRangesCyrillicOnly());

		io.Fonts->Build();

		return font;
	}

	void Menu::SetupFonts()
	{
		auto& IO = ImGui::GetIO();
		auto file_path = std::filesystem::path(std::getenv("appdata")) / "YimMenuV2" / "imgui.ini";
		static auto path = file_path.string();
		IO.IniFilename = path.c_str();
		IO.LogFilename = NULL;
		ImFontConfig FontCfg{};
		FontCfg.FontDataOwnedByAtlas = false;

		IO.Fonts->Clear();
		Menu::Font::g_DefaultFont = CreateFontWithCyrillicSupport(IO, Menu::Font::g_DefaultFontSize);
		Menu::Font::g_OptionsFont = CreateFontWithCyrillicSupport(IO, Menu::Font::g_OptionsFontSize);
		Menu::Font::g_ChildTitleFont = CreateFontWithCyrillicSupport(IO, Menu::Font::g_ChildTitleFontSize);
		Menu::Font::g_ChatFont = CreateFontWithCyrillicSupport(IO, Menu::Font::g_ChatFontSize);
		Menu::Font::g_OverlayFont = CreateFontWithCyrillicSupport(IO, Menu::Font::g_OverlayFontSize);
		static const ImWchar full_range[] = {0x0020, 0xFFFF, 0};
		Menu::Font::g_AwesomeFont = IO.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::IconFont), sizeof(Fonts::IconFont), Menu::Font::g_AwesomeFontSize, &FontCfg, full_range);

		UIManager::SetOptionsFont(Menu::Font::g_OptionsFont);
		Renderer::SetFontsUpdated();
	}
}
