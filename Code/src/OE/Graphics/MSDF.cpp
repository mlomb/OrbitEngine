#include "OE/Graphics/MSDF.hpp"

#include "OE/Math/Math.hpp"

// FreeType
#include <ft2build.h>
#include FT_OUTLINE_H

// msdfgen
#undef INFINITE
#include <msdfgen.h>
#include <core/Shape.h>

////////////////////////////////////////////////////////////
// The following code has been taken from import-font.cpp //
////////////////////////////////////////////////////////////

namespace msdfgen {
	struct FtContext {
		Point2 position;
		Shape* shape;
		Contour* contour;
	};

	static Point2 ftPoint2(const FT_Vector& vector) {
		return Point2(vector.x / 64., vector.y / 64.);
	}

	static int ftMoveTo(const FT_Vector* to, void* user) {
		FtContext* context = reinterpret_cast<FtContext*>(user);
		context->contour = &context->shape->addContour();
		context->position = ftPoint2(*to);
		return 0;
	}

	static int ftLineTo(const FT_Vector* to, void* user) {
		FtContext* context = reinterpret_cast<FtContext*>(user);
		context->contour->addEdge(new LinearSegment(context->position, ftPoint2(*to)));
		context->position = ftPoint2(*to);
		return 0;
	}

	static int ftConicTo(const FT_Vector* control, const FT_Vector* to, void* user) {
		FtContext* context = reinterpret_cast<FtContext*>(user);
		context->contour->addEdge(new QuadraticSegment(context->position, ftPoint2(*control), ftPoint2(*to)));
		context->position = ftPoint2(*to);
		return 0;
	}

