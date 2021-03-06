#ifndef GRAPHICS_LIGHT_HPP
#define GRAPHICS_LIGHT_HPP

#include "OE/Math/Vec3.hpp"
#include "OE/Math/Color.hpp"

#include "OE/Graphics/API/Shader.hpp"
#include "OE/Graphics/API/UniformsPack.hpp"

namespace OrbitEngine { namespace Graphics {
	namespace priv {
		struct DirectionalData {
			Math::Vec3f direction = Math::Vec3f(0.8f, 0.8f, 0.8f);
			float pad;
		};
		struct PointData {
			//float test;
		};
	}

	class Light {
	public:
		virtual ~Light() {};

		virtual void setPosition(Math::Vec3f position) = 0;
		virtual void setColor(Math::Color4f color) = 0;

		virtual Math::Vec3f getPosition() = 0;

		virtual void fillBuffer(Shader* shader) = 0;
		virtual ShaderDefinitions getRequiredDefinitions() = 0;
	protected:
		Light() {};
	};

	template<class T>
	class GenericLight : public Light {
	public:
		virtual ~GenericLight();

		void setPosition(Math::Vec3f position) override;
		void setColor(Math::Color4f color) override;
		void fillBuffer(Shader* shader) override;

		Math::Vec3f getPosition() override;

		virtual ShaderDefinitions getRequiredDefinitions() override;
	protected:
		GenericLight();

		struct Data {
			Math::Vec3f position;
			float pad;
			Math::Color4f color;
			T specific;
		} p_Data;
	private:
		// TODO Make this static
		UniformsPack<Data>* s_LightUniformPack;
	};

	class PointLight : public GenericLight<priv::PointData> {
	public:
		ShaderDefinitions getRequiredDefinitions() override;

	private:

	};

	class DirectionalLight : public GenericLight<priv::DirectionalData> {
	public:
		ShaderDefinitions getRequiredDefinitions() override;

	private:

	};
} }

#include "OE/Graphics/3D/Light.inl"

#endif