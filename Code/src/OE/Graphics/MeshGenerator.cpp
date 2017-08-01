#include "OE/Graphics/MeshGenerator.hpp"

#include "OE/Math/UV.hpp"

namespace OrbitEngine { namespace Graphics {
	Mesh* MeshGenerator::FromVertexs(std::vector<Vertex3D>& verts, float size)
	{
		std::vector<unsigned short> triangles = Mesh::GenerateIndices(Topology::TRIANGLES, verts.size());
		return FromVertexs(verts, triangles, size);
	}

	Mesh* MeshGenerator::FromVertexs(std::vector<Vertex3D>& verts, std::vector<unsigned short>& indices, float size)
	{
		if (size != 1) {
			Math::Vec3f s(size, size, size);
			for (Vertex3D& v : verts)
				v.position *= s;
		}

		//Mesh::FillTangents(verts, indices);
		Mesh* mesh = Mesh::Create(verts, Renderer3D::GetVertex3DLayout(), indices);

		verts.clear();
		indices.clear();

		return mesh;
	}

	Mesh* MeshGenerator::Plane(float width, float length, unsigned int resX, unsigned int resZ)
	{
		if (resX < 2 || resZ < 2) {
			OE_LOG_WARNING("The resolution of a Plane must be > 2.");
			return nullptr;
		}

		std::vector<Vertex3D> vertices(resX * resZ);
		for (unsigned int z = 0; z < resZ; z++)
		{
			float zPos = ((float)z / (resZ - 1) - .5f) * length;
			for (unsigned int x = 0; x < resX; x++)
			{
				float xPos = ((float)x / (resX - 1) - .5f) * width;
				vertices[x + z * resX] = {
					Math::Vec3f(xPos, 0.0f, zPos),
					Math::Vec3f::Up(),
					Math::Vec2f((float)x / (resX - 1), (float)z / (resZ - 1))
				};
			}
		}

		int faces = (resX - 1) * (resZ - 1);
		int t = 0;
		std::vector<unsigned short> triangles(faces * 6);

		for (int face = 0; face < faces; face++)
		{
			int i = face % (resX - 1) + (face / (resZ - 1) * resX);

			triangles[t++] = i + resX;
			triangles[t++] = i + 1;
			triangles[t++] = i;

			triangles[t++] = i + resX;
			triangles[t++] = i + resX + 1;
			triangles[t++] = i + 1;
		}

		return FromVertexs(vertices, triangles);
	}

	Mesh* MeshGenerator::Box(float width, float height, float length)
	{
		float halfWidth = width * 0.5f;
		float halfHeight = height * 0.5f;
		float halfLength = length * 0.5f;

		Math::Vec3f p0(-halfWidth, -halfHeight, halfLength);
		Math::Vec3f p1(halfWidth, -halfHeight, halfLength);
		Math::Vec3f p2(halfWidth, -halfHeight, -halfLength);
		Math::Vec3f p3(-halfWidth, -halfHeight, -halfLength);

		Math::Vec3f p4(-halfWidth, halfHeight, halfLength);
		Math::Vec3f p5(halfWidth, halfHeight, halfLength);
		Math::Vec3f p6(halfWidth, halfHeight, -halfLength);
		Math::Vec3f p7(-halfWidth, halfHeight, -halfLength);

		std::vector<Vertex3D> vertices = {
			{ p0, Math::Vec3f::Down() },{ p1, Math::Vec3f::Down() },{ p2, Math::Vec3f::Down() },{ p3, Math::Vec3f::Down() },
			{ p7, Math::Vec3f::Left() },{ p4, Math::Vec3f::Left() },{ p0, Math::Vec3f::Left() },{ p3, Math::Vec3f::Left() },
			{ p4, Math::Vec3f::Forward() },{ p5, Math::Vec3f::Forward() },{ p1, Math::Vec3f::Forward() },{ p0, Math::Vec3f::Forward() },
			{ p6, Math::Vec3f::Back() },{ p7, Math::Vec3f::Back() },{ p3, Math::Vec3f::Back() },{ p2, Math::Vec3f::Back() },
			{ p5, Math::Vec3f::Right() },{ p6, Math::Vec3f::Right() },{ p2, Math::Vec3f::Right() },{ p1, Math::Vec3f::Right() },
			{ p7, Math::Vec3f::Up() },{ p6, Math::Vec3f::Up() },{ p5, Math::Vec3f::Up() },{ p4, Math::Vec3f::Up() },
		};

		for (size_t i = 0; i < vertices.size(); i++)
			vertices[i].uv = Math::UV_Default.getUV(i);

		return FromVertexs(vertices);
	}

