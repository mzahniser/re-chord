/* Page.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef PAGE_H_
#define PAGE_H_

#include "Block.h"
#include "Config.h"
#include "Fragment.h"
#include "Leader.h"
#include "Line.h"
#include "TextType.h"

#include <vector>

using namespace std;



// Represents a single output page, and the text laid out on it.
class Page : public vector<Fragment> {
public:
	// Initialize all the page output settings based on the given configuration.
	static void Init(Config &config);
	// Get the page dimensions.
	static double Width();
	static double Height();
	
	
public:
	// Construct a page, with the given page number.
	explicit Page(size_t number = 0);

	// Set whether the text is indented. Call this at the start of each line.
	void Indent(bool isIndented);
	// Add a line of chords to the page. If there is no 
	// Try to add a block of text to this page. If there is no room for the
	// block, this returns false and you must create a new page for the text.
	// The line needs to be passed in because it's possible that a given block
	// does not have a particular line of text but the line does.
	bool Add(const Line &line, const Block &block, bool force = false);
	// Try to add a line of the given type of text. If two strings are given,
	// the second one is placed right-aligned. This returns false if there is
	// not space for this line on this page.
	bool AddLine(TextType type, const string &left, const string &right = "");
	// End the given line of layout, adding a gap after it.
	void EndLine(const Line &line);
	// End the title block (i.e. add the title gap).
	void EndTitle();

	// Get the page number string.
	const string &Number() const;
	// Set the alignment of the page number: -1 = left, 0 = center, 1 = right.
	void PlaceNumber(int side = 0);
	
	// Get the leader lines, if any.
	const vector<Leader> &Leaders() const;
	
	
private:
	string pageNumber;
	double x;
	double y;
	vector<Leader> leaders;
};



#endif
