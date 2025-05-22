/* re-chord
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Config.h"
#include "Song.h"
#include "Page.h"
#include "Fragment.h"

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Load the configuration files from the default locations, as well as any .conf
// files specified in the command line arguments. If STDIN is being redirected,
// also read configuration from there.
Config InitConfig(char **argv);
// Determine the output file path based on the configuration and the command
// line arguments. If STDOUT is being redirected, return an empty string to
// signify that output should be to STDOUT.
string OutputPath(const Config &config, char **argv);
// Parse all the files that are left in the command line, and return a vector
// that contains their parsed contents.
vector<Song> ParseFiles(char **argv);
// Lay out the files on pages, including possibly pages at the start or end for
// the table of contents.
vector<Page> Layout(const vector<Song> &songs, const string &indexLocation, const string &layout);
// Render the pages, saving them in PDF form to the give path. If the path is
// empty, write the results to STDOUT instead.
void Render(const vector<Page> &pages, const string &layout, const string &path);

// Function to write output to STDOUT instead of to a named file.
Cairo::ErrorStatus Write(const unsigned char *data, unsigned int length);
// Check if the given string ends with the given ending.
bool EndsWith(const string &str, const string &end);



int main(int argc, char *argv[])
{
	// Parse the command line and the configuration files.
	Config config = InitConfig(argv);
	string path = OutputPath(config, argv);
	Page::Init(config);
	
	// Parse any files given in the command line.
	vector<Song> songs = ParseFiles(argv);
	
	// Generate the layout of all the pages, without yet writing them out.
	string indexLocation = config.Text("index-location", "none");
	string layout = config.Text("layout", "single");
	vector<Page> pages = Layout(songs, indexLocation, layout);
	
	// Lay out the pages, add page numbers, and write the file.
	Render(pages, layout, path);
	
	return 0;
}



// Load the configuration files from the default locations, as well as any .conf
// files specified in the command line arguments. If STDIN is being redirected,
// also read configuration from there.
Config InitConfig(char **argv)
{
	// Priority for config values is:
	// 1. Values from STDIN (only if STDIN is not a tty).
	// 2. Values from any "*.conf" file in the command line arguments.
	// 3. Values from "cairo.conf" in the current folder.
	// 4. Values from "~/.cairo.conf".
	// Load the configuration in the opposite order of that, so that the highest
	// priority values are read last and override any previous values.
	Config config;
	config.Load(getenv("HOME") + string("/.cairo.conf"));
	config.Load("cairo.conf");
	
	// Parse the command line arguments. Anything ending in ".conf" should be
	// parsed as a configuration file and removed from the arguments.
	char **out = argv + 1;
	for(char **it = out; *it; ++it)
	{
		string arg = *it;
		if(EndsWith(arg, ".conf"))
			config.Load(arg);
		else
			*out++ = *it;
	}
	// Mark the new end of the argv[] array. There is no need to update argc
	// because I never make use of it.
	*out = nullptr;
	
	// If STDIN is being redirected from a file, read configuration from it.
	if(!isatty(fileno(stdin)))
		config.Load(cin);
	
	return config;
}



// Determine the output file path based on the configuration and the command
// line arguments. If STDOUT is being redirected, return an empty string to
// signify that output should be to STDOUT.
string OutputPath(const Config &config, char **argv)
{
	// Priority for output destinations is:
	// 1. STDOUT, if it is redirected to something other than a tty.
	// 2. The last "*.pdf" file in the command line arguments (if any).
	// 3. The value of "output" in the configuration.
	// 4. "out.pdf" if no output value was given.
	if(!isatty(fileno(stdout)))
		return "";
	string path;
	string textPath;
	int textPathCount = 0;
	
	// Parse the command line arguments. Anything ending in ".pdf" should be
	// removed from the arguments, and the last such value is treated as the
	// output path (unless the output is to STDOUT).
	char **out = argv + 1;
	for(char **it = out; *it; ++it)
	{
		string arg = *it;
		if(EndsWith(arg, ".pdf"))
		{
			if(!path.empty())
				cerr << "Ignoring command line argument \"" << path << "\"." << endl;
			
			path = arg;
		}
		else
		{
			if(EndsWith(arg, ".txt")) {
				textPath = arg;
				++textPathCount;
			}
			*out++ = *it;
		}
	}
	// Mark the new end of the arguments.
	*out = nullptr;
	// If no output file was specified, check if the configuration specifies one
	// and if not, use the default file name:
	string defaultPath = (textPathCount == 1 ? textPath.substr(0, textPath.size() - 4) + ".pdf" : "out.pdf");
	if(path.empty())
		path = config.Text("output", defaultPath);
	
	return path;
}



// Parse all the files that are left in the command line, and return a vector
// that contains their parsed contents.
vector<Song> ParseFiles(char **argv)
{
	vector<Song> songs;
	for(char **it = argv + 1; *it; ++it)
	{
		songs.emplace_back(*it);
		// Make sure a song was actually loaded.
		if(songs.back().empty() || songs.back().Title().empty())
			songs.pop_back();
	}
	return songs;
}



// Lay out the files on pages, including possibly pages at the start or end for
// the table of contents.
vector<Page> Layout(const vector<Song> &songs, const string &indexLocation, const string &layout)
{
	// Check where the index is supposed to be.
	bool hasIndex = (indexLocation != "none");
	
	// Store the index in a separate set of pages, which will be inserted in
	// the proper place once all the songs have been laid out.
	vector<Page> pages;
	vector<Page> index(hasIndex);
	
	for(const Song &song : songs)
	{
		// Each song starts on a new page.
		pages.emplace_back(pages.size() + 1);
		// If we're building an index, add a line for this song.
		if(hasIndex)
		{
			string entry = song.Title() + " (" + song.Subtitle() + ")";
			// Try twice to add a line to the index. If it fails the first time,
			// that means we need to start a new page.
			for(int tries = 0; tries < 2; ++tries)
			{
				if(index.back().AddLine(TextType::INDEX, entry, pages.back().Number()))
					break;
				index.emplace_back();
			}
		}
		// Lay out this song on the page. Assume there's always space for the
		// title and the subtitle, so we don't need to check if this succeeds.
		// Also assume that every song has a title.
		pages.back().AddLine(TextType::TITLE, song.Title());
		if(!song.Subtitle().empty())
			pages.back().AddLine(TextType::SUBTITLE, song.Subtitle());
		pages.back().EndTitle();
		
		// Now, try to lay out each line of the song on the page.
		for(const Line &line : song)
		{
			pages.back().Indent(line.IsIndented());
			for(const Block &block : line)
			{
				// If adding the block doesn't work, start a new page and add it
				// there. Assume it always works the second time around.
				for(int tries = 0; tries < 2; ++tries)
				{
					if(pages.back().Add(line, block))
						break;
					pages.emplace_back(pages.size() + 1);
					// If we're still at the start of the line, indent.
					if(&block == &line.front())
						pages.back().Indent(line.IsIndented());
				}
			}
			pages.back().EndLine(line);
		}
	}
	
	// Insert the index.
	if(indexLocation == "front")
		pages.insert(pages.begin(), index.begin(), index.end());
	else if(indexLocation == "back")
		pages.insert(pages.end(), index.begin(), index.end());
	
	// If there is only one page, don't number it.
	if(pages.size() <= 1)
		return pages;
	
	// Place all the page numbers. If this is a booklet, the numbers will
	// alternate right and left sides. Otherwise they're all centered.
	int side = (layout == "booklet");
	for(Page &page : pages)
	{
		page.PlaceNumber(side);
		side = -side;
	}
	// If the layout is booklet, the number of pages must be a multiple of four.
	while(side && pages.size() & 3)
		pages.emplace_back();
	
	return pages;
}



// Render the pages, saving them in PDF form to the give path. If the path is
// empty, write the results to STDOUT instead.
void Render(const vector<Page> &pages, const string &layout, const string &path)
{
	double width = Page::Width();
	double height = Page::Height();
	int xPages = 1 + (layout == "2up" || layout == "booklet");
	int yPages = 1;
	
	// Create the output context.
	Cairo::RefPtr<Cairo::PdfSurface> surface;
	if(path.empty())
		surface = Cairo::PdfSurface::create_for_stream(&Write, width * xPages, height * yPages);
	else
		surface = Cairo::PdfSurface::create(path, width * xPages, height * yPages);
	Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create(surface);
	
	// Multiple song pages may go on each PDF page.
	int x = 0;
	int y = 0;
	
	// Special case: booklet layout. The page order is N, 1, 2, N - 1, N - 2, 3, 4, ...
	vector<Page> reordered;
	if(layout == "booklet")
	{
		vector<Page>::const_iterator forwardIt = pages.begin();
		vector<Page>::const_iterator backwardIt = pages.end();
		
		while(forwardIt < backwardIt)
		{
			reordered.push_back(*--backwardIt);
			reordered.push_back(*forwardIt++);
			reordered.push_back(*forwardIt++);
			reordered.push_back(*--backwardIt);
		}
	}
	
	// Render each page.
	for(const Page &page : (reordered.empty() ? pages : reordered))
	{
		for(const Fragment &fragment : page)
			fragment.Draw(context, x * width, y * height);
		for(const Leader &leader : page.Leaders())
			leader.Draw(context, x * width, y * height);
		
		// Handle multiple pages being printed on one sheet. Only start a new
		// PDF page if all song pages have been drawn on this one.
		x = (x + 1) % xPages;
		y = (y + 1) % yPages;
		if(!x && !y)
			context->show_page();
	}
}



// Function to write output to STDOUT instead of to a named file.
Cairo::ErrorStatus Write(const unsigned char *data, unsigned int length)
{
	cout.write(reinterpret_cast<const char *>(data), length);
	return CAIRO_STATUS_SUCCESS;
}



// Check if the given string ends with the given ending.
bool EndsWith(const string &str, const string &end)
{
	if(end.length() > str.length())
		return false;
	
	return !str.compare(str.length() - end.length(), end.length(), end);
}
