#pragma once

#include <string>

/* Overloading operator for use in maps. It can be used by higher level managers as a common resource, to map handle with std::string. */
struct istring_less
{
	bool operator() (const std::string& l, const std::string& r) const
	{
		return (_stricmp(l.c_str(), r.c_str()) < 0);
	}
};