#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <crtdbg.h>
#include <vector>

//#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif // _DEBUG

typedef struct __ONLY_DOUBLE
{
	template<typename T>
	void getDouble(T) = delete;
	void getDouble(double d)
	{
		std::cout << "Get double: " << d << std::endl;
	}

} TKOnlyDouble;

typedef enum _TKNormalEnum : char
{
	TKLetterA = 'a',
	TKLetterB = 'b',
	TKLetterC = 'c'

} TKNormalEnum;

enum class TKUserType : char
{
	TKNormalUser = 'N',
	TKPremiumUser = 'P',
	TKAdmin = 'A'
};

//typedef struct __USER_INFO
//{
//private:
//	TKUserType m_castType{ TKUserType::TKNormalUser };
//
//public:
//	uint16_t id;
//	const char* name;
//	const char* description;
//
//	char castTypeC() const { return static_cast<char>(this->m_castType); }
//
//	__USER_INFO() :
//		id(0),
//		name(""),
//		description("no description")
//	{
//	}
//	__USER_INFO(TKUserType castType) : __USER_INFO()
//	{
//		this->m_castType = castType;
//	}
//
//	static __USER_INFO GetDefaultUser() { return { TKUserType::TKNormalUser }; }
//
//} TKUserInfo;

std::string FormatWithChar(const char* format, ...) noexcept
{
	int fn(0);
	int n(static_cast<int>(strlen(format) * 2));

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
		}
	}

	return std::string(formatted.get());
}

typedef struct __USER_INFO
{
private:
	uint16_t m_userId;
	bool m_isPrivate;

public:
	const char* userName{ "" };
	const char* userDescription{ "no descript." };

	__USER_INFO() = default;
	__USER_INFO(uint16_t userId, bool isPrivate) : m_userId(userId), m_isPrivate(isPrivate) 
	{ 
		std::cout << "Create __USER_INFO: " << this->m_userId << std::endl; 
	}

	virtual ~__USER_INFO() 
	{ 
		std::cout << "Destroy __USER_INFO: " << this->m_userId << std::endl; 
	}

	virtual void ShowSummaryInfo()
	{
		std::cout << ::FormatWithChar("[INFO] %d : This user name is '%s'", this->m_userId, this->userName) << std::endl;
	}

} TKUserInfo;

typedef struct __PREMIUM_USER_INFO final : public TKUserInfo
{
private:

public:
	__PREMIUM_USER_INFO(uint16_t userId, bool isPrivate) : TKUserInfo(userId, isPrivate)
	{

	}

	void ShowSummaryInfo() override
	{
		TKUserInfo::ShowSummaryInfo();

		std::cout << ::FormatWithChar("[INFO]") << std::endl;
	}

} TKPremiumUserInfo;

int main()
{
	_CrtDumpMemoryLeaks();
	return 0;
}