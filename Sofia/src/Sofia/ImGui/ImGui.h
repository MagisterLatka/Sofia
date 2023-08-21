#pragma once

#include "Sofia/Core.h"
#include "Sofia/Renderer/Texture.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Sofia::UI
{
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
