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

		void setPVMatrix(const Math::Mat4& matrix);
		void setTransform(const Math::Mat4& matrix);
		void resetTransform();

		virtual void begin() override;
		virtual void end() override;

		static void RenderQuadScreen();
	protected:
		Renderer2D();

		Math::Mat4 p_Transform;

		UniformsPack<Math::Mat4>* m_UPVMatrix;

	private:
		static Mesh* s_Quad;
	};
} }

#endif