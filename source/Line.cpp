/* Line.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Line.h"

using namespace std;

namespace {
	bool NextToken(const string &line, size_t &pos, string &token, TextType &type);
}



Line::Line(const string &line)
{
	Parse(line);
}



// Parse a line of text. The caller is responsible for screening out comment
// lines and not handing them to this function.
void Line::Parse(const string &line)
{
	// Find the first non-whitespace character in this line.
	size_t pos = 0;
	while(pos < line.length() && line[pos] <= ' ')
		++pos;
	// If this is an empty line, bail out.
	if(pos == line.length())
		return;
	// Otherwise, check if it is indented.
	isIndented = (pos != 0);
	
	string token;
	TextType type;
	while(NextToken(line, pos, token, type))
	{
		// First of all, if there are no blocks in the line yet, add one to
		// contain this token. Also start a new block if the token is text or
		// subtext and that line of the current block is already filled in.
		if(empty() || (type != TextType::TEXT && back().Has(TextType::TEXT)))
			emplace_back();
		
		// Add this token to the appropriate line of the current block.
		back().Add(token, type);
	}
	
	// Check what types of text this line contains.
	for(const Block &block : *this)
		for(size_t i = 0; i < 3; ++i)
			has[i] |= block.Has(static_cast<TextType>(i));
}



// Check if this line is indented.
bool Line::IsIndented() const
{
	return isIndented;
}



// Check what types of text this line contains.
bool Line::Has(TextType type) const
{
	return has[type];
}



namespace {
	bool NextToken(const string &line, size_t &pos, string &token, TextType &type)
	{
		if(pos == line.length())
			return false;
		
		// Check if the next character denotes a chord or subtext.
		type = (line[pos] == '[' ? TextType::CHORD : line[pos] == '{' ? TextType::SUBTEXT : TextType::TEXT);
		size_t start = pos + (type != TextType::TEXT);
		// If this is a text block, search for the start of the next thing other
		// than a text block. Otherwise, search for the closing character.
		const char *END[] = {"]", "[{", "}"};
		pos = line.find_first_of(END[type], start);
		if(pos == string::npos)
			pos = line.length();
		// Set the token to the block of text we just found.
		token.assign(line, start, pos - start);
		// If we're at a closing character, move forward one character.
		if(pos != line.length() && type != TextType::TEXT)
			++pos;
		
		return true;
	}
}
