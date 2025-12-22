#include "imgui.h"
#include "imgui_internal.h"
#include "common.hpp"

namespace YimMenu
{
	extern void theme_editor();
	extern std::vector<std::filesystem::path> GetJsonThemes();
	extern void LoadTheme(const std::filesystem::path& file);
	extern void SaveTheme(const std::filesystem::path& file);
	extern std::filesystem::path GetThemesPath();

}
