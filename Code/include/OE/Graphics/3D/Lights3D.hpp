#ifndef GRAPHICS_LIGHTS3D_HPP
#define GRAPHICS_LIGHTS3D_HPP

#include "OE/Math/Vec3.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {
	struct Light3DData {
		Math::Vec3f position;
		float pad0;
		Math::Vec3f color;
		float pad1;
		Math::Vec3f fallofs; // constant, linear, quadratic
		float pad2;
		Math::Vec3f direction;
		float pad3;
		Math::Mat4 lightMatrix;
	};

	class Light3D {
	public:
		inline void setPosition(Math::Vec3f position) { data.position = position; }
		inline void setColor(Math::Vec3f color) { data.color = color; }

		inline Math::Vec3f& getPosition() { return data.position; }
		inline Math::Vec3f& getColor() { return data.color; }

		inline Light3DData getData() const { return data; }
	protected:
		Light3DData data;
	protected:
		Light3D() {};
	};

	class PointLight3D : public Light3D {
	public:
		PointLight3D();
	};
} }

#endif