#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <crtdbg.h>
#include <vector>

#ifdef _DEBUG
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

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

//: public std::enable_shared_from_this<TKElement>
class TKElement 
{
	using TKElementContainer = std::vector<std::shared_ptr<TKElement>>;

private:
	uint16_t m_id;
	TKElementContainer m_subelements;

public:
	TKElement() : m_id(0) { std::cout << "생성 수행. 기본 객체 식별 번호 할당." << std::endl; }
	TKElement(const uint16_t id) : 
		m_id(id), 
		m_subelements(TKElementContainer())
	{
		std::cout << "생성 수행. 현재 객체의 식별 번호 : " << this->m_id << std::endl;
	}
	TKElement(const TKElement& rElement)
	{
		for (auto spElement : rElement.m_subelements)
			this->m_subelements.push_back(std::make_shared<TKElement>(*spElement));

		this->m_id = uint16_t(rElement.m_id) + 1;

		std::cout << "복사 생성 수행" << std::endl;
	}
	
	//복사 할당 생성자 구현할 것.
	TKElement& operator=(const TKElement& rElement)
	{
		if (this != &rElement)
		{
			std::cout << "복사 할당 수행" << std::endl;
		}

		return *this;
	}

	//이동 생성자 및 이동 할당 생성자 구현할 것.

	~TKElement()
	{
		std::cout << "소멸 수행 객체의 식별 번호 : " << this->m_id << std::endl;
	}

	//Utils
	void AddSubelement(std::shared_ptr<TKElement> spElement)
	{
		this->m_subelements.push_back(spElement);
	}
};

//Linked list 구현 중.
class TKNamedElement
{
	using TKWeakNamedElement = std::weak_ptr<TKNamedElement>;

private:
	const uint16_t m_id;
	TKWeakNamedElement m_nextElement;

public:
	TKNamedElement(TKWeakNamedElement nextElemnt=TKWeakNamedElement()) : m_id(0), m_nextElement(nextElemnt)
	{
		std::cout << "기본 생성 수행." << std::endl;
	}	
	virtual ~TKNamedElement()
	{
		std::cout << "소멸 수행." << std::endl;
	}

	//Utils
	
};

int main()
{
	::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	return 0;
}