/* Leader.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Leader.h"

using namespace std;



Leader::Leader(double fromX, double toX, double y)
	: fromX(fromX), toX(toX), y(y)
{
}



void Leader::Draw(Cairo::RefPtr<Cairo::Context> &context, double xOff, double yOff) const
{
	vector<double> pattern = {1., 7.};
	context->set_dash(pattern, 0.);
	context->set_line_width(1);
	
	context->move_to(fromX + xOff, y + yOff);
	context->line_to(toX + xOff, y + yOff);
	context->stroke();
}
