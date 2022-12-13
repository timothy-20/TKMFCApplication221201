#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

class TKException : public std::exception
{
public:
	const char* what() const noexcept override { return "Unable to format va_args string."; }
};

const char* FormatWithString(const char* format, ...)
{
	//const_cast ? or mutable
	int fn(0), n(static_cast<int>(sizeof(format) * 2));
	std::unique_ptr<char[]> formatted{};
	va_list ap{};

	while (true)
	{
		formatted.reset(new char[n]);

		strcpy_s(&formatted[0], n, format);
		va_start(ap, format);

		fn = vsnprintf(&formatted[0], n, format, ap);

		va_end(ap);

		if (fn < 0 || fn >= n)
		{
			n += abs(fn - n + 1);
		}
		else
		{
			break;
			throw ::TKException();
		}
	}

	char result[40]{};
	char* string(formatted.get());
	char c(*string);

	for (int i = 1; c != '\0'; c = *(string + i++))
	{
		result[i - 1] = c;
	}

	//const char[]

	std::cout << result << std::endl;

	return std::string(formatted.get());
}


typedef struct __USER_INFO
{
private:
	uint16_t m_userId;
	bool m_isPrivate;

public:
	const char* userName{""};
	const char* userDescription{"no descript."};

	virtual void ShowSummaryInfo()
	{
		
	}

	__USER_INFO(uint16_t userId, bool isPrivate) : m_userId(userId), m_isPrivate(isPrivate) {}
	virtual ~__USER_INFO() {}

} TKUserInfo;

typedef struct __PREMIUM_USER_INFO final : public TKUserInfo
{
private:

public:

} TKPremiumUserInfo;

int main()
{
	/*std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);*/

	try
	{
		std::string result = ::FormatWithString("result: %d", 10);

		std::cout << result << std::endl;
	}
	catch (const std::exception& reason)
	{
		std::cout << "Exception reason: " << reason.what() << std::endl;
	}
	
	_CrtDumpMemoryLeaks();
	return 0;
}