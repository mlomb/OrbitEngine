#ifndef MAT4_INL
#define MAT4_INL

#include "OE/Application/Context.hpp"

#include "OE/Math/Vec4.hpp"
#include "Mat4.hpp"

namespace OrbitEngine { namespace Math {
	inline Mat4::Mat4(float d)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m[i][j] = 0.0f;

		m[0][0] = d;
		m[1][1] = d;
		m[2][2] = d;
		m[3][3] = d;
	}

	inline Mat4 operator*(const Mat4& a, const float& b)
	{
		Mat4 r = Mat4();
		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				r.m[x][y] = a.m[x][y] * b;
			}
		}
		return r;
	}

	inline Mat4 operator*(const Mat4& a, const Mat4& b)
	{
		Mat4 r = Mat4();
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				float sum = 0.0;
				for (int e = 0; e < 4; e++) sum += a.m[x][e] * b.m[e][y];
				r.m[x][y] = sum;
			}
		}
		return r;
	}

	inline Vec2f operator*(const Mat4& a, const Vec2f& b)
	{
		return Vec2f(
			(b.x * a._11) + (b.y * a._21) + a._41,
			(b.x * a._12) + (b.y * a._22) + a._42);
	}
	
	inline Vec3f operator*(const Mat4& a, const Vec3f& b)
	{
		return Vec3f(
			(b.x * a._11) + (b.y * a._21) + (b.z * a._31) + a._41,
			(b.x * a._12) + (b.y * a._22) + (b.z * a._32) + a._42,
			(b.x * a._13) + (b.y * a._23) + (b.z * a._33) + a._43);
	}

	inline Vec4f operator*(const Mat4& a, const Vec4f& b)
	{
		return Vec4f(
			(b.x * a._11) + (b.y * a._21) + (b.z * a._31) + (b.w * a._41),
			(b.x * a._12) + (b.y * a._22) + (b.z * a._32) + (b.w * a._42),
			(b.x * a._13) + (b.y * a._23) + (b.z * a._33) + (b.w * a._43),
			(b.x * a._14) + (b.y * a._24) + (b.z * a._34) + (b.w * a._44));
	}

	inline Vec4f operator*(const Vec4f& a, const Mat4& b)
	{
		return Vec4f(
			(a.x * b._11) + (a.y * b._12) + (a.z * b._13) + (a.w * b._14),
			(a.x * b._21) + (a.y * b._22) + (a.z * b._23) + (a.w * b._24),
			(a.x * b._31) + (a.y * b._32) + (a.z * b._33) + (a.w * b._34),
			(a.x * b._41) + (a.y * b._42) + (a.z * b._43) + (a.w * b._44));
	}

	inline Mat4 Mat4::Identity() {
		return Mat4(1.0f);
	}

	inline Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float _near, float _far) {
		Mat4 r = Mat4(1.0f);

		r._11 = 2.0f / (right - left);
		r._22 = 2.0f / (top - bottom);
		r._33 = 1.0f / (_near - _far);

		r._41 = (left + right) / (left - right);
		r._42 = (top + bottom) / (bottom - top);
		r._43 = _near / (_near - _far);

		return r;
	}

	inline Mat4 Mat4::Perspective(float fov, float aspectRatio, float _near, float _far)
	{
		Mat4 r = Mat4(1.0f);

		float q = (float)(1.0 / tan(deg2rad(0.5f * fov)));
		float a = q / aspectRatio;

		float b = (_near + _far) / (_near - _far);
		float c = (2.0f * _near * _far) / (_near - _far);

		r._11 = a;
		r._22 = q;
		r._33 = b;

		r._34 = -1.0f;
		r._43 = c;

#if OE_D3D
		// Don't ask me.
		if (Application::Context::GetCurrentAPI() == RenderAPI::DIRECT3D)
			r = r * Mat4::Scale(Math::Vec3f(1, -1, 1));
#endif

		return r;
	}

	inline Mat4 Mat4::LookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up)
	{
		Mat4 r = Mat4(1.0);

		Vec3f f = Vec3f::Normalize(target - eye);
		Vec3f s = Vec3f::Cross(f, Vec3f::Normalize(up));
		Vec3f u = Vec3f::Cross(s, f);

		r._11 = s.x;
		r._21 = s.y;
		r._31 = s.z;

		r._12 = u.x;
		r._22 = u.y;
		r._32 = u.z;

		r._13 = -f.x;
		r._23 = -f.y;
		r._33 = -f.z;

		return Translation(eye * -1.0f) * r;
	}

	inline Mat4 Mat4::Translation(const Vec3f& translation)
	{
		Mat4 r = Mat4(1.0);

		r._41 = translation.x;
		r._42 = translation.y;
		r._43 = translation.z;

		return r;
	}

	inline Mat4 Mat4::Translation(const Vec2f& translation)
	{
		Mat4 r = Mat4(1.0);

		r._41 = translation.x;
		r._42 = translation.y;

		return r;
	}

	inline Mat4 Mat4::Rotation(float angle, const Vec3f& axis)
	{
		Mat4 r = Mat4(1.0f);

		double rad = angle;
		float c = (float)cos(rad);
		float s = (float)sin(rad);
		float omc = 1.0f - c;

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		r._11 = x * omc + c;
		r._21 = y * x * omc + z * s;
		r._31 = x * z * omc - y * s;

		r._12 = x * y * omc - z * s;
		r._22 = y * omc + c;
		r._32 = y * z * omc + x * s;

		r._13 = x * z * omc + y * s;
		r._23 = y * z * omc - x * s;
		r._33 = z * omc + c;

		return r;
	}

	inline Mat4 Mat4::Scale(const Vec3f& scale)
	{
		Mat4 r = Mat4(1.0f);

		r._11 = scale.x;
		r._22 = scale.y;
		r._33 = scale.z;

		return r;
	}

	inline Mat4 Mat4::Transpose(const Mat4& matrix)
	{
		Mat4 r = Mat4(1.0f);

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				r.m[y][x] = matrix.m[x][y];
			}
		}

		return r;
	}

	inline Mat4 Mat4::Inverse(const Mat4& matrix)
	{
		// Taken from gluInvertMatrix

		float inv[16], det;
		int i;

		inv[0] = matrix.data[5] * matrix.data[10] * matrix.data[15] -
			matrix.data[5] * matrix.data[11] * matrix.data[14] -
			matrix.data[9] * matrix.data[6] * matrix.data[15] +
			matrix.data[9] * matrix.data[7] * matrix.data[14] +
			matrix.data[13] * matrix.data[6] * matrix.data[11] -
			matrix.data[13] * matrix.data[7] * matrix.data[10];

		inv[4] = -matrix.data[4] * matrix.data[10] * matrix.data[15] +
			matrix.data[4] * matrix.data[11] * matrix.data[14] +
			matrix.data[8] * matrix.data[6] * matrix.data[15] -
			matrix.data[8] * matrix.data[7] * matrix.data[14] -
			matrix.data[12] * matrix.data[6] * matrix.data[11] +
			matrix.data[12] * matrix.data[7] * matrix.data[10];

		inv[8] = matrix.data[4] * matrix.data[9] * matrix.data[15] -
			matrix.data[4] * matrix.data[11] * matrix.data[13] -
			matrix.data[8] * matrix.data[5] * matrix.data[15] +
			matrix.data[8] * matrix.data[7] * matrix.data[13] +
			matrix.data[12] * matrix.data[5] * matrix.data[11] -
			matrix.data[12] * matrix.data[7] * matrix.data[9];

		inv[12] = -matrix.data[4] * matrix.data[9] * matrix.data[14] +
			matrix.data[4] * matrix.data[10] * matrix.data[13] +
			matrix.data[8] * matrix.data[5] * matrix.data[14] -
			matrix.data[8] * matrix.data[6] * matrix.data[13] -
			matrix.data[12] * matrix.data[5] * matrix.data[10] +
			matrix.data[12] * matrix.data[6] * matrix.data[9];

		inv[1] = -matrix.data[1] * matrix.data[10] * matrix.data[15] +
			matrix.data[1] * matrix.data[11] * matrix.data[14] +
			matrix.data[9] * matrix.data[2] * matrix.data[15] -
			matrix.data[9] * matrix.data[3] * matrix.data[14] -
			matrix.data[13] * matrix.data[2] * matrix.data[11] +
			matrix.data[13] * matrix.data[3] * matrix.data[10];

		inv[5] = matrix.data[0] * matrix.data[10] * matrix.data[15] -
			matrix.data[0] * matrix.data[11] * matrix.data[14] -
			matrix.data[8] * matrix.data[2] * matrix.data[15] +
			matrix.data[8] * matrix.data[3] * matrix.data[14] +
			matrix.data[12] * matrix.data[2] * matrix.data[11] -
			matrix.data[12] * matrix.data[3] * matrix.data[10];

		inv[9] = -matrix.data[0] * matrix.data[9] * matrix.data[15] +
			matrix.data[0] * matrix.data[11] * matrix.data[13] +
			matrix.data[8] * matrix.data[1] * matrix.data[15] -
			matrix.data[8] * matrix.data[3] * matrix.data[13] -
			matrix.data[12] * matrix.data[1] * matrix.data[11] +
			matrix.data[12] * matrix.data[3] * matrix.data[9];

		inv[13] = matrix.data[0] * matrix.data[9] * matrix.data[14] -
			matrix.data[0] * matrix.data[10] * matrix.data[13] -
			matrix.data[8] * matrix.data[1] * matrix.data[14] +
			matrix.data[8] * matrix.data[2] * matrix.data[13] +
			matrix.data[12] * matrix.data[1] * matrix.data[10] -
			matrix.data[12] * matrix.data[2] * matrix.data[9];

		inv[2] = matrix.data[1] * matrix.data[6] * matrix.data[15] -
			matrix.data[1] * matrix.data[7] * matrix.data[14] -
			matrix.data[5] * matrix.data[2] * matrix.data[15] +
			matrix.data[5] * matrix.data[3] * matrix.data[14] +
			matrix.data[13] * matrix.data[2] * matrix.data[7] -
			matrix.data[13] * matrix.data[3] * matrix.data[6];

		inv[6] = -matrix.data[0] * matrix.data[6] * matrix.data[15] +
			matrix.data[0] * matrix.data[7] * matrix.data[14] +
			matrix.data[4] * matrix.data[2] * matrix.data[15] -
			matrix.data[4] * matrix.data[3] * matrix.data[14] -
			matrix.data[12] * matrix.data[2] * matrix.data[7] +
			matrix.data[12] * matrix.data[3] * matrix.data[6];

		inv[10] = matrix.data[0] * matrix.data[5] * matrix.data[15] -
			matrix.data[0] * matrix.data[7] * matrix.data[13] -
			matrix.data[4] * matrix.data[1] * matrix.data[15] +
			matrix.data[4] * matrix.data[3] * matrix.data[13] +
			matrix.data[12] * matrix.data[1] * matrix.data[7] -
			matrix.data[12] * matrix.data[3] * matrix.data[5];

		inv[14] = -matrix.data[0] * matrix.data[5] * matrix.data[14] +
			matrix.data[0] * matrix.data[6] * matrix.data[13] +
			matrix.data[4] * matrix.data[1] * matrix.data[14] -
			matrix.data[4] * matrix.data[2] * matrix.data[13] -
			matrix.data[12] * matrix.data[1] * matrix.data[6] +
			matrix.data[12] * matrix.data[2] * matrix.data[5];

		inv[3] = -matrix.data[1] * matrix.data[6] * matrix.data[11] +
			matrix.data[1] * matrix.data[7] * matrix.data[10] +
			matrix.data[5] * matrix.data[2] * matrix.data[11] -
			matrix.data[5] * matrix.data[3] * matrix.data[10] -
			matrix.data[9] * matrix.data[2] * matrix.data[7] +
			matrix.data[9] * matrix.data[3] * matrix.data[6];

		inv[7] = matrix.data[0] * matrix.data[6] * matrix.data[11] -
			matrix.data[0] * matrix.data[7] * matrix.data[10] -
			matrix.data[4] * matrix.data[2] * matrix.data[11] +
			matrix.data[4] * matrix.data[3] * matrix.data[10] +
			matrix.data[8] * matrix.data[2] * matrix.data[7] -
			matrix.data[8] * matrix.data[3] * matrix.data[6];

		inv[11] = -matrix.data[0] * matrix.data[5] * matrix.data[11] +
			matrix.data[0] * matrix.data[7] * matrix.data[9] +
			matrix.data[4] * matrix.data[1] * matrix.data[11] -
			matrix.data[4] * matrix.data[3] * matrix.data[9] -
			matrix.data[8] * matrix.data[1] * matrix.data[7] +
			matrix.data[8] * matrix.data[3] * matrix.data[5];

		inv[15] = matrix.data[0] * matrix.data[5] * matrix.data[10] -
			matrix.data[0] * matrix.data[6] * matrix.data[9] -
			matrix.data[4] * matrix.data[1] * matrix.data[10] +
			matrix.data[4] * matrix.data[2] * matrix.data[9] +
			matrix.data[8] * matrix.data[1] * matrix.data[6] -
			matrix.data[8] * matrix.data[2] * matrix.data[5];

		det = matrix.data[0] * inv[0] + matrix.data[1] * inv[4] + matrix.data[2] * inv[8] + matrix.data[3] * inv[12];

		if (det == 0)
			return Mat4(); // Mat4() or matrix?

		det = 1.0f / det;

		Mat4 result;

		for (i = 0; i < 16; i++)
			result.data[i] = inv[i] * det;

		return result;
	}
} }

#endif