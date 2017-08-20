#include "OE/Graphics/3D/Material.hpp"

#if OE_OPENGL_ANY
	#include "OE/Platform/OpenGL/GLShader.hpp"
#endif

namespace OrbitEngine { namespace Graphics {

	UniformsPack<MaterialUniforms>* Material::s_MaterialUniformsBuffer = nullptr;

	Material::Material(ManagedShader* shader)
		: m_Shader(shader)
	{
		if (s_MaterialUniformsBuffer == nullptr)
			s_MaterialUniformsBuffer = UniformsPack<MaterialUniforms>::Create();
	}

	Shader* Material::use(ShaderDefinitions definitions)
	{
		Shader* shader = m_Shader->requestShader(definitions);

		switch (Application::Context::GetCurrentAPI())
		{
		case RenderAPI::OPENGL:
#if OE_OPENGL_ES
		case RenderAPI::OPENGL_ES:
#endif
			GLShader* glShader = (GLShader*)shader;
			glShader->bind();
			for (int i = 0; i < 4; i++)
			{
				MaterialMapType type = static_cast<MaterialMapType>(i);
				glShader->setUniform1i(("map_" + MapTypeToString(type)).c_str(), i);
			}
			glShader->bindUBO("MaterialUniforms", 2);
			break;
		}
		s_MaterialUniformsBuffer->setData(m_Uniforms);
		s_MaterialUniformsBuffer->bind(2, ShaderType::FRAGMENT);

		std::map<MaterialMapType, Texture*>::iterator it;
		for (int i = 0; i < 4; i++)
		{
			MaterialMapType type = static_cast<MaterialMapType>(i);
			std::map<MaterialMapType, Texture*>::iterator it2 = m_Maps.find(type);
			if (it2 != m_Maps.end() && it2->second != 0)
				it2->second->bind(i);
			else
				Texture::Unbind(i);
		}

		return shader;
	}

	void Material::setMap(MaterialMapType mapType, Texture* texture)
	{
		m_Maps[mapType] = texture;

		float active = texture != nullptr ? 1.0f : 0.0f;
		switch (mapType)
		{
		case ALBEDO:
			m_Uniforms.usingAlbedoMap = active;
			break;
		case NORMALS:
			m_Uniforms.usingNormalMap = active;
			break;
		case METALLIC:
			m_Uniforms.usingMetallicMap = active;
			break;
		case ROUGHNESS:
			m_Uniforms.usingRoughnessMap = active;
			break;
		}
	}
	
	std::string Material::MapTypeToString(MaterialMapType mapType)
	{
		switch (mapType) {
		case ALBEDO: return "albedo";
		case METALLIC: return "metallic";
		case ROUGHNESS: return "roughness";
		case NORMALS: return "normals";
		}
		return "unknown";
	}
} }