	static int ftCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user) {
		FtContext* context = reinterpret_cast<FtContext*>(user);
		context->contour->addEdge(new CubicSegment(context->position, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
		context->position = ftPoint2(*to);
		return 0;
	}

	bool outlineToShape(FT_Outline* outline, Shape& output) {
		if (!outline)
			return false;

		output.contours.clear();
		output.inverseYAxis = false;

		FtContext context = { };
		context.shape = &output;
		FT_Outline_Funcs ftFunctions;
		ftFunctions.move_to = &ftMoveTo;
		ftFunctions.line_to = &ftLineTo;
		ftFunctions.conic_to = &ftConicTo;
		ftFunctions.cubic_to = &ftCubicTo;
		ftFunctions.shift = 0;
		ftFunctions.delta = 0;
		FT_Error error = FT_Outline_Decompose(outline, &ftFunctions, &context);
		if (error)
			return false;
		return true;
	}
}

///////////////////////////////////////////////////////////
// The following code has been taken from import-svg.cpp //
///////////////////////////////////////////////////////////

namespace msdfgen {
	#define REQUIRE(cond) { if (!(cond)) return false; }
	#define ARC_SEGMENTS_PER_PI 2
	#define ENDPOINT_SNAP_RANGE_PROPORTION (1/16384.)
	#define M_PI PI

	static void skipExtraChars(const char*& pathDef) {
		while (*pathDef == ',' || *pathDef == ' ' || *pathDef == '\t' || *pathDef == '\r' || *pathDef == '\n')
			++pathDef;
	}

	static bool readNodeType(char& output, const char*& pathDef) {
		skipExtraChars(pathDef);
		char nodeType = *pathDef;
		if (nodeType && nodeType != '+' && nodeType != '-' && nodeType != '.' && nodeType != ',' && (nodeType < '0' || nodeType > '9')) {
			++pathDef;
			output = nodeType;
			return true;
		}
		return false;
	}

	static bool readCoord(Point2& output, const char*& pathDef, double scale) {
		skipExtraChars(pathDef);
		int shift;
		double x, y;
		if (sscanf(pathDef, "%lf%lf%n", &x, &y, &shift) == 2 || sscanf(pathDef, "%lf , %lf%n", &x, &y, &shift) == 2) {
			output.x = x * scale;
			output.y = y * scale;
			pathDef += shift;
			return true;
		}
		return false;
	}

	static bool readDouble(double& output, const char*& pathDef) {
		skipExtraChars(pathDef);
		int shift;
		double v;
		if (sscanf(pathDef, "%lf%n", &v, &shift) == 1) {
			pathDef += shift;
			output = v;
			return true;
		}
		return false;
	}

	static bool readBool(bool& output, const char*& pathDef) {
		skipExtraChars(pathDef);
		int shift;
		int v;
		if (sscanf(pathDef, "%d%n", &v, &shift) == 1) {
			pathDef += shift;
			output = v != 0;
			return true;
		}
		return false;
	}

	static double arcAngle(Vector2 u, Vector2 v) {
		return nonZeroSign(crossProduct(u, v)) * acos(clamp(dotProduct(u, v) / (u.length() * v.length()), -1., +1.));
	}

	static Vector2 rotateVector(Vector2 v, Vector2 direction) {
		return Vector2(direction.x * v.x - direction.y * v.y, direction.y * v.x + direction.x * v.y);
	}

	static void addArcApproximate(Contour& contour, Point2 startPoint, Point2 endPoint, Vector2 radius, double rotation, bool largeArc, bool sweep) {
		if (endPoint == startPoint)
			return;
		if (radius.x == 0 || radius.y == 0)
			return contour.addEdge(new LinearSegment(startPoint, endPoint));

		radius.x = fabs(radius.x);
		radius.y = fabs(radius.y);
		Vector2 axis(cos(rotation), sin(rotation));

		Vector2 rm = rotateVector(.5 * (startPoint - endPoint), Vector2(axis.x, -axis.y));
		Vector2 rm2 = rm * rm;
		Vector2 radius2 = radius * radius;
		double radiusGap = rm2.x / radius2.x + rm2.y / radius2.y;
		if (radiusGap > 1) {
			radius *= sqrt(radiusGap);
			radius2 = radius * radius;
		}
		double dq = (radius2.x * rm2.y + radius2.y * rm2.x);
		double pq = radius2.x * radius2.y / dq - 1;
		double q = (largeArc == sweep ? -1 : +1) * sqrt(max(pq, 0.));
		Vector2 rc(q * radius.x * rm.y / radius.y, -q * radius.y * rm.x / radius.x);
		Point2 center = .5 * (startPoint + endPoint) + rotateVector(rc, axis);

		double angleStart = arcAngle(Vector2(1, 0), (rm - rc) / radius);
		double angleExtent = arcAngle((rm - rc) / radius, (-rm - rc) / radius);
		if (!sweep && angleExtent > 0)
			angleExtent -= 2 * M_PI;
		else if (sweep && angleExtent < 0)
			angleExtent += 2 * M_PI;

		int segments = (int)ceil(ARC_SEGMENTS_PER_PI / M_PI * fabs(angleExtent));
		double angleIncrement = angleExtent / segments;
		double cl = 4 / 3. * sin(.5 * angleIncrement) / (1 + cos(.5 * angleIncrement));

		Point2 prevNode = startPoint;
		double angle = angleStart;
		for (int i = 0; i < segments; ++i) {
			Point2 controlPoint[2];
			Vector2 d(cos(angle), sin(angle));
			controlPoint[0] = center + rotateVector(Vector2(d.x - cl * d.y, d.y + cl * d.x) * radius, axis);
			angle += angleIncrement;
			d.set(cos(angle), sin(angle));
			controlPoint[1] = center + rotateVector(Vector2(d.x + cl * d.y, d.y - cl * d.x) * radius, axis);
			Point2 node = i == segments - 1 ? endPoint : center + rotateVector(d * radius, axis);
			contour.addEdge(new CubicSegment(prevNode, controlPoint[0], controlPoint[1], node));
			prevNode = node;
		}
	}

	static bool buildFromPath(msdfgen::Shape& shape, const char* pathDef, double scale) {
		char nodeType = '\0';
		char prevNodeType = '\0';
		msdfgen::Point2 prevNode(0, 0);
		bool nodeTypePreread = false;
		while (nodeTypePreread || readNodeType(nodeType, pathDef)) {
			nodeTypePreread = false;
			msdfgen::Contour& contour = shape.addContour();
			bool contourStart = true;

			msdfgen::Point2 startPoint;
			msdfgen::Point2 controlPoint[2];
			msdfgen::Point2 node;

			while (*pathDef) {
				switch (nodeType) {
				case 'M': case 'm':
					if (!contourStart) {
						nodeTypePreread = true;
						goto NEXT_CONTOUR;
					}
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 'm')
						node += prevNode;
					startPoint = node;
					--nodeType; // to 'L' or 'l'
					break;
				case 'Z': case 'z':
					REQUIRE(!contourStart);
					goto NEXT_CONTOUR;
				case 'L': case 'l':
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 'l')
						node += prevNode;
					contour.addEdge(new msdfgen::LinearSegment(prevNode, node));
					break;
				case 'H': case 'h':
					REQUIRE(readDouble(node.x, pathDef));
					if (nodeType == 'h')
						node.x += prevNode.x;
					contour.addEdge(new msdfgen::LinearSegment(prevNode, node));
					break;
				case 'V': case 'v':
					REQUIRE(readDouble(node.y, pathDef));
					if (nodeType == 'v')
						node.y += prevNode.y;
					contour.addEdge(new msdfgen::LinearSegment(prevNode, node));
					break;
				case 'Q': case 'q':
					REQUIRE(readCoord(controlPoint[0], pathDef, scale));
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 'q') {
						controlPoint[0] += prevNode;
						node += prevNode;
					}
					contour.addEdge(new msdfgen::QuadraticSegment(prevNode, controlPoint[0], node));
					break;
				case 'T': case 't':
					if (prevNodeType == 'Q' || prevNodeType == 'q' || prevNodeType == 'T' || prevNodeType == 't')
						controlPoint[0] = node + node - controlPoint[0];
					else
						controlPoint[0] = node;
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 't')
						node += prevNode;
					contour.addEdge(new msdfgen::QuadraticSegment(prevNode, controlPoint[0], node));
					break;
				case 'C': case 'c':
					REQUIRE(readCoord(controlPoint[0], pathDef, scale));
					REQUIRE(readCoord(controlPoint[1], pathDef, scale));
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 'c') {
						controlPoint[0] += prevNode;
						controlPoint[1] += prevNode;
						node += prevNode;
					}
					contour.addEdge(new msdfgen::CubicSegment(prevNode, controlPoint[0], controlPoint[1], node));
					break;
				case 'S': case 's':
					if (prevNodeType == 'C' || prevNodeType == 'c' || prevNodeType == 'S' || prevNodeType == 's')
						controlPoint[0] = node + node - controlPoint[1];
					else
						controlPoint[0] = node;
					REQUIRE(readCoord(controlPoint[1], pathDef, scale));
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 's') {
						controlPoint[1] += prevNode;
						node += prevNode;
					}
					contour.addEdge(new msdfgen::CubicSegment(prevNode, controlPoint[0], controlPoint[1], node));
					break;
				case 'A': case 'a':
				{
					msdfgen::Vector2 radius;
					double angle;
					bool largeArg;
					bool sweep;
					REQUIRE(readCoord(radius, pathDef, scale));
					REQUIRE(readDouble(angle, pathDef));
					REQUIRE(readBool(largeArg, pathDef));
					REQUIRE(readBool(sweep, pathDef));
					REQUIRE(readCoord(node, pathDef, scale));
					if (nodeType == 'a')
						node += prevNode;
					angle *= M_PI / 180.0;
					addArcApproximate(contour, prevNode, node, radius, angle, largeArg, sweep);
				}
				break;
				default:
					REQUIRE(!"Unknown node type");
				}
				contourStart &= nodeType == 'M' || nodeType == 'm';
				prevNode = node;
				prevNodeType = nodeType;
				readNodeType(nodeType, pathDef);
			}
		NEXT_CONTOUR:
			// Fix contour if it isn't properly closed
			if (!contour.edges.empty() && prevNode != startPoint) {
				if ((contour.edges.back()->point(1) - contour.edges[0]->point(0)).length() < ENDPOINT_SNAP_RANGE_PROPORTION * scale /* size */)
					contour.edges.back()->moveEndPoint(contour.edges[0]->point(0));
				else
					contour.addEdge(new msdfgen::LinearSegment(prevNode, startPoint));
			}
			prevNode = startPoint;
			prevNodeType = '\0';
		}
		return true;
	}

	#undef REQUIRE
	#undef ARC_SEGMENTS_PER_PI
	#undef ENDPOINT_SNAP_RANGE_PROPORTION
	#undef M_PI
}

