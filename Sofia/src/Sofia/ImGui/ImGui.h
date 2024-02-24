#pragma once

#include "Sofia/Core.h"
#include "Sofia/Renderer/Texture.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Sofia::UI
{
	SOF_CORE void ShiftCursorX(float x) noexcept;
	SOF_CORE void ShiftCursorY(float y) noexcept;
	SOF_CORE void ShiftCursor(float x, float y) noexcept;

	SOF_CORE ImRect GetItemRect() noexcept;
	SOF_CORE ImRect RectExpand(const ImRect& rect, float x, float y) noexcept;
	SOF_CORE ImRect RectMove(const ImRect& rect, float x, float y) noexcept;

	bool BeginMenuBar(const ImRect& menuBarRect);
	void EndMenuBar();

	SOF_CORE void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax) noexcept;
	SOF_CORE void DrawButtonImage(Ref<Texture2D>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rect) noexcept;

	SOF_CORE void RenderWindowOuterBorders(ImGuiWindow* window);
}
