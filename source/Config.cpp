/* Config.cpp
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "Config.h"

#include <fstream>
#include <tuple>

using namespace std;

// Helper functions:
namespace {
	// Parse the given string as a number in the format [-]<digit>*[.<digit>*].
	double ParseValue(const string &text);
	
	// Trim leading and trailing whitespace from the given section of the given
	// string, and return the trimmed result.
	string Trim(const string &line, size_t from, size_t to);
	
	tuple<string, double> UNITS[] = {
		make_tuple("in", 72.),
		make_tuple("cm", 28.35),
		make_tuple("mm", 2.835),
		make_tuple("pc", 12.)
	};
}



// Constructor, specifying the path to the file.
Config::Config(const string &path)
{
	Load(path);
}



// Load from the given path.
void Config::Load(const string &path)
{
	ifstream in(path);
	Load(in);
}



void Config::Load(istream &in)
{
	string line;
	
	while(getline(in, line))
	{
		// First, check if this is a comment. It's only a comment if it begins
		// with '#', to allow that character to be used in other places.
		if(line.empty() || line[0] == '#')
			continue;
		
		// Find the colon that separates the key from the value.
		size_t colonPos = line.find(':');
		if(colonPos == string::npos)
			continue;
		
		values[Trim(line, 0, colonPos)] = Trim(line, colonPos + 1, line.length());
	}
}



// Check if the config includes a value for the given key.
bool Config::Has(const string &key) const
{
	return values.count(key);
}



// Query the value of the given key, as a text string.
const string &Config::Text(const string &key, const string &defaultValue) const
{
	auto it = values.find(key);
	if(it == values.end())
		return defaultValue;
	
	return it->second;
}



// Query the value of the given key, as a number.
double Config::Value(const string &key, double defaultValue) const
{
	auto it = values.find(key);
	if(it == values.end())
		return defaultValue;
	
	return ParseValue(it->second);
}



// Query the value of the given key, as a number. If the key does not exist,
// parse the given default value as a number and return it instead.
double Config::Value(const string &key, const string &defaultValue) const
{
	return ParseValue(Text(key, defaultValue));
}



// Helper functions:
namespace {
	// Parse the given string as a number in the format [-]<digit>*[.<digit>*].
	double ParseValue(const string &text)
	{
		if(text.empty())
			return 0.;
		
		double value = 0.;
		double place = 1.;
		
		// Check if the text starts with a '-' sign.
		auto it = text.begin();
		bool negate = (*it == '-');
		if(negate)
			++it;
		
		// Loop through all the digits of the text, until we encounter a
		// character that can't be part of a number.
		for( ; it != text.end(); ++it)
		{
			if(*it == '.' && place == 1.)
				place = .1;
			else if(*it >= '0' && *it <= '9')
			{
				double digit = *it - '0';
				if(place == 1.)
				{
					// We have not yet seen a decimal point.
					value *= 10.;
					value += digit;
				}
				else
				{
					// This is after the decimal point.
					value += digit * place;
					place *= .1;
				}
			}
			else
				break;
		}
		// Negate the result if the text started with a '-' sign.
		if(negate)
			value = -value;
		
		// Check for unit suffixes.
		if(it != text.end())
		{
			size_t pos = it - text.begin();
			for(const auto &unit : UNITS)
				if(text.find(get<0>(unit)) != string::npos)
				{
					value *= get<1>(unit);
					break;
				}
		}
		
		return value;
	}
	
	
	
	// Trim leading and trailing whitespace from the given section of the given
	// string, and return the trimmed result.
	string Trim(const string &line, size_t from, size_t to)
	{
		if(from >= to || to > line.length())
			return "";
		
		while(from < to && line[from] <= ' ')
			++from;
		
		while(from < to && line[to - 1] <= ' ')
			--to;
		
		return line.substr(from, to - from);
	}
}
