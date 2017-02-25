/* Song.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef SONG_H_
#define SONG_H_

#include "Line.h"

#include <string>
#include <vector>

using namespace std;



// This represents the chords and lyrics to a single song. Each song may occupy
// more than one page, depending on its length.
class Song : public vector<Line> {
public:
	Song() = default;
	explicit Song(const string &path);
	
	// Load a song from a file.
	void Load(const string &path);
	
	// Access the song information.
	const string &Title() const;
	const string &Subtitle() const;
	
	
private:
	string title;
	string subtitle;
};



#endif