	Mesh* MeshGenerator::SphereStandard(float radius, unsigned int stacks /* --- */, unsigned int slices /* ||| */)
	{
		// Adapted from
		// http://wiki.unity3d.com/index.php/ProceduralPrimitives#C.23_-_Sphere

		std::vector<Vertex3D> vertices((slices + 1) * stacks + 2);

		vertices[0] = { Math::Vec3f::Up() * radius, Math::Vec3f::Up(), Math::Vec2f(0, 1) };
		for (unsigned int lat = 0; lat < stacks; lat++)
		{
			float a1 = PI * (float)(lat + 1) / (stacks + 1);
			float sin1 = sin(a1);
			float cos1 = cos(a1);

			for (unsigned int lon = 0; lon <= slices; lon++)
			{
				float a2 = TAU * (float)(lon == slices ? 0 : lon) / slices;
				float sin2 = sin(a2);
				float cos2 = cos(a2);

				Math::Vec3f pos(sin1 * cos2, cos1, sin1 * sin2);

				vertices[lon + lat * (slices + 1) + 1] = {
					pos * radius,
					pos.normalize(),
					Math::Vec2f(1.0f - (float)(lat + 1) / (stacks + 1.0f), (float)lon / slices)
				};
			}
		}
		Math::Vec3f negup = Math::Vec3f::Up() * -radius;
		vertices[vertices.size() - 1] = { negup, negup, Math::Vec2f() };

		std::vector<unsigned short> triangles(vertices.size() * 2 * 3);

		//Top Cap
		int i = 0;
		for (unsigned int lon = 0; lon < slices; lon++)
		{
			triangles[i++] = lon + 2;
			triangles[i++] = lon + 1;
			triangles[i++] = 0;
		}
		//Middle
		for (unsigned int lat = 0; lat < stacks - 1; lat++)
		{
			for (unsigned int lon = 0; lon < slices; lon++)
			{
				int current = lon + lat * (slices + 1) + 1;
				int next = current + slices + 1;

				triangles[i++] = current;
				triangles[i++] = current + 1;
				triangles[i++] = next + 1;

				triangles[i++] = current;
				triangles[i++] = next + 1;
				triangles[i++] = next;
			}
		}
		//Bottom Cap
		for (unsigned int lon = 0; lon < slices; lon++)
		{
			triangles[i++] = vertices.size() - 1;
			triangles[i++] = vertices.size() - (lon + 2) - 1;
			triangles[i++] = vertices.size() - (lon + 1) - 1;
		}

		return FromVertexs(vertices, triangles);
	}

	Mesh* MeshGenerator::Torus(float radius1, float radius2, unsigned int segments, unsigned int sides)
	{
		std::vector<Vertex3D> vertices((segments + 1) * (sides + 1));
		std::vector<Math::Vec3f> majorRing(segments + 1, Math::Vec3f());

		float a = 0.0f;
		float step = 2.0f * PI / segments;
		for (unsigned int i = 0; i <= segments; ++i)
		{
			majorRing[i].x = cos(a) * radius1;
			majorRing[i].y = sin(a) * radius1;
			a += step;
		}

		for (unsigned int seg = 0; seg <= segments; ++seg)
		{
			Math::Vec3f u = ((Math::Vec3f() - majorRing[seg]) * radius2).normalize();
			Math::Vec3f v = Math::Vec3f(0.0f, 0.0f, 1.0f) * radius2;

			float a = 0.0f;
			float step = 2.0f * PI / sides;
			for (unsigned int side = 0; side <= sides; ++side)
			{
				float c = cos(a);
				float s = sin(a);

				Math::Vec3f r = c * u + s * v;

				vertices[seg * (sides + 1) + side] = {
					majorRing[seg] + r,
					r.normalize(),
					Math::Vec2f((float)seg / (float)segments, (float)side / (float)sides)
				};
				a += step;
			}
		}

		std::vector<unsigned short> triangles(segments * sides * 6);
		int offset = 0;
		for (unsigned int seg = 0; seg < segments; ++seg)
		{
			int i1 = seg;
			int i2 = (i1 + 1);

			for (unsigned int side = 0; side < sides; ++side)
			{
				int j1 = side;
				int j2 = (j1 + 1);

				triangles[offset++] = i1 * (sides + 1) + j1;
				triangles[offset++] = i1 * (sides + 1) + j2;
				triangles[offset++] = i2 * (sides + 1) + j1;

				triangles[offset++] = i2 * (sides + 1) + j2;
				triangles[offset++] = i2 * (sides + 1) + j1;
				triangles[offset++] = i1 * (sides + 1) + j2;
			}
		}

		return FromVertexs(vertices, triangles);
	}
} }