///////////
// Usage //
///////////

namespace OrbitEngine { namespace Graphics {

	BitmapRGB GenerateFromShape(msdfgen::Shape& shape, SDFMode mode, double range, Math::Vec2i& center, bool square) {
		//if (!shape.validate())
		//	return BitmapRGB();

		double l = 1e240, b = 1e240, r = -1e240, t = -1e240;

		shape.bounds(l, b, r, t);

		if (l > r || b > t)
			return BitmapRGB();

		int width, height;
		msdfgen::Vector2 translate;

		double w = r - l + range;
		double h = t - b + range;

		if (square) {
			width = height = std::round(std::max(w, h));

			center.x = width / 2.0;
			center.y = height / 2.0;

			translate = (center.x, center.y);
		} else {
			width = std::round(w);
			height = std::round(h);

			translate = (-l + range / 2.0, -b + range / 2.0);

			center.x = std::round(translate.x);
			center.y = h - std::round(translate.y);
		}

		shape.normalize();
		edgeColoringSimple(shape, 3.0);

		if (mode == SDFMode::SDF || mode == SDFMode::PSDF) {
			msdfgen::Bitmap<float, 1> buff(width, height);
			if (mode == SDFMode::SDF)
				generateSDF(buff, shape, range, 1, translate);
			else
				generatePseudoSDF(buff, shape, range, 1, translate);
			Bitmap<float, 1> nbuff(buff.operator float* (), width, height); // no copy
			return ConvertBitmap<float, 1, unsigned char, 3>(nbuff);
		}
		else { // mode == SDFMode::MSDF
			msdfgen::Bitmap<float, 3> buff(width, height);
			generateMSDF(buff, shape, range, 1, translate);
			Bitmap<float, 3> nbuff = Bitmap<float, 3>(buff.operator float* (), width, height); // no copy
			return ConvertBitmap<float, 3, unsigned char, 3>(nbuff);
		}
	}

	BitmapRGB GenerateBitmapFromOutline(FT_Outline* outline, SDFMode mode, double range)
	{
		msdfgen::Shape shape;
		outlineToShape(outline, shape);
		Math::Vec2i center;
		return GenerateFromShape(shape, mode, range, center, false);
	}

	BitmapRGB GenerateBitmapFromSVGPath(const std::string& svg_path, SDFMode mode, double range, Math::Vec2i& center, double scale, bool square)
	{
		msdfgen::Shape shape;
		msdfgen::buildFromPath(shape, svg_path.c_str(), scale);
		return GenerateFromShape(shape, mode, range, center, square);
	}

} }