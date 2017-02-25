/* Line.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef LINE_H_
#define LINE_H_

#include "Block.h"
#include "TextType.h"

#include <vector>

using namespace std;



// This represents a line of text, comprised of multiple blocks.
class Line : public vector<Block> {
public:
	Line() = default;
	explicit Line(const string &line);
	
	// Parse a line of text. The caller is responsible for screening out comment
	// lines and not handing them to this function.
	void Parse(const string &line);
	
	// Check if this line is indented.
	bool IsIndented() const;
	// Check what types of text this line contains.
	bool Has(TextType type) const;
	
	
private:
	bool has[3] = {false, false, false};
	bool isIndented = false;
};



#endif
