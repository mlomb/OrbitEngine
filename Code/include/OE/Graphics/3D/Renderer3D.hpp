#ifndef GRAPHICS_RENDERER3D_HPP
#define GRAPHICS_RENDERER3D_HPP

#include "OE/Graphics/Renderer.hpp"
#include "OE/Graphics/API/Mesh.hpp"

#include "OE/Graphics/3D/Material.hpp"

#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class Skybox;

	struct Vertex3D {
		Math::Vec3f position;
		Math::Vec3f normal;
		Math::Vec2f uv;
		Math::Vec3f tangent;
	};

	class Renderer3D : public Renderer {
	public:
		struct Command {
			Mesh* mesh;
			Material* material;
			Math::Mat4 transform;
			unsigned int count;
		};

		void begin() override;

		void submitMesh(Mesh* mesh, unsigned int count, Material* material = 0, const Math::Mat4& transform = Math::Mat4::Identity());
		inline void useSkybox(Skybox* skybox) { m_Skybox = skybox; }

		static VertexLayout* GetVertex3DLayout();
	protected:
		std::vector<Command> m_Commands;
		Skybox* m_Skybox = 0;

		static VertexLayout* s_Vertex3DLayout;
	};
} }

#endif