#ifndef GRAPHICS_MODEL_HPP
#define GRAPHICS_MODEL_HPP

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/3D/Material.hpp"
#include "OE/Graphics/3D/Renderer3D.hpp"

#include "OE/Math/Mat4.hpp"

class aiScene;
class aiNode;
class aiMesh;

namespace OrbitEngine { namespace Graphics {
	struct MeshEntry {
		Mesh* mesh;
		Material* material;
		unsigned int count;
	};

	class Model {
	public:
		Model(std::string path);
		~Model();

		void render(Renderer3D* r3d, const Math::Mat4 &transform);
		MeshEntry* getEntry(unsigned int i) const;
		unsigned int getEntryCount() const { return m_Meshes.size(); };

	private:
		std::string m_Directory;
		const aiScene* m_Scene;
		std::vector<MeshEntry*> m_Meshes;
		std::map<std::string, Texture*> m_TexturesMap;

		void processTextures();
		void processNode(aiNode* node);
		MeshEntry* processMeshEntry(aiMesh* mesh);
	};
} }

#endif