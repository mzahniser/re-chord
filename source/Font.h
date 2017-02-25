/* Font.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef FONT_H_
#define FONT_H_

#include <cairomm/context.h>
#include <cairomm/fontface.h>

#include <string>

using namespace std;



// This class represents a particular weight, style, and size of a particular
// typeface. It can be used to draw text to a cairo context.
class Font {
public:
	Font() = default;
	Font(const string &name, double points = 12.);
	// Don't allow copying.
	Font(const Font &) = delete;
	Font &operator=(const Font &) = delete;
	
	// Set the font face and font size.
	void SetFace(const string &name);
	void SetSize(double points);
	// Set how far below the draw coordinates the "baseline" of the text should
	// be placed.
	void SetBaseline(double points);
	// Set the total height of a line of text drawn with this font.
	void SetLineHeight(double points);
	
	// Get the width of the given text string (in points).
	double Width(const string &text) const;
	// Get the suggested height of a line of text in this font (in points).
	double LineHeight() const;
	// Get the baseline height.
	double Baseline() const;
	
	// Draw the given text at the given location.
	void Draw(const string &text, Cairo::RefPtr<Cairo::Context> &context, double x, double y) const;
	
	
private:
	Cairo::RefPtr<Cairo::FtFontFace> face;
	double size = 12.;
	double baseline = 9.;
	double lineHeight = 14.;
	
	// Each font stores its own private context so it can make measurements
	// without having to change the font binding of the main context.
	Cairo::RefPtr<Cairo::Context> myContext;
};



#endif
