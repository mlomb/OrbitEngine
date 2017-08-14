#include "OE/Graphics/3D/Model.hpp"

#include "OE/System/File.hpp"

#define ASSIMP_BUILD_BOOST_WORKAROUND
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "OE/System/AssimpIOWrapper.hpp"

namespace OrbitEngine { namespace Graphics {

	static MaterialMapType AssimpTextureTypeToMaterialMap(aiTextureType textureType)
	{
		switch (textureType) {
		case aiTextureType_DIFFUSE: return ALBEDO;
		case aiTextureType_SPECULAR: return METALLIC;
		case aiTextureType_NORMALS: return NORMALS;
		case aiTextureType_SHININESS: return ROUGHNESS;
		}

		return (MaterialMapType)-1;
	}

	Model::Model(std::string path)
		: m_Scene(0)
	{
		OE_LOG_DEBUG("Loading model: " << path);

		Assimp::Importer* importer = new Assimp::Importer();
		importer->SetIOHandler(new System::priv::AssimpIOSystem());

		m_Scene = importer->ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate | aiProcess_CalcTangentSpace);

		if (!m_Scene)
		{
			OE_LOG_WARNING("Error loading model '" + path + "' with Assimp!");
			delete importer;
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));

		processTextures();
		processNode(m_Scene->mRootNode);

		aiReleaseImport(m_Scene);
		m_Scene = NULL;

		OE_LOG_DEBUG("Loaded '" + path + "' with " + std::to_string(m_Meshes.size()) + " meshes and " + std::to_string(m_TexturesMap.size()) + " textures.");
	}

	Model::~Model() {
		// TODO cleanup
	}

	void Model::render(Renderer3D* r3d, const Math::Mat4& transform)
	{
		for (MeshEntry* meshEntry : m_Meshes)
			if (meshEntry != nullptr)
				r3d->submitMesh(meshEntry->mesh, meshEntry->count, meshEntry->material, transform);
	}

	MeshEntry* Model::getEntry(unsigned int i) const
	{
		return m_Meshes[i];
	}

	void Model::processTextures()
	{
		aiString path;
		for (int i = aiTextureType_DIFFUSE; i != aiTextureType_REFLECTION; i++) {
			aiTextureType type = static_cast<aiTextureType>(i);
			MaterialMapType inputType = AssimpTextureTypeToMaterialMap(type);

			if (inputType == -1) {
				OE_LOG_WARNING("TextureMap not supported (aiTextureType = " + std::to_string(i) + ")");
				continue;
			}

			for (unsigned int m = 0; m < m_Scene->mNumMaterials; m++)
			{
				int textures = m_Scene->mMaterials[m]->GetTextureCount(type);

				for (int i = 0; i < textures; i++) {
					if (AI_SUCCESS == m_Scene->mMaterials[m]->GetTexture(type, i, &path)) {
						m_TexturesMap[path.data] = NULL;
					}
				}
			}
		}

		std::map<std::string, Graphics::Texture*>::iterator itr = m_TexturesMap.begin();

		for (size_t i = 0; i < m_TexturesMap.size(); i++)
		{
			std::string filename = (*itr).first;

			Graphics::TextureProperties properties;
			properties.sampleProperties.filter = Graphics::TextureFilter::ANISOTROPIC;
			properties.sampleProperties.wrap = Graphics::TextureWrap::REPEAT;
			Graphics::Texture* texture = Graphics::Texture::Load(m_Directory + '/' + filename, properties);

			(*itr).second = texture;
			itr++;
		}
	}

	void Model::processNode(aiNode* node)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++)
			m_Meshes.push_back(processMeshEntry(m_Scene->mMeshes[node->mMeshes[i]]));

		for (size_t i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i]);
	}

	MeshEntry* Model::processMeshEntry(aiMesh* mesh)
	{
		MeshEntry* meshEntry = new MeshEntry();

		std::vector<Vertex3D> vertices(mesh->mNumVertices);
		std::vector<unsigned short> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// Vertices
			vertices[i].position.x = mesh->mVertices[i].x;
			vertices[i].position.y = mesh->mVertices[i].y;
			vertices[i].position.z = mesh->mVertices[i].z;

			// Normals
			vertices[i].normal.x = mesh->mNormals[i].x;
			vertices[i].normal.y = mesh->mNormals[i].y;
			vertices[i].normal.z = mesh->mNormals[i].z;

			// UVs
			if (mesh->mTextureCoords[0])
			{
				vertices[i].uv.x = mesh->mTextureCoords[0][i].x;
				vertices[i].uv.y = mesh->mTextureCoords[0][i].y;
			}

			// Tangents
			if (mesh->mTangents != nullptr) {
				vertices[i].tangent.x = mesh->mTangents[i].x;
				vertices[i].tangent.y = mesh->mTangents[i].y;
				vertices[i].tangent.z = mesh->mTangents[i].z;
			}

			/*
			// Bitangents
			vertices[i].bitangent.x = mesh->mBitangents[i].x;
			vertices[i].bitangent.y = mesh->mBitangents[i].y;
			vertices[i].bitangent.z = mesh->mBitangents[i].z;
			*/
		}

		// Indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Materials
		Material* material = new Material();
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* aiMaterial = m_Scene->mMaterials[mesh->mMaterialIndex];
			aiString path;

			for (int i = aiTextureType_DIFFUSE; i != aiTextureType_REFLECTION; i++) {
				aiTextureType type = static_cast<aiTextureType>(i);
				for (unsigned int j = 0; j < aiMaterial->GetTextureCount(type); j++) {
					if (AI_SUCCESS == aiMaterial->GetTexture(type, j, &path)) {
						Graphics::Texture* texture = m_TexturesMap[path.data];

						// TODO REMOVE, TESTING
						if (std::strstr(path.data, "gi_flag") != NULL)
							return nullptr;

						if (texture != NULL) {
							MaterialMapType inputType = AssimpTextureTypeToMaterialMap(type);

							if (inputType == -1)
								continue;

							material->setMap(inputType, texture);
						}
					}
				}
			}
		}

		meshEntry->material = material;

		meshEntry->mesh = Mesh::Create<Vertex3D>(vertices, Renderer3D::GetVertex3DLayout(), indices);
		meshEntry->count = indices.size();

		vertices.clear();
		indices.clear();

		return meshEntry;
	}
} }