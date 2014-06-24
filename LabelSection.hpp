#ifndef LABEL_SECTION_HPP
#define LABELS_SECTION_HPP

#include <string>
#include <vector>
#include <sstream>

struct LabelSection {
	unsigned int Nr = 0;

	std::string* label;
	std::vector<std::string> labels;

	void createLabel() {
		std::ostringstream ss;
		ss << 'L' << Nr++ << std::endl;

		const std::string label = ss.str();

		this->labels.push_back(label);
		this->label = &this->labels.back();
	}
};

#endif