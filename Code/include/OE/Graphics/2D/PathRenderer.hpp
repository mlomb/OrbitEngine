#ifndef GRAPHICS_PATH_RENDERER_HPP
#define GRAPHICS_PATH_RENDERER_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Vec4.hpp"
#include "OE/Math/Mat4.hpp"
#include "OE/Math/Scissor.hpp"

#define MAX_DRAW_COMMANDS_BUFFER_SIZE 50000
#define MAX_POINTS_BUFFER_SIZE 50000
#define MAX_PATHS_BUFFER_SIZE 5000

/* Based on NanoVG implementation */

namespace OrbitEngine { namespace Graphics {
	enum PathRendererCommandType {
		MOVETO = 0,
		LINETO,
		BEZIERTO,
		CLOSE
	};

	struct PathPoint {
		Math::Vec2f position;
		Math::Vec2f direction;
		Math::Vec2f directionm;
		float length;

		bool equals(const PathPoint* b, float tolerance) {
			float deltax = b->position.x - position.x;
			float deltay = b->position.y - position.y;
			float dist = sqrt((deltax * deltax) + (deltay * deltay));
			return dist < tolerance;
		}
	};

	struct Path {
		bool closed = false;

		int offset, count;
		int fillOffset, fillCount;
		int strokeOffset, strokeCount;
	};

	struct PaintStyle {
		Math::Mat4 paintMat;
		Math::Vec4f innerColor = Math::Vec4f(1, 1, 1, 1);
		Math::Vec4f outerColor = Math::Vec4f(0, 0, 0, 1);
		Math::Vec2f extent;
		float radius = 0;
		float feather = 1;
		float texid = 0;
		float pad1, pad2, pad3;
	};

	struct PathRendererUniformData {
		//Math::Scissor scissor;
		Math::Vec4f scissor = Math::Vec4f(0, 0, 32000, 32000);
		PaintStyle paint;
		float texid = 0;
		float strokeMul;
		float pad4, pad5;
	};

	struct PathRendererCall {
		Path* paths;
		unsigned int pathsCount;
		unsigned int trianglesOffset, trianglesCount;
		PathRendererUniformData data;
	};

	class PathRenderer {
	public:
		static PaintStyle SolidColor(const Math::Vec4f& color);
		static PaintStyle BoxGradient(const Math::Vec2f& position, const Math::Vec2f& size, float radius, float feather, const Math::Vec4f& innerColor, const Math::Vec4f& outerColor);
		static PaintStyle LinearGradient(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Vec4f& innerColor, const Math::Vec4f& outerColor);
		static PaintStyle ImagePattern(const Math::Vec2f& position, const Math::Vec2f& size, unsigned int texture);

		void beginFrame();

		void setDPR(float devicePixelRatio);

		/* Create path */
		void pushCommands(float* data, int count);

		void beginPath();
		void moveTo(const Math::Vec2f& position);
		void lineTo(const Math::Vec2f& position);
		void bezierTo(const Math::Vec2f& position, const Math::Vec2f& c1, const Math::Vec2f& c2);
		void closePath();

		/* From Shapes */
		void shapeCareOfStroke(bool careOfStroke);
		void roundedRect(const Math::Vec2f& position, const Math::Vec2f& size, const Math::Vec4f& radius);
		void ellipse(const Math::Vec2f& position, const Math::Vec2f& size);
		void rect(const Math::Vec2f& position, const Math::Vec2f& size);
		void roundedRect(const Math::Vec2f& position, const Math::Vec2f& size, float radius) { return roundedRect(position, size, Math::Vec4f(radius, radius, radius, radius)); };
		void circle(const Math::Vec2f& position, float radius) { return ellipse(position, Math::Vec2f(radius, radius)); }

		/* Change style */
		void fillStyle(const PaintStyle& p) { m_FillStyle = p; p_DirtyState = true; };
		void fillColor(const Math::Vec4f& color) { fillStyle(SolidColor(color)); };
		void strokeStyle(const PaintStyle& p) { m_StrokeStyle = p; p_DirtyState = true; };
		void strokeColor(const Math::Vec4f& color) { strokeStyle(SolidColor(color)); };
		void strokeWidth(float strokeWidth) { m_StrokeWidth = strokeWidth; p_DirtyState = true; };

		PaintStyle fillStyle() { return m_FillStyle; }
		PaintStyle strokeStyle() { return m_StrokeStyle; }

		/* Fill or stroke */
		void fill();
		void stroke();
		void strokeFill();
	private:
		float* m_CommandsBuffer;
		unsigned int m_CommandsSize;

		float m_AA_Fringe = 1.0f;
		float m_CareOfStroke = false;

		// Transform the current commands to paths and fill points cache
		void computePaths();
		void computeJoins();
		void __addCall();
		void __addPath();
		void __addPoint(const Math::Vec2f& position);
		void __tesselateBezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int level = 0);

		/* DPR */
		float m_TesselationTolerance;

		/* -- Cache -- */
		PathRendererCall* m_CurrentCall;
		Path* m_CurrentPath;
		Math::Vec4f m_Bounds;

		Path* m_Paths;
		PathPoint* m_Points;
		unsigned int m_NPaths, m_NPoints;

		PaintStyle m_FillStyle;
		PaintStyle m_StrokeStyle;
		float m_StrokeWidth;
	protected:
		PathRenderer();
		~PathRenderer();

		bool p_DirtyState;

		virtual void pushVertex(const Math::Vec2f& position, const Math::Vec2f& uv) = 0;
		virtual void pushCall(PathRendererCall* call) = 0;
		virtual unsigned int getVertexOffset() = 0;
	};
} }

#endif