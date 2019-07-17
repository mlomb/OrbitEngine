#ifndef GRAPHICS_RENDERER2D_HPP
#define GRAPHICS_RENDERER2D_HPP

#include <vector>

#include "OE/Graphics/Renderer.hpp"
#include "OE/Graphics/API/Mesh.hpp"

#include "OE/Misc/ScissorStack.hpp"

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Color.hpp"
#include "OE/Math/UV.hpp"

#define OE_2D_MAX_TEXTURES 16

namespace OrbitEngine { namespace Graphics {
	class Texture;

	struct Vertex2DSimple {
		Math::Vec2f position;
		Math::Vec2f uv;
	};

	class Renderer2D : public Renderer {
	public:
		~Renderer2D() {};

		int handleTexture(Texture* tex);
		Misc::ScissorStack* getScissorStack();

		virtual void bindColor(const Math::Color& color) = 0;
		void bindTexture(Texture* tex);
		void setPVMatrix(const Math::Mat4& matrix);
		void setTransform(const Math::Mat4& matrix);
		void resetTransform();

		void rect(const Math::Vec2f& position, const Math::Vec2f& size);
		void rect(const Math::Vec2f& position, const Math::Vec2f& size, const Math::UV& uvs, bool flipX = false, bool flipY = false);

		virtual void begin();
		virtual void submitRect(const Math::Vec2f(&positions)[4], const Math::Vec2f(&uvs)[4]) = 0;
		virtual void end();

		static void RenderQuadScreen();
	protected:
		Renderer2D();

		unsigned short p_BindTexture;
		std::vector<Texture*> p_Textures;
		Misc::ScissorStack* p_ScissorStack;
		Math::Mat4 p_Transform;

		UniformsPack<Math::Mat4>* m_UPVMatrix;

	private:
		// Little cache
		Math::Vec2f m_TempVertexs[4];
		Math::Vec2f m_TempUVs[4];

		static Mesh* s_Quad;
	};
} }

#endif