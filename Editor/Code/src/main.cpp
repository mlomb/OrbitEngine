#include <random>

#include <OE/Application/Window.hpp>
#include <OE/Application/Context.hpp>
#include <OE/Misc/Ticker.hpp>

//#include <OE/UI/Composer.hpp>
#include <OE/UI/Element.hpp>
//#include <OE/UI/Text.hpp>

#include <OE/Platform/OpenGL/OpenGL.hpp>
#include <OE/Graphics/2D/SpriteBatcher.hpp>
#include <OE/Graphics/Font.hpp>
#include <OE/Graphics/FontCollection.hpp>
#include <OE/Graphics/2D/TextRenderer2D.hpp>
#include <OE/Graphics/API/FrameBuffer.hpp>
#include <OE/Graphics/TextureBlitter.hpp>
#include <OE/Graphics/DynamicAtlasAllocator.hpp>
#include <OE/Graphics/DynamicAtlas.hpp>
#include <OE/Graphics/2D/ImGuiRenderer.hpp>
#include <OE/UI/Style/StyleParse.hpp>
#include <OE/UI/Style/StyleSheet.hpp>
#include <OE/UI/Style/StyleComputed.hpp>
#include <OE/UI/Surface.hpp>
#include <OE/UI/Components/Text.hpp>
#include <OE/UI/Components/Button.hpp>
#include <OE/UI/Events/Events.hpp>
#include <OE/UI/Events/EventsController.hpp>

#if OE_WINDOWS
#include <OE/Platform/Windows/WindowWindows.hpp>
#endif

using namespace OrbitEngine;
using namespace OrbitEngine::Application;
using namespace OrbitEngine::Misc;

static int generated = 0;

float getRandomFloat() {
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(0, 1); // rage 0 - 1
	return dis(e);
}

void generateRandomUI(OrbitEngine::UI::Element* parent, int depth = 0) {
	using namespace OrbitEngine;
	using namespace OrbitEngine::UI;

	int num_childs = 0;
	float max_sz = 0;

	switch (depth)
	{
	case 0: num_childs = 10/*35*/; max_sz = 200; break;
	case 1: num_childs = 3/*20*/; max_sz = 70; break;
	case 2: num_childs = 5; max_sz = 25; break;
	}

	for (int i = 0; i < num_childs; i++) {
		Element* child = new Element(); generated++;
        child->addClass("d" + std::to_string(depth));

        //child->setStyleProperty({ StylePropertyID::PADDING_LEFT, { getRandomFloat() * max_sz, StyleLengthUnit::PIXELS } });
        //child->setStyleProperty({ StylePropertyID::PADDING_TOP, { getRandomFloat() * max_sz, StyleLengthUnit::PIXELS } });
        //child->setStyleProperty({ StylePropertyID::PADDING_RIGHT, { getRandomFloat() * max_sz, StyleLengthUnit::PIXELS } });
        //child->setStyleProperty({ StylePropertyID::PADDING_BOTTOM, { getRandomFloat() * max_sz, StyleLengthUnit::PIXELS } });
        //
        //child->setStyleProperty({ StylePropertyID::MARGIN_LEFT, { 20, StyleLengthUnit::PIXELS } });
        //child->setStyleProperty({ StylePropertyID::MARGIN_TOP, { 20, StyleLengthUnit::PIXELS } });
        //child->setStyleProperty({ StylePropertyID::MARGIN_RIGHT, { 20, StyleLengthUnit::PIXELS } });
        //child->setStyleProperty({ StylePropertyID::MARGIN_BOTTOM, { 20, StyleLengthUnit::PIXELS } });
        
        /*StyleProperty bg_prop;
        bg_prop.id = StylePropertyID::BACKGROUND_COLOR;
        bg_prop.value.color = {
            getRandomFloat(),
            getRandomFloat(),
            getRandomFloat(),
            0.5f + getRandomFloat() * 0.5f
        };
        child->setStyleProperty(bg_prop);*/
		//YGNodeStyleSetPadding(child->m_Node, YGEdgeAll, getRandomFloat() * max_sz);
		//YGNodeStyleSetAlignSelf(child->m_Node, YGAlignCenter);
		//YGNodeStyleSetMargin(child->m_Node, YGEdgeEnd, 20);

		generateRandomUI(child, depth + 1);

		parent->addElement(child, 0);
	}
}

