/* de-chord
Copyright (c) 2017 by Michael Zahniser

Program to parse ChordPro files and convert them to a simpler format closer to
plain text, for input to the re-chord program.

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Trim leading and trailing whitespace and squash any multiple spaces into a
// single space.
string Squash(const string &line);
// Split a string into two parts, one of which comes before a colon and one of
// which comes after. Ignore leading whitespace after the colon.
pair<string, string> Split(const string &line);



int main(int argc, char *argv[])
{
	// Output text:
	string title;
	string subtitle;
	vector<string> output;
	
	bool isChorus = false;
	string line;
	while(getline(cin, line))
	{
		bool leadingWhite = (!line.empty() && line[0] <= ' ');
		
		// First, strip leading and trailing whitespace.
		line = Squash(line);
		if(line.empty())
		{
			if(!output.empty() && !output.back().empty())
				output.push_back(line);
			continue;
		}
		if(line.front() == '#')
			continue;
		if(line.front() == '{' && line.back() == '}')
		{
			pair<string, string> split = Split(line);
			if(split.first == "t" || split.first == "title")
				title = split.second;
			else if(split.first == "st" || split.second == "subtitle")
				subtitle = split.second;
			else if(split.first == "soc" || split.first == "start_of_chorus")
				isChorus = true;
			else if(split.first == "eoc" || split.first == "end_of_chorus")
				isChorus = false;
			else if(split.first == "c" || split.first == "comment"
					|| split.first == "ci" || split.first == "comment_italic"
					|| split.first == "cb" || split.first == "comment_box")
				output.emplace_back((isChorus ? "\t{" : "{") + split.second + "}");
			
			continue;
		}
		if(isChorus || leadingWhite)
			output.emplace_back("\t" + line);
		else
			output.emplace_back(line);
	}
	
	cout << title << endl;
	cout << subtitle << endl << endl;
	for(const string &line : output)
		cout << line << endl;
	
	return 0;
}



// Trim leading and trailing whitespace.
string Squash(const string &line)
{
	string output;
	bool wasSpace = true;
	for(char c : line)
	{
		if(c > ' ' || !wasSpace)
		{
			wasSpace = (c <= ' ');
			output += c;
		}
	}
	while(!output.empty() && output.back() <= ' ')
		output.pop_back();
	
	return output;
}



// Split a string into two parts, one of which comes before a colon and one of
// which comes after. Ignore leading whitespace after the colon.
pair<string, string> Split(const string &line)
{
	pair<string, string> result;
	
	// Find the colon, if there is one.
	size_t split = line.find(':');
	if(split == string::npos)
		split = line.length() - 1;
	
	// Everythign before the colon is the key.
	result.first = line.substr(1, split - 1);
	
	// Ignore any spaces after the colon and before the value.
	++split;
	while(split < line.length() && line[split] <= ' ')
		++split;
	if(split < line.length() - 1)
		result.second = line.substr(split, line.length() - 1 - split);
	
	return result;
}
