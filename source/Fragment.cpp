/* Fragment.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Fragment.h"

using namespace std;



// Constructor.
Fragment::Fragment(const Font &font, const string &text, double x, double y)
	: font(&font), text(text), x(x), y(y)
{
}



void Fragment::Draw(Cairo::RefPtr<Cairo::Context> &context, double xOff, double yOff) const
{
	if(font)
		font->Draw(text, context, x + xOff, y + yOff);
}

