#ifndef GRAPHICS_COMMAND_BUFFER_HPP
#define GRAPHICS_COMMAND_BUFFER_HPP

#include "OE/Graphics/API/Mesh.hpp"
#include "OE/Graphics/3D/Material.hpp"
#include "OE/Math/Mat4.hpp"

namespace OrbitEngine { namespace Graphics {

	struct RenderCommand {
		Mesh* mesh;
		Material* material;
		Math::Mat4 transform;
		unsigned int offset, count;
	};

	class CommandBuffer {
	public:
		CommandBuffer();
		~CommandBuffer();

		void Add(const RenderCommand& cmd);
		void Sort();

	private:
		std::vector<RenderCommand> m_Commands;
	};
} }

#endif