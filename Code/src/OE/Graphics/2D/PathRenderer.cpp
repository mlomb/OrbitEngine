#include "OE/Graphics/2D/PathRenderer.hpp"

#include <stdlib.h>
#include <cstring>

#include "OE/Math/Math.hpp"
#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {
	PathRenderer::PathRenderer()
	{
		m_CommandsBuffer = (float*)malloc(sizeof(float)*MAX_DRAW_COMMANDS_BUFFER_SIZE);
		m_Points = (PathPoint*)malloc(sizeof(PathPoint)*MAX_POINTS_BUFFER_SIZE);
		m_Paths = (Path*)malloc(sizeof(Path)*MAX_PATHS_BUFFER_SIZE);

		setDPR(1.0f);
	}

	PathRenderer::~PathRenderer()
	{
		free(m_CommandsBuffer);
		free(m_Paths);
		free(m_Points);
	}

	void PathRenderer::beginFrame()
	{
		m_NPaths = 0;
		m_NPoints = 0;
		m_CommandsSize = 0;
		m_CurrentCall = nullptr;
		m_FillStyle = PaintStyle();
		m_StrokeStyle = PaintStyle();
		m_StrokeWidth = 3.0f;
		p_DirtyState = false;
	}

	void PathRenderer::setDPR(float devicePixelRatio)
	{
		m_TesselationTolerance = 0.25f / devicePixelRatio;
	}

	/* ----------------- Paint ----------------- */
	PaintStyle PathRenderer::SolidColor(const Math::Vec4f& color)
	{
		PaintStyle paint = {};

		paint.paintMat = Math::Mat4::Identity();
		paint.extent = Math::Vec2f(-1, -1);
		paint.radius = 0;
		paint.feather = 1;
		paint.innerColor = paint.outerColor = color;

		return paint;
	}

	PaintStyle PathRenderer::BoxGradient(const Math::Vec2f& position, const Math::Vec2f& size, float radius, float feather, const Math::Vec4f& innerColor, const Math::Vec4f& outerColor)
	{
		Math::Vec2f halfSize = 0.5f * size;

		PaintStyle paint = {};

		paint.paintMat = Math::Mat4::Translation(-1 * Math::Vec2f(position.x + halfSize.x, position.y + halfSize.y));
		paint.extent = halfSize;
		paint.radius = radius;
		paint.feather = std::max(1.0f, feather);
		paint.innerColor = innerColor;
		paint.outerColor = outerColor;

		return paint;
	}

	PaintStyle PathRenderer::LinearGradient(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Vec4f& innerColor, const Math::Vec4f& outerColor)
	{
		const float large = 1e5;

		Math::Vec2f delta = size - position;
		float d = delta.normalize();

		PaintStyle paint = {};
		// TODO
		/*
		paint.paintMat = Math::Mat4::Translation(1 * (position));
		paint.extent = Math::Vec2f(large, large + d * 0.5f);
		*/
		paint.radius = 0;
		paint.feather = std::max(1.0f, d);
		paint.innerColor = innerColor;
		paint.outerColor = outerColor;

		return paint;
	}

	PaintStyle PathRenderer::ImagePattern(const Math::Vec2f& position, const Math::Vec2f& size, unsigned int texture)
	{
		PaintStyle paint = {};

		paint.paintMat = Math::Mat4::Translation(position);
		paint.extent = size;
		paint.innerColor = paint.outerColor = Math::Vec4f(1, 1, 1, 1);
		paint.texid = (float)texture;

		return paint;
	}

	void PathRenderer::shapeCareOfStroke(bool careOfStroke)
	{
		m_CareOfStroke = careOfStroke;
	}

	/* ----------------- Shapes ----------------- */
	void PathRenderer::roundedRect(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Vec4f& radius)
	{
		Math::Vec2f halfSize = 0.5f * size;

		float rxTL = std::min(radius.x, halfSize.x) * Math::sign(size.x), ryTL = std::min(radius.x, halfSize.y) * Math::sign(size.y);
		float rxTR = std::min(radius.y, halfSize.x) * Math::sign(size.x), ryTR = std::min(radius.y, halfSize.y) * Math::sign(size.y);
		float rxBR = std::min(radius.z, halfSize.x) * Math::sign(size.x), ryBR = std::min(radius.z, halfSize.y) * Math::sign(size.y);
		float rxBL = std::min(radius.w, halfSize.x) * Math::sign(size.x), ryBL = std::min(radius.w, halfSize.y) * Math::sign(size.y);

		float x = position.x;
		float y = position.y;
		float w = size.x;
		float h = size.y;

		if (m_CareOfStroke) {
			x += m_StrokeWidth;
			y += m_StrokeWidth;
			w -= m_StrokeWidth * 2;
			h -= m_StrokeWidth * 2;
		}

		float vals[] = {
			MOVETO, x, y + ryTL,
			LINETO, x, y + h - ryBL,
			BEZIERTO, x, y + h - ryBL*(1 - KAPPA90), x + rxBL*(1 - KAPPA90), y + h, x + rxBL, y + h,
			LINETO, x + w - rxBR, y + h,
			BEZIERTO, x + w - rxBR*(1 - KAPPA90), y + h, x + w, y + h - ryBR*(1 - KAPPA90), x + w, y + h - ryBR,
			LINETO, x + w, y + ryTR,
			BEZIERTO, x + w, y + ryTR*(1 - KAPPA90), x + w - rxTR*(1 - KAPPA90), y, x + w - rxTR, y,
			LINETO, x + rxTL, y,
			BEZIERTO, x + rxTL*(1 - KAPPA90), y, x, y + ryTL*(1 - KAPPA90), x, y + ryTL,
			CLOSE
		};

		pushCommands(vals, ARRAY_COUNT(vals));
	}

	void PathRenderer::ellipse(const Math::Vec2f& position, const Math::Vec2f& size)
	{
		Math::Vec2f cornerTopLeft = position - size;
		Math::Vec2f cornerBottomRight = position + size;
		Math::Vec2f kappa90size = KAPPA90 * size;

		float commands[] = {
			MOVETO, cornerTopLeft.x, position.y,
			BEZIERTO, cornerTopLeft.x, position.y + kappa90size.y, position.x - kappa90size.x, cornerBottomRight.y, position.x, cornerBottomRight.y,
			BEZIERTO, position.x + kappa90size.x, cornerBottomRight.y, cornerBottomRight.x, position.y + kappa90size.y, cornerBottomRight.x, position.y,
			BEZIERTO, cornerBottomRight.x, position.y - kappa90size.y, position.x + kappa90size.x, cornerTopLeft.y, position.x, cornerTopLeft.y,
			BEZIERTO, position.x - kappa90size.x, cornerTopLeft.y, cornerTopLeft.x, position.y - kappa90size.y, cornerTopLeft.x, position.y,
			CLOSE
		};

		pushCommands(commands, ARRAY_COUNT(commands));
	}

	void PathRenderer::rect(const Math::Vec2f& position, const Math::Vec2f& size)
	{
		if (size.x == 0 || size.y == 0)
			return;

		float commands[] = {
			MOVETO, position.x, position.y,
			LINETO, position.x, position.y + size.y,
			LINETO, position.x + size.x, position.y + size.y,
			LINETO, position.x + size.x, position.y,
			CLOSE
		};

		pushCommands(commands, ARRAY_COUNT(commands));
	}

	/* ----------------- Path ----------------- */
	void PathRenderer::beginPath()
	{
		m_CommandsSize = 0;
	}

	void PathRenderer::pushCommands(float* data, int size)
	{
		OE_ASSERT(m_CommandsSize + size < sizeof(float) * MAX_DRAW_COMMANDS_BUFFER_SIZE);
		std::memcpy(&m_CommandsBuffer[m_CommandsSize], data, size * sizeof(float));
		m_CommandsSize += size;
	}

	void PathRenderer::moveTo(const Math::Vec2f& position)
	{
		float commands[] = { MOVETO, position.x, position.y };
		pushCommands(commands, ARRAY_COUNT(commands));
	}

	void PathRenderer::lineTo(const Math::Vec2f& position)
	{
		float commands[] = { LINETO, position.x, position.y };
		pushCommands(commands, ARRAY_COUNT(commands));
	}

	void PathRenderer::bezierTo(const Math::Vec2f& c1, const Math::Vec2f& c2, const Math::Vec2f& position)
	{
		float commands[] = { BEZIERTO, c1.x, c1.y, c2.x, c2.y, position.x, position.y };
		pushCommands(commands, ARRAY_COUNT(commands));
	}

	void PathRenderer::closePath()
	{
		float commands[] = { CLOSE };
		pushCommands(commands, ARRAY_COUNT(commands));
	}

	void PathRenderer::computePaths()
	{
		int paths = m_NPaths;
		m_CurrentCall->paths = &m_Paths[paths];
		m_NPoints = 0;

		unsigned int i = 0;
		while (i < m_CommandsSize) {
			PathRendererCommandType command = static_cast<PathRendererCommandType>((int)m_CommandsBuffer[i]);
			switch (command)
			{
			case PathRendererCommandType::MOVETO:
				__addPath();
				// fall
			case PathRendererCommandType::LINETO:
				__addPoint(Math::Vec2f(m_CommandsBuffer[i + 1], m_CommandsBuffer[i + 2]));
				i += 3;
				break;
			case PathRendererCommandType::BEZIERTO:
			{
				PathPoint* lastP = &m_Points[m_NPoints - 1];
				if (lastP != nullptr)
					__tesselateBezier(lastP->position.x, lastP->position.y, m_CommandsBuffer[i + 1], m_CommandsBuffer[i + 2], m_CommandsBuffer[i + 3], m_CommandsBuffer[i + 4], m_CommandsBuffer[i + 5], m_CommandsBuffer[i + 6]);
				i += 7;
				break;
			}
			case PathRendererCommandType::CLOSE:
				m_CurrentPath->closed = true;
				i++;
				break;
			default:
				i++;
			}
		}

		m_CurrentCall->pathsCount = m_NPaths - paths;

		// Reset bounds
		m_Bounds.x = m_Bounds.y = 1e6f;
		m_Bounds.z = m_Bounds.w = -1e6f;

		// Calculate bounds and direction
		Path* p;
		PathPoint* pts;
		PathPoint* p0;
		PathPoint* p1;

		for (i = 0; i < m_CurrentCall->pathsCount; i++) {
			p = &m_CurrentCall->paths[i];
			pts = &m_Points[p->offset];

			p0 = &pts[p->count - 1];
			p1 = &pts[0];

			// Check if start and end points are the 'same' -> remove the last one and mark as closed
			if (p0->equals(p1, 0.001f)) {
				p->count--;
				p->closed = true;
				p0 = &pts[p->count - 1];
			}

			for (int j = 0; j < p->count; j++) {
				m_Bounds.x = std::min(m_Bounds.x, p0->position.x);
				m_Bounds.y = std::min(m_Bounds.y, p0->position.y);
				m_Bounds.z = std::max(m_Bounds.z, p0->position.x);
				m_Bounds.w = std::max(m_Bounds.w, p0->position.y);

				p0->direction = p1->position - p0->position;
				p0->length = p0->direction.normalize();

				p0 = p1++;
			}
		}
	}

	void PathRenderer::computeJoins()
	{
		PathPoint* pts;
		PathPoint* p0;
		PathPoint* p1;
		Path* p;

		for (unsigned int i = 0; i < m_CurrentCall->pathsCount; i++) {
			p = &m_CurrentCall->paths[i];
			pts = &m_Points[p->offset];

			p0 = &pts[p->count - 1];
			p1 = &pts[0];
			for (int j = 0; j < p->count; j++) {
				float dlx0, dly0, dlx1, dly1, dmr2;
				dlx0 = p0->direction.y;
				dly0 = -p0->direction.x;
				dlx1 = p1->direction.y;
				dly1 = -p1->direction.x;
				// Calculate extrusions
				p1->directionm = Math::Vec2f((dlx0 + dlx1) * 0.5f, (dly0 + dly1) * 0.5f);
				dmr2 = p1->directionm.x * p1->directionm.x + p1->directionm.y * p1->directionm.y;
				if (dmr2 > 0.000001f) {
					float scale = 1.0f / dmr2;
					if (scale > 600.0f)
						scale = 600.0f;
					p1->directionm.x *= scale;
					p1->directionm.y *= scale;
				}

				p0 = p1++;
			}
		}
	}

	void PathRenderer::fill()
	{
		__addCall();
		m_CurrentCall->data.paint = m_FillStyle;
		m_CurrentCall->data.strokeMul = 1;

		/* --------- */
		bool fringe = m_AA_Fringe > 0.0f;

		PathPoint* pts;
		PathPoint* p0;
		PathPoint* p1;
		Path* p;

		for (unsigned int i = 0; i < m_CurrentCall->pathsCount; i++) {
			p = &m_CurrentCall->paths[i];
			pts = &m_Points[p->offset];


			float rw, lw;
			float ru, lu;
			float woff = 0.5f * m_AA_Fringe;

			// -- Calculate shape --
			p->fillOffset = getVertexOffset();

			p0 = &pts[p->count - 1];
			p1 = &pts[0];
			for (int j = 0; j < p->count; j++) {
				if (fringe)
					pushVertex(Math::Vec2f(p1->position.x + (p1->directionm.x * woff), p1->position.y + (p1->directionm.y * woff)), Math::Vec2f(0.5f, 1));
				else
					pushVertex(p0->position, Math::Vec2f(0.5f, 1));
				p0 = p1++;
			}

			p->fillCount = getVertexOffset() - p->fillOffset;

			// -- Calculate fringe --
			if (fringe) {
				lw = m_AA_Fringe + woff;
				rw = m_AA_Fringe - woff;
				lu = 0;
				ru = 1;

				p->strokeOffset = getVertexOffset();

				p0 = &pts[p->count - 1];
				p1 = &pts[0];
				for (int j = 0; j < p->count; j++) {
					pushVertex(Math::Vec2f(p1->position.x + (p1->directionm.x * lw), p1->position.y + (p1->directionm.y * lw)), Math::Vec2f(lu, 1));
					pushVertex(Math::Vec2f(p1->position.x - (p1->directionm.x * rw), p1->position.y - (p1->directionm.y * rw)), Math::Vec2f(ru, 1));
					p0 = p1++;
				}

				// Loop
				p1 = &pts[0];
				pushVertex(Math::Vec2f(p1->position.x + (p1->directionm.x * lw), p1->position.y + (p1->directionm.y * lw)), Math::Vec2f(lu, 1));
				pushVertex(Math::Vec2f(p1->position.x - (p1->directionm.x * rw), p1->position.y - (p1->directionm.y * rw)), Math::Vec2f(ru, 1));

				p->strokeCount = getVertexOffset() - p->strokeOffset;
			}
		}

		/* Fill */
		m_CurrentCall->trianglesOffset = getVertexOffset();
		m_CurrentCall->trianglesCount = 6;
		pushVertex(Math::Vec2f(m_Bounds.x, m_Bounds.w), Math::Vec2f(0.5f, 1));
		pushVertex(Math::Vec2f(m_Bounds.z, m_Bounds.w), Math::Vec2f(0.5f, 1));
		pushVertex(Math::Vec2f(m_Bounds.z, m_Bounds.y), Math::Vec2f(0.5f, 1));

		pushVertex(Math::Vec2f(m_Bounds.x, m_Bounds.w), Math::Vec2f(0.5f, 1));
		pushVertex(Math::Vec2f(m_Bounds.z, m_Bounds.y), Math::Vec2f(0.5f, 1));
		pushVertex(Math::Vec2f(m_Bounds.x, m_Bounds.y), Math::Vec2f(0.5f, 1));

		pushCall(m_CurrentCall);
	}

	void PathRenderer::stroke()
	{
		__addCall();
		m_CurrentCall->data.paint = m_StrokeStyle;

		/* --------- */
		float w = m_StrokeWidth * 0.5f + m_AA_Fringe * 0.5f;
		m_CurrentCall->data.strokeMul = (w*0.5f + m_AA_Fringe*0.5f) / m_AA_Fringe;

		PathPoint* pts;
		PathPoint* p0;
		PathPoint* p1;
		Path* p;

		for (unsigned int i = 0; i < m_CurrentCall->pathsCount; i++) {
			p = &m_CurrentCall->paths[i];
			pts = &m_Points[p->offset];

			bool loop = p->closed;
			int s, e;

			p0 = &pts[p->count - 1];
			p1 = &pts[0];
			s = 0;
			e = p->count;
			if (!loop) {

			}

			p->strokeOffset = getVertexOffset();

			for (int j = 0; j < p->count; j++) {
				pushVertex(Math::Vec2f(p1->position.x + (p1->directionm.x * w), p1->position.y + (p1->directionm.y * w)), Math::Vec2f(0, 1));
				pushVertex(Math::Vec2f(p1->position.x - (p1->directionm.x * w), p1->position.y - (p1->directionm.y * w)), Math::Vec2f(1, 1));
				p0 = p1++;
			}

			if (loop) {
				p1 = &pts[0];
				pushVertex(Math::Vec2f(p1->position.x + (p1->directionm.x * w), p1->position.y + (p1->directionm.y * w)), Math::Vec2f(0, 1));
				pushVertex(Math::Vec2f(p1->position.x - (p1->directionm.x * w), p1->position.y - (p1->directionm.y * w)), Math::Vec2f(1, 1));
			}

			p->strokeCount = getVertexOffset() - p->strokeOffset;
		}

		pushCall(m_CurrentCall);
	}

	void PathRenderer::strokeFill()
	{
		stroke();
		fill();
	}

	void PathRenderer::__addCall() {
		m_CurrentCall = new PathRendererCall();
		m_CurrentCall->trianglesOffset = getVertexOffset();
		if (m_CommandsSize > 0) {
			computePaths();
			computeJoins();
		}
	}

	void PathRenderer::__addPath()
	{
		OE_ASSERT(m_NPaths + 1 < MAX_PATHS_BUFFER_SIZE);

		Path* path = &m_Paths[m_NPaths];
		std::memset(path, 0, sizeof(*path));
		path->offset = m_NPoints;

		m_CurrentPath = path;
		m_NPaths++;
	}

	void PathRenderer::__addPoint(const Math::Vec2f& position)
	{
		OE_ASSERT(m_NPoints + 1 < MAX_POINTS_BUFFER_SIZE);

		PathPoint* point = &m_Points[m_NPoints];

		std::memset(point, 0, sizeof(*point));
		point->position = position;

		m_CurrentPath->count++;
		m_NPoints++;
	}

	/* http://www.antigrain.com/research/adaptive_bezier/index.html */
	void PathRenderer::__tesselateBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int level)
	{
		if (level > 10) return;

		float dx, dy, d2, d3;

		dx = x4 - x1;
		dy = y4 - y1;
		d2 = fabs(((x2 - x4) * dy - (y2 - y4) * dx));
		d3 = fabs(((x3 - x4) * dy - (y3 - y4) * dx));

		if ((d2 + d3)*(d2 + d3) < m_TesselationTolerance * (dx*dx + dy*dy)) {
			__addPoint(Math::Vec2f(x4, y4));
			return;
		}

		float x12, y12, x23, y23, x34, y34, x123, y123, x234, y234, x1234, y1234;
		x12 = (x1 + x2)*0.5f;
		y12 = (y1 + y2)*0.5f;
		x23 = (x2 + x3)*0.5f;
		y23 = (y2 + y3)*0.5f;
		x34 = (x3 + x4)*0.5f;
		y34 = (y3 + y4)*0.5f;
		x123 = (x12 + x23)*0.5f;
		y123 = (y12 + y23)*0.5f;
		x234 = (x23 + x34)*0.5f;
		y234 = (y23 + y34)*0.5f;
		x1234 = (x123 + x234)*0.5f;
		y1234 = (y123 + y234)*0.5f;

		__tesselateBezier(x1, y1, x12, y12, x123, y123, x1234, y1234, level + 1);
		__tesselateBezier(x1234, y1234, x234, y234, x34, y34, x4, y4, level + 1);
	}
} }