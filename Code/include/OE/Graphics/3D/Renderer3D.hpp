#ifndef GRAPHICS_RENDERER3D_HPP
#define GRAPHICS_RENDERER3D_HPP

#include "OE/Graphics/Renderer.hpp"
#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/3D/Material.hpp"
#include "OE/Graphics/3D/Light.hpp"

#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	class BRDFLUT;
	class Skybox;

	struct Vertex3D {
		Math::Vec3f position;
		Math::Vec3f normal;
		Math::Vec2f uv;
		Math::Vec3f tangent;
	};

	struct CameraBuffer {
		Math::Mat4 PVMatrix;
		Math::Vec3f CameraPosition;

		float pad0;
	};

	struct ObjectBuffer {
		Math::Mat4 MMatrix;
	};

	class Renderer3D : public Renderer {
	public:
		virtual ~Renderer3D();

		struct Command {
			Mesh* mesh;
			Material* material;
			Math::Mat4 transform;
			unsigned int count;
		};

		void begin() override;

		void submitMesh(Mesh* mesh, unsigned int count, Material* material = 0, const Math::Mat4& transform = Math::Mat4::Identity());
		void addLight(Light* light);
		inline void useSkybox(Skybox* skybox) { p_Skybox = skybox; }

		static VertexLayout* GetVertex3DLayout();
	protected:
		Renderer3D();

		void fillCameraBuffer(Shader* shader);
		void fillObjectBuffer(Shader* shader, const Command& command);

		std::vector<Command> p_Commands;
		std::vector<Light*> p_Lights;
		Skybox* p_Skybox = 0;

		BRDFLUT* p_BRDFLUT;
	private:
		UniformsPack<CameraBuffer>* m_CameraBufferPack;
		UniformsPack<ObjectBuffer>* m_ObjectBufferPack;

		static VertexLayout* s_Vertex3DLayout;
	};
} }

#endif