void GenerateDemoPanel(UI::Element* root, Graphics::Font* tmp_font) {
	UI::Element* container = new UI::Element();
	container->setID("demo");

	UI::Button* btn = new UI::Button();
	btn->setFont(tmp_font);
	btn->setText("Button #1");
	container->addElement(btn, 0);
	UI::Button* btn2 = new UI::Button();
	btn2->setFont(tmp_font);
	btn2->setText("Button #2");
	container->addElement(btn2, 0);

	root->addElement(container, 0);
}

std::string test_css();

int main() {
    OE_LOG_DEBUG("sizeof(UI::Element): " << sizeof(UI::Element));
    OE_LOG_DEBUG("sizeof(UI::StyleComputed): " << sizeof(UI::StyleComputed));

	// CSS testing
	std::string css_source = test_css();
    UI::StyleParseResult pr;
    UI::StyleSheet* ss = UI::ParseStyleSheet(css_source, pr);

    for(auto s : pr.warnings)
        OE_LOG_WARNING(s);
    for (auto s : pr.errors)
		OE_LOG_ERROR(s);
    //return 0;
	 
    UI::Element* se = new UI::Element();
    se->setID("test");

    UI::Element* se2 = new UI::Element();
    se2->addClass("test");

    se->addElement(se2, 0);

    UI::StyleSelector sel;
    UI::ParseSingleStyleSelector("StyledElement", sel, pr);

    //bool m = se2->matchesSelector(&sel);

	Window* window = new Window();
	window->setTitle("UI Testing");
	window->setVisibility(true);
	
	Context* ctx = new Context(RenderAPI::OPENGL, window);

	Graphics::Font* font = new Graphics::Font("Resources/Fonts/Inter-Regular.ttf");

	Ticker t;

    UI::Surface* surface = new UI::Surface();
	UI::Element* root = surface->getRoot();
    root->setID("root");

    root->addStyleSheet(ss);

	//UI::Style root_style;
	//root_style.background = Math::Color::Transparent;
	//root->setStyle(root_style);
    /*
	YGNodeStyleSetFlexDirection(root->m_Node, YGFlexDirectionRow);
	YGNodeStyleSetPadding(root->m_Node, YGEdgeAll, 20);
	YGNodeStyleSetMargin(root->m_Node, YGEdgeAll, 20);

	UI::Element* divA = new UI::Element();
	//divA->setStyle({ Math::Color::Red });
	YGNodeStyleSetWidth(divA->m_Node, 80);
	YGNodeStyleSetHeight(divA->m_Node, 80);
	YGNodeStyleSetAlignSelf(divA->m_Node, YGAlignCenter);
	YGNodeStyleSetMargin(divA->m_Node, YGEdgeEnd, 20);
    */

	UI::Text* textA = new UI::Text();
	textA->setFont(font);
	textA->setText("Hello World");
	//textA->setStyle({ Math::Color4f(0.3,0.3,0.3,1.0) });
	//YGNodeStyleSetHeight(textA->m_Node, 25);
	//YGNodeStyleSetAlignSelf(textA->m_Node, YGAlignCenter);
	//YGNodeStyleSetMargin(textA->m_Node, YGEdgeRight, 10);
	//YGNodeStyleSetFlexGrow(textA->m_Node, 1);

	//generateRandomUI(root);

	//root->addElement(divA, 0);
	root->addElement(textA, 0);

	UI::Text* textLong = new UI::Text();
	textLong->setFont(font);
	textLong->setText(R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec nec dui elementum, ultricies leo in, porttitor elit. Aenean in orci sit amet diam suscipit eleifend. Nam cursus leo ac tincidunt vehicula. Ut id lobortis risus, nec pulvinar risus. Nulla finibus enim et risus vulputate hendrerit. Quisque ultrices urna at euismod dapibus. Nulla dapibus justo eget viverra varius. Aliquam enim tortor, pellentesque id imperdiet non, vehicula sed risus. Donec dignissim ultrices mauris, et consectetur tellus scelerisque id. Aenean iaculis neque eget ipsum posuere rutrum. Pellentesque mattis suscipit arcu in dapibus. Cras ac scelerisque lacus. Sed sed turpis a erat mollis vestibulum.
Vivamus erat nisl, fermentum ac sagittis quis, rutrum nec quam. Vivamus aliquet, orci in congue dignissim, nisi lectus posuere quam, id molestie quam augue sed justo. Mauris vehicula finibus sagittis. Mauris posuere mattis mi, et faucibus ipsum hendrerit in. Nulla non diam ultricies, aliquet est quis, bibendum urna. Nunc odio est, volutpat ut fermentum semper, lacinia in diam. Maecenas semper tellus diam, sed accumsan dolor efficitur id. Nulla interdum sapien dapibus, maximus purus sit amet, blandit velit. Sed suscipit libero eu sodales lobortis.
Sed fringilla lacus sed eros molestie tristique. Nullam vitae tortor pharetra, blandit nisi venenatis, porta nibh. Ut quis nisi pharetra, ullamcorper lorem ac, scelerisque tellus. Sed facilisis egestas neque quis pellentesque. Ut quam massa, pretium vitae felis in, consectetur volutpat metus. Proin fermentum urna eget tempus volutpat. Proin quis nibh ut justo rutrum vehicula. Nunc a purus at leo efficitur feugiat. Donec id felis pretium, aliquet eros ac, tincidunt augue. Proin dolor sem, dapibus nec aliquam a, blandit a justo. Fusce condimentum magna lorem, non pellentesque turpis congue at. Etiam tristique dolor vel molestie cursus. Cras ipsum arcu, bibendum ac malesuada sit amet, fringilla et dolor. Duis faucibus efficitur dictum. Quisque dolor est libero.)");
	//textLong->setStyle({ Math::Color4f(0.3,0.3,0.3,1.0) });
	//YGNodeStyleSetHeight(textA->m_Node, 25);
	//YGNodeStyleSetAlignSelf(textLong->m_Node, YGAlignCenter);
	//YGNodeStyleSetMaxWidth(textLong->m_Node, 500);
	root->addElement(textLong, 0);
    
	// Button
	UI::Button* btn = new UI::Button();
	btn->setFont(font);
	btn->setText("Button #1");
	root->addElement(btn, 1);

	GenerateDemoPanel(root, font);
    generateRandomUI(root);

	OE_LOG_DEBUG("UI ELEMENTS GENERATED: " << generated);

	Graphics::SpriteBatcher* renderer = new Graphics::SpriteBatcher();
	Graphics::TextRenderer2D* tr2d = new Graphics::TextRenderer2D();

	Graphics::FontCollection* collection = new Graphics::FontCollection();
	std::vector<Graphics::GlyphCodepoint> editor_glyphs;
	for (Graphics::GlyphCodepoint gcp : font->getAvailableGlyphs())
		if (gcp < 256)
			editor_glyphs.push_back(gcp);

	collection->addGlyphs(font, editor_glyphs, 12, Graphics::GlyphRenderMode::COLOR);
	collection->exportToFiles("cache/inter-16-font.json", "cache/inter-16-atlas.json", "cache/inter-16-image.png");
	delete collection;

	Graphics::FontCollection* coll = Graphics::FontCollection::Load("cache/inter-16-font.json", "cache/inter-16-atlas.json", "cache/inter-16-image.png");

	float time = 0.0f;

	//UI::Composer* ui_composer = new UI::Composer();

	Graphics::FrameBuffer* tmp_fb = Graphics::FrameBuffer::Create(4096, 4096);
	Graphics::TextureFormatProperties props;
	props.format = Graphics::RGBA;
	props.dataType = Graphics::UNSIGNED_BYTE;
	//tmp_fb->setClearColor(Math::Vec4f(1.0, 0.0, 1.0, 1.0));
	tmp_fb->attachColorTextures(1, props);
	tmp_fb->finalize();
	tmp_fb->clear();

	Graphics::Texture* sample_tex = Graphics::Texture::Load("sample.png");

	Graphics::TextureBlitter* blitter = new Graphics::TextureBlitter();
	Graphics::DynamicAtlasAllocator* atlas_alloc = new Graphics::DynamicAtlasAllocator();
	
	/*
	for (int i = 0; i < 70; i++) {
		unsigned int lx, ly;
		if (atlas_alloc->tryAllocate(32.0f * getRandomFloat(), getRandomFloat()*32.0f, lx, ly)) {
			Graphics::BlitInfo bi;
			bi.src = sample_tex;
			bi.dstPos = Math::Vec2i(lx, ly);
			blitter->queueBlit(bi);
		}
	}
	*/
	/*
	std::vector<Graphics::Texture*> gen_texs;
	for (int i = 12; i < 80; i++) {
		for (int c = 32; c <= 127; c++) {
			Graphics::BitmapRGBA bitmap;
			Graphics::GlyphMetrics metrics;
			if (font->getGlyph(c, i, Graphics::GlyphRenderMode::GRAY, bitmap, metrics)) {
				Graphics::Texture* tex = bitmap.toTexture();

				unsigned int lx, ly;
				if (atlas_alloc->tryAllocate(tex->getProperties().width, tex->getProperties().height, lx, ly)) {
					Graphics::BlitInfo bi;
					bi.src = tex;
					bi.dstPos = Math::Vec2i(lx, ly);
					blitter->queueBlit(bi);
				}

				gen_texs.push_back(tex);
			}
		}
	}
	
	blitter->commit(tmp_fb);

	for (Graphics::Texture* tex : gen_texs)
		delete tex;
	gen_texs.clear();
	*/

	Graphics::DynamicAtlas* da = new Graphics::DynamicAtlas();
	Graphics::ImGuiRenderer* igr = new Graphics::ImGuiRenderer();

	auto im = InputManager::Get();
	im->onMouseMoveEvent.AddListener([&](const MouseMoveInputEvent& mme) {
		surface->getEventsController()->sendMouseMove(mme.position);
	});

	auto frame = [&]() {
		window->processEvents();
		t.tick();
		ctx->resizeContext(window->getSize());
		ctx->prepare();

		float dt = t.getFrameDelta();
		time += dt;

		// render
		auto proj = Math::Mat4::Orthographic(0, window->getSize().w, window->getSize().h, 0, 0, 1);

		/*
		int i = 12 + (80 - 12) * getRandomFloat();
		int c = 32 + (127 - 32) * getRandomFloat();
		int idx = i * 1000 + c;

		Graphics::BitmapRGBA bitmap;
		Graphics::GlyphMetrics metrics;
		if (font->getGlyph(c, i, Graphics::GlyphRenderMode::GRAY, bitmap, metrics)) {
			Graphics::Texture* tex = bitmap.toTexture();
			Math::Recti r;
			if (da->add(idx, r, tex)) {

			}
			da->commit();
			delete tex;
		}
		*/
		/*renderer->begin();
		renderer->end();*/
		Graphics::FrameBuffer::Prepare();

		//YGNodeStyleSetMaxWidth(textLong->m_Node, 500 + abs(sin(time)) * 500);
		//textA->setSize(12 + abs(sin(time)) * 70);
		//textA->setSize(16);
		//textLong->setSize(12 + abs(sin(3 * time)) * 12);

		igr->begin();

		//ui_composer->render(root, proj, window->getSize());
	
        surface->setSize(window->getSize());
        surface->updateTree();

		Cursor appCursor = Cursor::HIDDEN;
		switch (surface->getCurrentCusor()) {
		default:
		case UI::StyleCursor::AUTO:
		case UI::StyleCursor::DEFAULT:
			appCursor = Cursor::DEFAULT;
			break;
		case UI::StyleCursor::POINTER:
			appCursor = Cursor::HAND;
			break;
		}
		window->setCursor(appCursor);
		
		igr->end();
		
		/*
		renderer->begin();
		renderer->setPVMatrices(proj);


		renderer->bindColor(Math::Color4f(1, 1, 1, 1));
		renderer->bindTexture(sample_tex);
		renderer->rect(Math::Vec2f(100, 100), Math::Vec2f(32, 32));

		auto atlas = /*tmp_fb->getColorTextures()[0]* /da->getTexture();
		//renderer->bindTexture(atlas);
		//renderer->rect(Math::Vec2f(10,10), 1.0f * Math::Vec2f(atlas->getProperties().width, atlas->getProperties().height));

		renderer->end();
		*/
		
		/*
		tr2d->begin();
		tr2d->setPVMatrices(proj);
		Graphics::TextStyle ts;
		ts.size = abs(sin(time) * 80) + 16;
		ts.color = 0xFFFFFFFF;
		//tr2d->drawText(txt, Math::Vec2f(100, 100), ts, coll);
		float y = 10;
		for (int i = 0; i < 60; i++) {
			ts.size = i;
			std::vector<Graphics::GlyphCodepoint> txt;
			std::string str = "Untitled   " + std::to_string(i) + "px";
			for (char c : str)
				txt.push_back(c);
			tr2d->drawText(txt, Math::Vec2f(100, y), ts, coll);
			y += i;
		}
		tr2d->end();
		*/

		ctx->present();
	};

	#if OE_WINDOWS
	priv::WindowWindows* winWindow = static_cast<priv::WindowWindows*>(window->getPimpl());

	winWindow->setResizingCallback(frame);
	#endif

	while (!window->destroyRequested())
		frame();

	return 0;
}

