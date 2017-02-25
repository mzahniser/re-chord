/* Song.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Song.h"

#include <fstream>

using namespace std;



// Constructor.
Song::Song(const string &path)
{
	Load(path);
}



// Load a song from a file.
void Song::Load(const string &path)
{
	ifstream in(path);
	string line;
	
	// The lines up to the first empty line are the title and subtitle.
	getline(in, title);
	if(!title.empty())
	{
		getline(in, subtitle);
		if(!subtitle.empty())
			getline(in, line);
	}
	
	// The rest of the lines are the text of the song.
	while(getline(in, line))
	{
		// Check if this is a comment.
		size_t pos = 0;
		while(pos != line.length() && line[pos] == ' ')
			++pos;
		if(pos != line.length() && line[pos] == '#')
			continue;
		
		// Parse the line.
		emplace_back(line);
	}
}



// Access the song information.
const string &Song::Title() const
{
	return title;
}



const string &Song::Subtitle() const
{
	return subtitle;
}
