#ifndef UI_TREE_UPDATER_HPP
#define UI_TREE_UPDATER_HPP

namespace OrbitEngine { namespace UI {

	class Element;
	class Surface;

	enum class VersionChangeType {
		STYLE,
		LAYOUT,
		PAINT
	};

	class TreeUpdater {
	public:
		TreeUpdater(Surface* surface);
		virtual ~TreeUpdater();

		virtual void update() = 0;
		virtual void onVersionChange(Element* element, VersionChangeType changes) = 0;

	protected:
		Surface* m_Surface;
	};

} }

#endif