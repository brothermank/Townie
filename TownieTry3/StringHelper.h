#pragma once
#include <sstream>
#include <string>

namespace strh {
	template<class T>
	std::string toString(const T &value) {
		std::ostringstream os;
		os << value;
		return os.str();
	}
}