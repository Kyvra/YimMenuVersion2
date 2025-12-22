#include "theme.h"
namespace YimMenu
{
	bool DrawColorWheel(const char* id, ImVec4& color, float radius = 60.f)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (!window || window->SkipItems)
			return false;
		ImDrawList* draw = window->DrawList;
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size(radius * 2.f, radius * 2.f);
		ImVec2 center = pos + ImVec2(radius, radius);
		ImGui::InvisibleButton(id, size);
		bool hovered = ImGui::IsItemHovered();
		bool active = ImGui::IsItemActive();
		const int segments = 96;
		for (int i = 0; i < segments; i++)
		{
			float a0 = (i / (float)segments) * IM_PI * 2.f;
			float a1 = ((i + 1) / (float)segments) * IM_PI * 2.f;
			ImU32 col = ImColor::HSV(i / (float)segments, 1.f, 1.f);
			draw->AddTriangleFilled(
			    center,
			    center + ImVec2(cosf(a0), sinf(a0)) * radius,
			    center + ImVec2(cosf(a1), sinf(a1)) * radius,
			    col);
		}
		draw->AddCircle(center, radius, IM_COL32(255, 255, 255, hovered ? 180 : 110), 0, 1.5f);
		if (active)
		{
			ImVec2 m = ImGui::GetIO().MousePos - center;
			float dist = sqrtf(m.x * m.x + m.y * m.y);
			if (dist <= radius)
			{
				float h = atan2f(m.y, m.x) / (IM_PI * 2.f);
				if (h < 0.f)
					h += 1.f;

				ImGui::ColorConvertHSVtoRGB(h, 1.f, 1.f, color.x, color.y, color.z);
			}
		}
		return active;
	}
	static std::filesystem::path GetThemesPath()
	{
		const char* appdata = std::getenv("APPDATA");
		if (!appdata)
			return {};
		std::filesystem::path path = appdata;
		path /= "YimMenuV2";
		path /= "Themes";
		std::filesystem::create_directories(path);
		return path;
	}
	void SaveTheme(const std::filesystem::path& file)
	{
		ImGuiStyle& s = ImGui::GetStyle();
		std::ofstream out(file, std::ios::trunc);
		if (!out.is_open())
			return;
		out << "{\n";
		out << "  \"style\": {\n";
		out << "    \"alpha\": " << s.Alpha << ",\n";
		out << "    \"window_rounding\": " << s.WindowRounding << ",\n";
		out << "    \"frame_rounding\": " << s.FrameRounding << ",\n";
		out << "    \"popup_rounding\": " << s.PopupRounding << ",\n";
		out << "    \"scrollbar_rounding\": " << s.ScrollbarRounding << ",\n";
		out << "    \"grab_rounding\": " << s.GrabRounding << ",\n";
		out << "    \"scrollbar_size\": " << s.ScrollbarSize << ",\n";
		out << "    \"window_padding\": [" << s.WindowPadding.x << ", " << s.WindowPadding.y << "],\n";
		out << "    \"frame_padding\": [" << s.FramePadding.x << ", " << s.FramePadding.y << "],\n";
		out << "    \"item_spacing\": [" << s.ItemSpacing.x << ", " << s.ItemSpacing.y << "],\n";
		out << "    \"item_inner_spacing\": [" << s.ItemInnerSpacing.x << ", " << s.ItemInnerSpacing.y << "]\n";
		out << "  },\n";
		out << "  \"colors\": [\n";
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			ImVec4& c = s.Colors[i];
			out << "    [" << c.x << ", " << c.y << ", " << c.z << ", " << c.w << "]";
			if (i != ImGuiCol_COUNT - 1)
				out << ",";
			out << "\n";
		}
		out << "  ]\n";
		out << "}\n";
	}
	void LoadTheme(const std::filesystem::path& file)
	{
		ImGuiStyle& s = ImGui::GetStyle();
		std::ifstream in(file);
		if (!in.is_open())
			return;
		std::string data((std::istreambuf_iterator<char>(in)),
		    std::istreambuf_iterator<char>());
		std::vector<float> values;
		values.reserve(256);
		const char* p = data.c_str();
		char* end = nullptr;
		while (*p)
		{
			if ((*p >= '0' && *p <= '9') || *p == '-' || *p == '.')
			{
				float v = strtof(p, &end);
				if (p != end)
				{
					values.push_back(v);
					p = end;
					continue;
				}
			}
			++p;
		}
		const size_t needed =
		    1 + 6 + 8 + (ImGuiCol_COUNT * 4);
		if (values.size() < needed)
		{
			return;
		}
		size_t i = 0;
		s.Alpha = values[i++];
		s.WindowRounding = values[i++];
		s.FrameRounding = values[i++];
		s.PopupRounding = values[i++];
		s.ScrollbarRounding = values[i++];
		s.GrabRounding = values[i++];
		s.ScrollbarSize = values[i++];
		s.WindowPadding = {values[i++], values[i++]};
		s.FramePadding = {values[i++], values[i++]};
		s.ItemSpacing = {values[i++], values[i++]};
		s.ItemInnerSpacing = {values[i++], values[i++]};
		for (int c = 0; c < ImGuiCol_COUNT; c++)
		{
			s.Colors[c] = {
			    values[i++],
			    values[i++],
			    values[i++],
			    values[i++]};
		}
	}
	static std::vector<std::filesystem::path> GetJsonThemes()
	{
		std::vector<std::filesystem::path> out;
		for (auto& e : std::filesystem::directory_iterator(GetThemesPath()))
		{
			if (e.is_regular_file() && e.path().extension() == ".json")
				out.push_back(e.path());
		}
		return out;
	}
	void theme_editor()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;
		static int editing = -1;
		static bool open_color_popup = false;
		ImGui::BeginChild("ThemeEditor", ImVec2(0, 0), true);
		if (ImGui::CollapsingHeader("Style", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SliderFloat("Alpha", &style.Alpha, 0.20f, 1.00f, "%.2f");
			ImGui::SliderFloat("Window Rounding", &style.WindowRounding, 0.0f, 16.0f);
			ImGui::SliderFloat("Frame Rounding", &style.FrameRounding, 0.0f, 16.0f);
			ImGui::SliderFloat("Popup Rounding", &style.PopupRounding, 0.0f, 16.0f);
			ImGui::SliderFloat("Scrollbar Rounding", &style.ScrollbarRounding, 0.0f, 16.0f);
			ImGui::SliderFloat("Grab Rounding", &style.GrabRounding, 0.0f, 16.0f);
			ImGui::SliderFloat2("Window Padding", (float*)&style.WindowPadding, 0.0f, 20.0f);
			ImGui::SliderFloat2("Frame Padding", (float*)&style.FramePadding, 0.0f, 20.0f);
			ImGui::SliderFloat2("Item Spacing", (float*)&style.ItemSpacing, 0.0f, 20.0f);
			ImGui::SliderFloat2("Item Inner Spacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f);
			ImGui::SliderFloat("Scrollbar Size", &style.ScrollbarSize, 6.0f, 24.0f);
		}
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Colors", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static ImGuiTextFilter filter;
			filter.Draw("Filter colors");
			ImGui::Spacing();
			ImGui::BeginChild("##color_list", ImVec2(0, 260.f), true);
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName(i);
				if (!filter.PassFilter(name))
					continue;
				ImGui::PushID(i);
				ImGui::Selectable(name, false);
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
				{
					editing = i;
					open_color_popup = true;
				}
				ImGui::SameLine(220);
				ImGui::ColorButton("##preview", colors[i], ImGuiColorEditFlags_NoTooltip, ImVec2(28, 14));
				ImGui::PopID();
			}
			ImGui::EndChild();
		}
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Presets", ImGuiTreeNodeFlags_DefaultOpen))
		{
			static char theme_name[64] = "MyTheme";
			static int selected_theme = -1;
			if (ImGui::Button("Dark"))
				ImGui::StyleColorsDark();
			ImGui::SameLine();
			if (ImGui::Button("Classic"))
				ImGui::StyleColorsClassic();
			ImGui::SameLine();
			if (ImGui::Button("Light"))
				ImGui::StyleColorsLight();
			ImGui::Separator();
			ImGui::InputText("Theme Name", theme_name, IM_ARRAYSIZE(theme_name));
			if (ImGui::Button("Save Theme"))
			{
				auto path = GetThemesPath() / (std::string(theme_name) + ".json");
				SaveTheme(path);
			}
			ImGui::Separator();
			ImGui::TextUnformatted("Available Themes:");
			auto themes = GetJsonThemes();
			ImGui::BeginChild("##themes", ImVec2(0, 150.f), true);
			for (int i = 0; i < (int)themes.size(); i++)
			{
				std::string name = themes[i].stem().string();
				if (ImGui::Selectable(name.c_str(), selected_theme == i))
					selected_theme = i;
			}
			ImGui::EndChild();
			if (selected_theme >= 0 && selected_theme < (int)themes.size())
			{
				if (ImGui::Button("Load Selected"))
					LoadTheme(themes[selected_theme]);
			}
		}
		if (open_color_popup && editing != -1)
		{
			ImGui::OpenPopup("Color Wheel");
			open_color_popup = false;
			ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
			ImGui::SetNextWindowSize(ImVec2(300.f, 380.f), ImGuiCond_Appearing);
		}
		if (ImGui::BeginPopup("Color Wheel",
		  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			if (editing >= 0 && editing < ImGuiCol_COUNT)
			{
				ImGui::TextUnformatted(ImGui::GetStyleColorName(editing));
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::PushID(editing);
				float radius = 70.f;
				float wheel_w = radius * 2.f;
				float avail = ImGui::GetContentRegionAvail().x;
				float pad = (avail > wheel_w) ? (avail - wheel_w) * 0.5f : 0.f;
				if (pad > 0.f)
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + pad);
				DrawColorWheel("##wheel", colors[editing], radius);
				ImGui::Spacing();
				ImGui::SliderFloat("R", &colors[editing].x, 0.f, 1.f);
				ImGui::SliderFloat("G", &colors[editing].y, 0.f, 1.f);
				ImGui::SliderFloat("B", &colors[editing].z, 0.f, 1.f);
				ImGui::SliderFloat("A", &colors[editing].w, 0.f, 1.f);
				ImGui::PopID();
				ImGui::Spacing();
				ImGui::Separator();
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::EndChild();
	}
}
