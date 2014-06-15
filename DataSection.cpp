#include "DataSection.hpp"

std::string uniqueLabel() {
	static unsigned int i = 0;
	std::ostringstream ss;

	ss << 'L' << i++;

	return ss.str();
}

std::string escapeString(const std::string& in) {
	std::string out;

	for (unsigned int i = 0; i < in.length(); ++i) {
		switch (in[i]) {
			case '\n':
				out += "\\n";
			break;
			case '\t':
				out += "\\t";
			break;
			case '\\':
				out += "\\\\";
			break;
			default:
				out += in[i];
			break;
		}
	}

	return out;
}

void DataSection::writeDataSection(std::ostream& out) const {
	out << "\n.data" << std::endl;

	for (auto iter = string_table.begin(); iter != string_table.end(); ++iter) {
		out << iter->first << ":.ascii \"" << escapeString(iter->second) << "\\0\"" << std::endl;
	}
}

std::string DataSection::addStringData(const std::string& data) {
	const std::string label = uniqueLabel();

	assert(string_table.find(label) == string_table.end());
	
	string_table[label] = data;

	return label;
}