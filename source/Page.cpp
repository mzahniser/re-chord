/* Page.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Page.h"

#include "Font.h"

using namespace std;

namespace {
	double width;
	double height;
	
	double leftMargin;
	double rightMargin;
	double topMargin;
	double bottomMargin;
	
	double indent;
	double outdent;
	
	double lineGap;
	double stanzaGap;
	double titleGap;
	
	Font font[7];
}



// Initialize all the page output settings based on the given configuration.
void Page::Init(Config &config)
{
	// Set the page layout values.
	width = config.Value("page-width", "8.5 in");
	height = config.Value("page-height", "11 in");
	
	leftMargin = config.Value("margin-left", "1 in");
	topMargin = config.Value("margin-top", "1 in");
	rightMargin = width - config.Value("margin-right", "1 in");
	bottomMargin = height - config.Value("margin-bottom", "1 in");
	
	indent = config.Value("line-indent", "0.5 in");
	outdent = config.Value("block-indent", "0.2 in");
	
	double textSize = config.Value("text-size", 12);
	lineGap = config.Value("line-gap", textSize * .25);
	stanzaGap = config.Value("stanza-gap", textSize * 1.5);
	titleGap = config.Value("title-gap", stanzaGap);
	
	// Set the font faces.
	string face[7];
	face[TextType::CHORD] = config.Text("chord-font", "Ubuntu:style=Medium");
	face[TextType::TEXT] = config.Text("text-font", "Ubuntu:style=Regular");
	face[TextType::SUBTEXT] = config.Text("subtext-font", "Ubuntu:style=Italic");
	face[TextType::TITLE] = config.Text("title-font", face[TextType::CHORD]);
	face[TextType::SUBTITLE] = config.Text("subtitle-font", face[TextType::SUBTEXT]);
	face[TextType::NUMBER] = config.Text("number-font", face[TextType::SUBTITLE]);
	face[TextType::INDEX] = config.Text("index-font", face[TextType::TEXT]);
	
	// Set the font sizes.
	double size[7];
	size[TextType::CHORD] = config.Value("chord-size", textSize);
	size[TextType::TEXT] = textSize;
	size[TextType::SUBTEXT] = config.Value("subtext-size", textSize);
	size[TextType::TITLE] = config.Value("title-size", 1.2 * textSize);
	size[TextType::SUBTITLE] = config.Value("subtitle-size", .8 * size[TextType::SUBTEXT]);
	size[TextType::NUMBER] = config.Value("number-size", size[TextType::SUBTITLE]);
	size[TextType::INDEX] = config.Value("index-size", textSize);
	
	// Set the baselines and line heights for all the fonts.
	static const string NAME[7] = {
		"chord",
		"text",
		"subtext",
		"title",
		"subtitle",
		"number",
		"index"
	};
	for(int i = 0; i < 7; ++i)
	{
		font[i].SetFace(face[i]);
		font[i].SetSize(size[i]);
		font[i].SetBaseline(config.Value(NAME[i] + "-base", .75 * size[i]));
		font[i].SetLineHeight(config.Value(NAME[i] + "-height", 1.15 * size[i]));
	}
}



// Get the page width.
double Page::Width()
{
	return width;
}



// Get the page height.
double Page::Height()
{
	return height;
}



// Construct a page, with the given page number.
Page::Page(size_t number)
	: x(leftMargin), y(topMargin)
{
	if(number)
		pageNumber = to_string(number);
}



// Set whether the text is indented or not.
void Page::Indent(bool isIndented)
{
	x += indent * isIndented;
}



// Try to add a block of text to this page. If there is no room for the
// block, this returns false and you must create a new page for the text.
bool Page::Add(const Line &line, const Block &block, bool force)
{
	// TODO: This would probably be a whole lot cleaner if Page just added a
	// line at a time instead of a single block, because I wouldn't have to
	// recalculate things and have special code to figure out if it's the start
	// of a line or not.
	
	// Determine how tall and wide this block is.
	double lineHeight = 0.;
	double width = 0.;
	for(size_t i = 0; i < 3; ++i)
	{
		TextType type = static_cast<TextType>(i);
		if(block.Has(type))
			width = max(width, font[type].Width(block.Get(type)) + outdent * block.IsIndented(type));
		if(line.Has(type))
			lineHeight += font[type].LineHeight();
	}
	
	// If this block will not fit on the page, start a new page.
	if(y + lineHeight > bottomMargin)
		return false;
	
	// Check if we're at the beginning of a line. If so, block indents should
	// instead outdent the chords so the text is flush.
	if(&block == &line.front())
	{
		// Outdent the block if it has indented text or subtext.
		if(block.IsIndented(TextType::TEXT) || block.IsIndented(TextType::SUBTEXT))
			x -= outdent;
	}
	
	// Unless this block is being forced to draw in the current position, check
	// whether there is space for it or not. If not, start a new line and force
	// the block to draw there even if there's not enough space.
	if(!force && x + width > rightMargin)
	{
		y += lineHeight;
		x = leftMargin;
		return Add(line, block, true);
	}
	
	// Now, we know there's space to draw this block in this location.
	double textY = y;
	for(size_t i = 0; i < 3; ++i)
	{
		TextType type = static_cast<TextType>(i);
		if(block.Has(type))
		{
			double textX = x + outdent * block.IsIndented(type);
			emplace_back(font[type], block.Get(type), textX, textY);
		}
		if(line.Has(type))
			textY += font[type].LineHeight();
	}
	// Advance the x position.
	x += width;
	
	return true;
}



// Try to add a line of the given type of text. If two strings are given,
// the second one is placed right-aligned. This returns false if there is
// not space for this line on this page.
bool Page::AddLine(TextType type, const string &left, const string &right)
{
	// Check if there's space for this line on this page. If not, return false.
	if(y + font[type].LineHeight() > bottomMargin)
		return false;
	
	// Place the text.
	if(!left.empty())
		emplace_back(font[type], left, x, y);
	if(!right.empty())
	{
		// Position the leader line.
		double leftWidth = font[type].Width(left);
		double rightWidth = font[type].Width(right);
		double fromX = leftMargin + leftWidth + font[type].Baseline();
		double toX = rightMargin - rightWidth - font[type].Baseline();
		double lineY = y + font[type].Baseline();
		leaders.emplace_back(fromX, toX, lineY);
		
		emplace_back(font[type], right, rightMargin - rightWidth, y);
	}
	// Advance to the next line.
	y += font[type].LineHeight();
	
	return true;
}



// End the current line of layout, adding a gap after it.
void Page::EndLine(const Line &line)
{
	// Figure out how much space the text in this line takes up.
	for(size_t i = 0; i < 3; ++i)
	{
		TextType type = static_cast<TextType>(i);
		if(line.Has(type))
			y += font[type].LineHeight();
	}
	// Add the gap, depending on whether this line was empty or not.
	y += (line.empty() ? stanzaGap : lineGap);
	// Reset the x position to the start of the line.
	x = leftMargin;
}



// End the title block (i.e. add the title gap).
void Page::EndTitle()
{
	y += titleGap;
	x = leftMargin;
}



// Get the page number string.
const string &Page::Number() const
{
	return pageNumber;
}



// Set the alignment of the page number: -1 = left, 0 = center, 1 = right.
void Page::PlaceNumber(int side)
{
	if(pageNumber.empty())
		return;
	
	double numberWidth = font[TextType::NUMBER].Width(pageNumber);
	emplace_back(
		font[TextType::NUMBER],
		pageNumber,
		leftMargin + (side + 1) * (rightMargin - leftMargin - numberWidth) * .5,
		bottomMargin);
}

// Get the leader lines, if any.
const vector<Leader> &Page::Leaders() const
{
	return leaders;
}
