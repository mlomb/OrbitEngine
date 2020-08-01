#include "OE/UI/Composer.hpp"

#include "OE/Graphics/ShaderLoader.hpp"

#include "OE/UI/Element.hpp"
#include "OE/UI/Painter.hpp"

namespace OrbitEngine { namespace UI {

	Composer::Composer()
	{
		m_Painter = new Painter();
	}

	Composer::~Composer()
	{

	}

	void Composer::render(Element* root, const Math::Mat4& transform, Math::Vec2f availableSize)
	{
		root->layout(availableSize);

		m_Painter->setProjection(transform);
		m_Painter->begin();
		paint(root);
		m_Painter->end();
	}

	void Composer::paint(Element* element)
	{
		element->generateContent(m_Painter);
		for (Element* e : element->m_Childrens)
			paint(e);
	}

} }