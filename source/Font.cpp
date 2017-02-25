/* Font.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Font.h"

#include <fontconfig/fontconfig.h>

using namespace std;

// Helper functions:
namespace {
	// Function for ignoring any writes to the embedded context.
	Cairo::ErrorStatus Ignore(const unsigned char *, unsigned int) { return CAIRO_STATUS_SUCCESS; }
}



// Constructor.
Font::Font(const string &name, double points)
{
	SetFace(name);
	SetSize(points);
}



// Set the font face.
void Font::SetFace(const string &name)
{
	// Get the font face that most closely matches the given string.
	FcPattern *pattern = FcNameParse(reinterpret_cast<const unsigned char *>(name.c_str()));
	face = Cairo::FtFontFace::create(pattern);
	FcPatternDestroy(pattern);
	
	// Allocate a PDF context just for measuring the font extents.
	if(!myContext)
	{
		myContext = Cairo::Context::create(Cairo::PdfSurface::create_for_stream(&Ignore, 0, 0));
		// Bail out if allocation failed for some reason.
		if(!myContext)
			return;
		// Set the font size of the context.
		myContext->set_font_size(size);
	}
	myContext->set_font_face(face);
}



// Set the font size.
void Font::SetSize(double points)
{
	// Do nothing if this is already what the size is set to.
	if(points == size)
		return;
	
	size = points;
	// Update the font size in the local context.
	if(myContext)
		myContext->set_font_size(size);
}



// Set how far below the draw coordinates the "baseline" of the text should
// be placed.
void Font::SetBaseline(double points)
{
	baseline = points;
}



// Set the total height of a line of text drawn with this font.
void Font::SetLineHeight(double points)
{
	lineHeight = points;
}



// Get the width of the given text string (in points).
double Font::Width(const string &text) const
{
	if(!myContext)
		return 0;
	
	Cairo::TextExtents extents;
	myContext->get_text_extents(text, extents);
	return extents.x_advance;
}



// Get the suggested height of a line of text in this font (in points).
double Font::LineHeight() const
{
	return lineHeight;
}



// Get the baseline height.
double Font::Baseline() const
{
	return baseline;
}



// Draw the given text at the given location.
void Font::Draw(const string &text, Cairo::RefPtr<Cairo::Context> &context, double x, double y) const
{
	// If this font face is not selected in the given context, select it. This
	// check is because there might be a performance penalty to setting a font
	// face over and over again.
	if(context->get_font_face() != face)
	{
		context->set_font_face(face);
		context->set_font_size(size);
	}
	
	context->move_to(x, y + baseline);
	context->show_text(text);
}
