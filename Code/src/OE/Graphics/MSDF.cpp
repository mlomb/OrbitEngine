#include "OE/Graphics/MSDF.hpp"

// FreeType
#include <ft2build.h>
#include FT_OUTLINE_H

// msdfgen
#undef INFINITE
#include <msdfgen.h>
#include <core/Shape.h>

// Partially taken from import-font.h

struct FtContext {
	msdfgen::Point2 position;
	msdfgen::Shape* shape;
	msdfgen::Contour* contour;
};

static msdfgen::Point2 ftPoint2(const FT_Vector& vector) {
	return msdfgen::Point2(vector.x / 64., vector.y / 64.);
}

static int ftMoveTo(const FT_Vector* to, void* user) {
	FtContext* context = reinterpret_cast<FtContext*>(user);
	context->contour = &context->shape->addContour();
	context->position = ftPoint2(*to);
	return 0;
}

static int ftLineTo(const FT_Vector* to, void* user) {
	FtContext* context = reinterpret_cast<FtContext*>(user);
	context->contour->addEdge(new msdfgen::LinearSegment(context->position, ftPoint2(*to)));
	context->position = ftPoint2(*to);
	return 0;
}

static int ftConicTo(const FT_Vector* control, const FT_Vector* to, void* user) {
	FtContext* context = reinterpret_cast<FtContext*>(user);
	context->contour->addEdge(new msdfgen::QuadraticSegment(context->position, ftPoint2(*control), ftPoint2(*to)));
	context->position = ftPoint2(*to);
	return 0;
}

static int ftCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user) {
	FtContext* context = reinterpret_cast<FtContext*>(user);
	context->contour->addEdge(new msdfgen::CubicSegment(context->position, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
	context->position = ftPoint2(*to);
	return 0;
}

bool outlineToShape(FT_Outline* outline, msdfgen::Shape& output) {
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

OrbitEngine::Graphics::BitmapRGB OrbitEngine::Graphics::GenerateBitmapFromOutline(FT_Outline* outline, SDFMode mode, double range)
{
	msdfgen::Shape shape;
	outlineToShape(outline, shape);

	if (!shape.validate())
		return BitmapRGB();

	double l = 1e240, b = 1e240, r = -1e240, t = -1e240;

	shape.bounds(l, b, r, t);

	if (l > r || b > t)
		return BitmapRGB();

	double w = r - l + range;
	double h = t - b + range;
	msdfgen::Vector2 translate(-l + range / 2.0, -b + range / 2.0);

	shape.normalize();
	edgeColoringSimple(shape, 3.0);

	int width = std::round(w);
	int height = std::round(h);

	if (mode == SDFMode::SDF || mode == SDFMode::PSDF) {
		msdfgen::Bitmap<float, 1> buff(width, height);
		if (mode == SDFMode::SDF)
			generateSDF(buff, shape, range, 1, translate);
		else
			generatePseudoSDF(buff, shape, range, 1, translate);
		Bitmap<float, 1> nbuff(buff.operator float* (), width, height); // no copy
		nbuff.flipVertically();
		return ConvertBitmap<float, 1, unsigned char, 3>(nbuff);
	}
	else { // mode == SDFMode::MSDF
		msdfgen::Bitmap<float, 3> buff(width, height);
		generateMSDF(buff, shape, range, 1, translate);
		Bitmap<float, 3> nbuff = Bitmap<float, 3>(buff.operator float* (), width, height); // no copy
		nbuff.flipVertically();
		return ConvertBitmap<float, 3, unsigned char, 3>(nbuff);
	}
}
