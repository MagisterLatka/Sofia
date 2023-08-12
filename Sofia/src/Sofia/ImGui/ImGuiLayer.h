#pragma once

#include "Sofia/Layer.h"

namespace Sofia {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer()
			: Layer("ImGuiLayer")
		{}
		virtual ~ImGuiLayer() = default;

		void Begin();
		void End();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		void Init();
	private:
		bool m_BlockEvents = true;
	};
}