std::string test_css() {
	return R"(

* {
    background-color: red;
}

#root {
    flex-direction: row;
    flex-wrap: wrap;
}

.d0, .d1, .d2, .d3 {
    align-self: center;
    flex-direction: row;
    padding: 8px;
}

.d0, .d2, {
    flex-direction: row;
}
.d1, .d3, {
    flex-direction: column;
}


* {
    background-color: rgba(255,0,255,0.3);
    font-size: 5px;
}

Text {
    font-size: 20px;
    max-width: 500px;
    
    background-color: gray;
}


.d0 { background-color: rgba(255,0,255,0.3); }
.d1 { background-color: rgba(255,0,255,0.5); }
.d2 { background-color: rgba(255,0,255,0.7); }

*:hover {
	background-color: rgba(255, 165, 0, 0.3);
}

#root {
    background-color: transparent;
}

/* REAL STYLES */

* {
    font-size: 13px;
}

Button {
    color: #D9D9D9;
    background-color: #656565;
    padding-left: 3px;
    padding-top: 0;
    padding-right: 3px;
    padding-bottom: 1px;
	border-color: #242424;
	border-width: 1px;
    border-radius: 3px;
	cursor: pointer;
}

Button:hover {
    background-color: #828282;
}

/* DEMO */

#demo {
	margin: 5px;
	flex-direction: column;
	align-items: center;
    justify-content: space-evenly;
    background-color: transparent;
}

#demo:hover {
    background-color: transparent;
}

)";
}