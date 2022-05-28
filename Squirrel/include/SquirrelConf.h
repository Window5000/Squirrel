#pragma once
#include <sstream>
#include <iostream>

namespace SquirrelConf {
	enum ItemList {
		Example1,
		Example2,

		Last // Do NOT remove this line
	};

	enum Type {
		AutoClicker,
		Multiplier,

		Last // Do NOT remove this line
	};

	void doType(SquirrelConf::Type type, float amount, float* cps, float* clickmtr, float* clicks) {
		if (type == SquirrelConf::Type::AutoClicker) *cps += amount;
		else if (type == SquirrelConf::Type::Multiplier) *clickmtr += amount;
		else std::cout << "Error!";
	}

	std::ostringstream doTypeDescription(SquirrelConf::Type type, float amount, const char* description) {
		std::ostringstream oss;
		if (type == SquirrelConf::Type::AutoClicker) oss << description << " (+" << amount << " CPS)";
		else if (type == SquirrelConf::Type::Multiplier) oss << description << " (+" << amount << " multiplier)";
		else oss << "Error!";
		return oss;
	}
}
