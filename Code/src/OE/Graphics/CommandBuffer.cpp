#include "OE/Graphics/CommandBuffer.hpp"

namespace OrbitEngine { namespace Graphics {
	CommandBuffer::CommandBuffer()
	{
	}

	CommandBuffer::~CommandBuffer()
	{
	}

	void CommandBuffer::Add(const RenderCommand& cmd)
	{
		m_Commands.push_back(cmd);
	}

	void CommandBuffer::Sort()
	{
		// slow sort for now
	}
} }