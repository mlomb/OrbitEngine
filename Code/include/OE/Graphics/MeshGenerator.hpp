#ifndef GRAPHICS_MESH_GENERATOR_HPP
#define GRAPHICS_MESH_GENERATOR_HPP

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/3D/Renderer3D.hpp"

namespace OrbitEngine { namespace Graphics {
	class MeshGenerator {
	public:
		static Mesh* FromVertexs(std::vector<Vertex3D>& verts, float size = 1.0f);
		static Mesh* FromVertexs(std::vector<Vertex3D>& verts, std::vector<unsigned short>& indices, float size = 1.0f);
			   
		static Mesh* Plane(float width = 1, float length = 1, unsigned int resX = 2, unsigned int resZ = 2);
		static Mesh* Plane(float size, int resolution) { return Plane(size, size, resolution, resolution); }
			   
		static Mesh* Box(float width = 1, float height = 1, float length = 1);
			   
		static Mesh* SphereStandard(float radius = 1.0f, unsigned int stacks = 20, unsigned int slices = 20);
			   
		static Mesh* Torus(float radius1, float radius2, unsigned int segments, unsigned int sides);
	};
} }

#endif