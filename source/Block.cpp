/* Block.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Block.h"

using namespace std;



// Add text of the given type.
void Block::Add(const string &line, TextType type)
{
	// If this is the first line in this block of the given type and it starts
	// with whitespace, this line of the block should be indented.
	bool indent = (lines[type].empty() && !line.empty() && line.front() <= ' ' && line.front() > 0);
	if(indent)
		isIndented[type] = true;
	
	// Add the given line, not counting the indent character if any.
	lines[type].append(line, indent, line.length() - indent);
	// Always add a space after chords, so they don't run into each other.
	// The actual text has spaces already.
	if(type == TextType::CHORD)
		lines[type] += ' ';
}

// Check what lines of the block are occupied.
bool Block::Has(TextType type) const
{
	return static_cast<size_t>(type) < 3 && !lines[type].empty();
}



// Get one of the lines of the block.
const string &Block::Get(TextType type) const
{
	static const string EMPTY;
	if(static_cast<size_t>(type) >= 3)
		return EMPTY;
	
	return lines[type];
}



// Check if each line of the block in indented.
bool Block::IsIndented(TextType type) const
{
	return Has(type) && isIndented[type];
}
