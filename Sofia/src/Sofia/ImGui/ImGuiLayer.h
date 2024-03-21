#pragma once

#include "Sofia/Layer.h"

struct ImGuiContext;

namespace Sofia {

	class ImGuiLayer : public Layer
	{
	public:
		SOF_CORE ImGuiLayer()
			: Layer("ImGuiLayer") {}
		SOF_CORE virtual ~ImGuiLayer() = default;

		SOF_CORE ImGuiContext* GetContext() noexcept { return m_Context; }

		SOF_CORE void Begin();
		SOF_CORE void End();

		SOF_CORE virtual void OnAttach() override;
		SOF_CORE virtual void OnDetach() override;
		SOF_CORE virtual void OnEvent(Event& e) override;
		
		SOF_CORE void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		void Init();
	private:
		ImGuiContext* m_Context = nullptr;
		bool m_BlockEvents = true;
	};
}
