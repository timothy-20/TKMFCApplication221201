#pragma once

#include <iostream>
#include <string>
#include <stdexcept>

class TKStringUtils
{
public:
	template<typename ... Args>
	std::string stringWithFormat(const std::string& format, Args ... args);
};

