#include <random>

#include <OE/Application/Window.hpp>
#include <OE/Application/Context.hpp>
#include <OE/Misc/Ticker.hpp>

#include <OE/UI/Composer.hpp>
#include <OE/UI/Element.hpp>
#include <OE/UI/Text.hpp>

#include <OE/Platform/OpenGL/OpenGL.hpp>
#include <OE/Graphics/2D/SpriteBatcher.hpp>
#include <OE/Graphics/Font.hpp>
#include <OE/Graphics/FontCollection.hpp>
#include <OE/Graphics/2D/TextRenderer2D.hpp>
#include <OE/Graphics/API/FrameBuffer.hpp>
#include <OE/Graphics/TextureBlitter.hpp>
#include <OE/Graphics/DynamicAtlasAllocator.hpp>
#include <OE/Graphics/DynamicAtlas.hpp>

#if OE_WINDOWS
#include <OE/Platform/Windows/WindowWindows.hpp>
#endif

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
	case 2: num_childs = 1/*5*/; max_sz = 25; break;
	}

	for (int i = 0; i < num_childs; i++) {
		Element* child = new Element(); generated++;
		child->setStyle({ Math::Color4f(
			getRandomFloat(),
			getRandomFloat(),
			getRandomFloat(),
			0.5f + getRandomFloat() * 0.5f
		) });
		YGNodeStyleSetPadding(child->m_Node, YGEdgeAll, getRandomFloat() * max_sz);
		YGNodeStyleSetAlignSelf(child->m_Node, YGAlignCenter);
		//YGNodeStyleSetMargin(child->m_Node, YGEdgeEnd, 20);

		generateRandomUI(child, depth + 1);

		parent->addElement(child, 0);
	}
}

int main() {
	using namespace OrbitEngine;
	using namespace OrbitEngine::Application;
	using namespace OrbitEngine::Misc;

	Window* window = new Window();
	window->setTitle("UI Testing");
	window->setVisibility(true);
	
	Context* ctx = new Context(RenderAPI::OPENGL, window);

	Graphics::Font* font = new Graphics::Font("Resources/Fonts/Inter-Regular.ttf");

	Ticker t;

	UI::Element* root = new UI::Element();
	UI::Style root_style;
	root_style.background = Math::Color::Transparent;
	root->setStyle(root_style);
	YGNodeStyleSetFlexDirection(root->m_Node, YGFlexDirectionRow);
	YGNodeStyleSetPadding(root->m_Node, YGEdgeAll, 20);
	YGNodeStyleSetMargin(root->m_Node, YGEdgeAll, 20);

	UI::Element* divA = new UI::Element();
	divA->setStyle({ Math::Color::Red });
	YGNodeStyleSetWidth(divA->m_Node, 80);
	YGNodeStyleSetHeight(divA->m_Node, 80);
	YGNodeStyleSetAlignSelf(divA->m_Node, YGAlignCenter);
	YGNodeStyleSetMargin(divA->m_Node, YGEdgeEnd, 20);

	UI::Text* textA = new UI::Text();
	textA->setFont(font);
	textA->setText("Hello World");
	textA->setStyle({ Math::Color4f(0.3,0.3,0.3,1.0) });
	//YGNodeStyleSetHeight(textA->m_Node, 25);
	YGNodeStyleSetPadding(textA->m_Node, YGEdgeAll, 5);
	YGNodeStyleSetAlignSelf(textA->m_Node, YGAlignCenter);
	//YGNodeStyleSetFlexGrow(textA->m_Node, 1);

	generateRandomUI(root);

	root->addElement(divA, 0);
	root->addElement(textA, 1);

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

	UI::Composer* ui_composer = new UI::Composer();

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

		textA->setSize(12 + abs(sin(time)) * 70);

		ui_composer->render(root, proj, window->getSize());
		
		
		renderer->begin();
		renderer->setPVMatrices(proj);

		renderer->bindColor(Math::Color4f(1, 1, 1, 1));
		renderer->bindTexture(sample_tex);
		renderer->rect(Math::Vec2f(100, 100), Math::Vec2f(32, 32));

		auto atlas = /*tmp_fb->getColorTextures()[0]*/da->getTexture();
		//renderer->bindTexture(atlas);
		//renderer->rect(Math::Vec2f(10,10), 1.0f * Math::Vec2f(atlas->getProperties().width, atlas->getProperties().height));

		renderer->end();
		
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