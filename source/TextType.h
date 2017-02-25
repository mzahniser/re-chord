/* Type.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef TEXT_TYPE_H_
#define TEXT_TYPE_H_



// Enumeration for types of text. These are often used as indices into arrays,
// so they can't use a scoped enum ("enum class").
enum TextType : unsigned {
	CHORD = 0,
	TEXT,
	SUBTEXT,
	TITLE,
	SUBTITLE,
	NUMBER,
	INDEX
};



#endif
