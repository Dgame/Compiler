#ifndef DATA_SECTION_HPP
#define DATA_SECTION_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <cassert>

static std::string uniqueLabel();
static std::string escapeString(std::string& in);

class DataSection {
public:
	std::string addStringData(const std::string& data);
	void writeDataSection(std::ostream& out) const;

private:
	std::map<std::string, std::string> string_table;
};

#endif