#ifndef GRAPHICS_MATERIAL_HPP
#define GRAPHICS_MATERIAL_HPP

#include <map>

#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/API/Texture.hpp"
#include "OE/Graphics/ManagedShader.hpp"

#include "OE/Math/Vec3.hpp"
#include "OE/Math/Color.hpp"

namespace OrbitEngine { namespace Graphics {
	enum MaterialMapType {
		ALBEDO = 0,
		METALLIC,
		ROUGHNESS,
		NORMALS
	};

	struct MaterialUniforms {
		Math::Color4f baseColor = Math::Color4f(1.0f, 1.0f, 1.0f, 1.0f);
		float metallic = 0.5f;
		float roughness = 0.5f;

		float usingAlbedoMap = 0;
		float usingNormalMap = 0;
		float usingMetallicMap = 0;
		float usingRoughnessMap = 0;

		float pad1, pad2;
	};

	class Material {
	public:
		Material(ManagedShader* shader);

		Shader* use(ShaderDefinitions definitions = {});

		void setMap(MaterialMapType mapType, Texture* texture);
		inline void setBaseColor(Math::Color4f& color) { m_Uniforms.baseColor = color; };
		inline void setMetallic(float metallic) { m_Uniforms.metallic = metallic; };
		inline void setRoughness(float roughness) { m_Uniforms.roughness = roughness; };

		inline Math::Color4f getBaseColor() { return m_Uniforms.baseColor; }
		inline float getMetallic() { return m_Uniforms.metallic; }
		inline float getRoughness() { return m_Uniforms.roughness; }

		static std::string MapTypeToString(MaterialMapType mapType);
	private:
		std::map<MaterialMapType, Graphics::Texture*> m_Maps;
		MaterialUniforms m_Uniforms;
		ManagedShader* m_Shader;

		static UniformsPack<MaterialUniforms>* s_MaterialUniformsBuffer;
	};
} }

#endif