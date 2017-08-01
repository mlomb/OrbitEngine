#ifndef GRAPHICS_MATERIAL_HPP
#define GRAPHICS_MATERIAL_HPP

#include <map>

#include "OE/Graphics/API/UniformsPack.hpp"
#include "OE/Graphics/API/Texture.hpp"

#include "OE/Math/Vec3.hpp"

namespace OrbitEngine { namespace Graphics {
	enum MaterialMapType {
		ALBEDO = 0,
		METALLIC,
		ROUGHNESS,
		NORMALS
	};

	struct MaterialUniforms {
		Math::Vec3f baseColor = Math::Vec3f(1, 1, 1);
		float metallic = 0.5f;
		float roughness = 0.5f;

		float usingAlbedoMap = 0;
		float usingNormalMap = 0;
		float usingMetallicMap = 0;
		float usingRoughnessMap = 0;

		float pad1, pad2, pad3;
	};

	class Material {
	public:
		Material();

		void use();

		void setMap(MaterialMapType mapType, Texture* texture);
		inline void setBaseColor(Math::Vec3f& color) { m_Uniforms.baseColor = color; };
		inline void setMetallic(float metallic) { m_Uniforms.metallic = metallic; };
		inline void setRoughness(float roughness) { m_Uniforms.roughness = roughness; };

		inline Math::Vec3f getBaseColor() { return m_Uniforms.baseColor; }
		inline float getMetallic() { return m_Uniforms.metallic; }
		inline float getRoughness() { return m_Uniforms.roughness; }

		static void Prepare(Shader* shader);
		static std::string MapTypeToString(MaterialMapType mapType);
	private:
		std::map<MaterialMapType, Graphics::Texture*> m_Maps;
		MaterialUniforms m_Uniforms;

		static UniformsPack<MaterialUniforms>* s_MaterialUniformsBuffer;
	};
} }

#endif