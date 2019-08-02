#ifndef GRAPHICS_IMGUI_RENDERER_HPP
#define GRAPHICS_IMGUI_RENDERER_HPP

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace OrbitEngine { namespace Graphics {

	class ImGuiRenderer : public Renderer2D {
	public:
		ImGuiRenderer();
		~ImGuiRenderer();

		void begin() override;
		void end() override;

		void draw(ImDrawData* draw_data);

	private:
		ImGuiContext* m_ImGuiContext;

		long long m_Time;
		Mesh* m_Mesh;
		Shader* m_Shader;
	};
} }

#endif