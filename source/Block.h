/* Block.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef BLOCK_H_
#define BLOCK_H_

#include "TextType.h"

#include <string>

using namespace std;



// This represents a single "block" of text. Each line of text in the block
// is left-aligned, and the block's width is the width of the longest line.
class Block {
public:
	// Add text of the given type.
	void Add(const string &line, TextType type);
	
	// Check what lines of the block are occupied.
	bool Has(TextType type) const;
	
	// Get one of the lines of the block.
	const string &Get(TextType type) const;
	
	// Check if each line of the block in indented. Return true if the given
	// type of text does not exist in this block.
	bool IsIndented(TextType type) const;
	
	
private:
	string lines[3];
	bool isIndented[3] = {false, false, false};
};



#endif
