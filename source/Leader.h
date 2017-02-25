/* Leader.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef LEADER_H_
#define LEADER_H_

#include <cairomm/context.h>

using namespace std;



// Class representing the connecting dots in the table of contents.
class Leader {
public:
	Leader(double fromX, double toX, double y);
	
	void Draw(Cairo::RefPtr<Cairo::Context> &context, double xOff = 0., double yOff = 0.) const;
	
	
private:
	double fromX;
	double toX;
	double y;
};



#endif
