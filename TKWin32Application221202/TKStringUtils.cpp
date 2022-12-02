#include "TKStringUtils.h"

template<typename ... Args>
std::string TKStringUtils::stringWithFormat(const std::string& format, Args ... args)
{
	int formatSize = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;

	if (formatSize <= 0)
		throw std::runtime_error("Format string length is 0.");

	auto size = static_cast<size_t>(formatSize);
	std::unique_ptr<char[]> buffer(new char[size]);

	std::snprintf(buffer.get(), size, format.c_str(), args ...);

	return std::string(buffer.get(), buffer.get() + size - 1);
}