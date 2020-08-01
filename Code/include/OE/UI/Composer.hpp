#ifndef UI_COMPOSER_HPP
#define UI_COMPOSER_HPP

#include <OE/Math/Mat4.hpp>

#include <OE/Graphics/API/Buffer.hpp>
#include <OE/Graphics/API/Shader.hpp>

namespace OrbitEngine { namespace UI {

	class Element;
	class Painter;

	class Composer {
	public:
		Composer();
		~Composer();

		void render(Element* root, const Math::Mat4& transform, Math::Vec2f availableSize);

	private:
		Painter* m_Painter;

		void paint(Element* element);
	};

} }

#endif