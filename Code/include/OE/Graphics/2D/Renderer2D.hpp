#ifndef GRAPHICS_RENDERER2D_HPP
#define GRAPHICS_RENDERER2D_HPP

#include <vector>

#include "OE/Graphics/Renderer.hpp"
#include "OE/Graphics/API/Mesh.hpp"

namespace OrbitEngine { namespace Graphics {
	class Texture;

	class Renderer2D : public Renderer {
	public:
		virtual ~Renderer2D() {};

		void setPVMatrices(const Math::Mat4& projection_matrix, const Math::Mat4& view_matrix = Math::Mat4::Identity());
		void setTransform(const Math::Mat4& matrix);
		void resetTransform();

		virtual void begin() override;
		virtual void end() override;

		static void RenderQuadScreen(Shader* shader);
	protected:
		Renderer2D();

		Math::Mat4 p_ProjectionMatrix;
		Math::Mat4 p_ViewMatrix;
		Math::Mat4 p_Transform;

		UniformsPack<Math::Mat4>* m_UPVMatrix;

	private:
		static Mesh* s_Quad;
	};
} }

#endif