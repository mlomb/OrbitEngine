#ifndef ENGINE_MESHRENDERER_HPP
#define ENGINE_MESHRENDERER_HPP

#include "OE/Engine/Component.hpp"

#include "OE/Graphics/3D/Material.hpp"
#include "OE/Graphics/API/Mesh.hpp"

namespace OrbitEngine {	namespace Engine {

	class MeshRenderer : public Component {
		NATIVE_REFLECTION

	public:
		MeshRenderer();
		~MeshRenderer();

	private:
		Graphics::Mesh* m_Mesh;
		Graphics::Material* m_Material;
	};
} }

#endif