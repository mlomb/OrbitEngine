#include "OE/Graphics/3D/BRDFLUT.hpp"

#include "OE/Graphics/API/FrameBuffer.hpp"
#include "OE/Graphics/API/Shader.hpp"

#include "OE/Graphics/ShaderLoader.hpp"
#include "OE/Graphics/2D/Renderer2D.hpp"

namespace OrbitEngine { namespace Graphics {
	BRDFLUT::BRDFLUT(unsigned short integratedBRDFSize)
		: m_BRDFFrameBuffer(0)
	{
		TextureFormatProperties formatProperties;
		formatProperties.format = TextureFormat::RGB8;
		formatProperties.dataType = TextureDataType::UNSIGNED_BYTE;

		m_BRDFFrameBuffer = FrameBuffer::Create(integratedBRDFSize, integratedBRDFSize);
		m_BRDFFrameBuffer->attachColorTextures(1, formatProperties);
		m_BRDFFrameBuffer->finalize();

		Shader* shader = ShaderLoader::IntegrateBRDF();

		FrameBuffer::Push(m_BRDFFrameBuffer);
		FrameBuffer::Prepare();
		shader->bind();

		Renderer2D::RenderQuadScreen();

		shader->unbind();
		FrameBuffer::Pop();

		p_Texture = m_BRDFFrameBuffer->getColorTextures()[0];

		delete shader;
	}

	BRDFLUT::~BRDFLUT()
	{
		if (m_BRDFFrameBuffer)
			delete m_BRDFFrameBuffer;
	}
} }