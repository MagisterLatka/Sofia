#pragma once

#include "Sofia/Core.h"
#include "Sofia/Renderer/Texture.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Sofia::UI
{
	bool DragFloat(const std::string& label, float& value, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragFloat2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragFloat3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragFloat4(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);

	bool DragInt(const std::string& label, int& value, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragInt2(const std::string& label, glm::ivec2& values, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragInt3(const std::string& label, glm::ivec3& values, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragInt4(const std::string& label, glm::ivec4& values, int resetValue = 0, int minVal = 0, int maxVal = 0, float speed = 0.0f, float columnWidth = 100.0f);

	bool ColorEdit3(const std::string& label, glm::vec3& values, float columnWidth = 100.0f);
	bool ColorEdit4(const std::string& label, glm::vec4& values, float columnWidth = 100.0f);

	void HelpMarker(const char* desc);

	void ShiftCursorX(float x) noexcept;
	void ShiftCursorY(float y) noexcept;
	void ShiftCursor(float x, float y) noexcept;

	ImRect GetItemRect() noexcept;
	ImRect RectExpand(const ImRect& rect, float x, float y) noexcept;
	ImRect RectMove(const ImRect& rect, float x, float y) noexcept;

	bool BeginMenuBar(const ImRect& menuBarRect);
	void EndMenuBar();

	void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax) noexcept;
	void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rect) noexcept;

	void RenderWindowOuterBorders(ImGuiWindow* window);
}
