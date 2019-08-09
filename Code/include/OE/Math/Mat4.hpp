#ifndef MATH_MAT4_HPP
#define MATH_MAT4_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec3.hpp"

namespace OrbitEngine { namespace Math {
	/// 4x4 matrix
	struct Mat4 {
		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
			float data[16];
		};

		/**
			@brief Create a new matrix
			@param[in] d Value along the diagonal
		*/
		Mat4(float d = 1.0f);

		static Mat4 Identity();
		static Mat4 Orthographic(float left, float right, float bottom, float top, float _near, float _far);
		static Mat4 Perspective(float fov, float aspectRatio, float _near, float _far);
		static Mat4 LookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up);

		static Mat4 Translation(const Vec3f& translation);
		static Mat4 Translation(const Vec2f& translation);
		static Mat4 Rotation(float angle, const Vec3f& axis);
		static Mat4 Scale(const Vec3f& scale);
		static Mat4 Transpose(const Mat4& matrix);
		static Mat4 Inverse(const Mat4& matrix);
	};
} }

#include "OE/Math/Mat4.inl"

#endif