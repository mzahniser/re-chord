/* Config.h
Copyright (c) 2017 by Michael Zahniser

This program is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <istream>
#include <map>
#include <string>

using namespace std;



// Class representing "key: value" pairs read from a configuration file. The
// values may be text strings or numbers. Each key can have at most a single
// value, so if it appears twice in the file its most recent value is used.
class Config {
public:
	Config() = default;
	explicit Config(const string &path);
	
	// Load from the given path or input stream.
	void Load(const string &path);
	void Load(istream &in);
	
	// Check if the config includes a value for the given key.
	bool Has(const string &key) const;
	// Query the values. The optional argument supplies the default value to
	// return if the key is not found.
	const string &Text(const string &key, const string &defaultValue = "") const;
	double Value(const string &key, double defaultValue = 0.) const;
	double Value(const string &key, const string &defaultValue = "") const;
	
	
private:
	map<string, string> values;
};



#endif
