/* Fragment.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef FRAGMENT_H_
#define FRAGMENT_H_

#include "Font.h"

#include <cairomm/context.h>

#include <string>

using namespace std;



// Class representing a single text fragment and its position on the page.
class Fragment {
public:
	Fragment(const Font &font, const string &text, double x, double y);
	
	void Draw(Cairo::RefPtr<Cairo::Context> &context, double xOff = 0., double yOff = 0.) const;
	
	
private:
	const Font *font = nullptr;
	string text;
	double x;
	double y;
	
	friend class Page;
};



#endif
