#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <crtdbg.h>

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

typedef struct __USER_INFO
{
private:
	TKUserType m_castType{ TKUserType::TKNormalUser };

public:
	uint16_t id;
	const char* name;
	const char* description;

	char castTypeC() const { return static_cast<char>(this->m_castType); }

	__USER_INFO() : 
		id(0),
		name(""),
		description("no description")
	{
	}
	__USER_INFO(TKUserType castType) : __USER_INFO()
	{
		this->m_castType = castType;
	}

	static __USER_INFO GetDefaultUser() { return { TKUserType::TKNormalUser }; }
	
} TKUserInfo;

int main()
{
	TKUserInfo defaultUser(::TKUserInfo::GetDefaultUser());
	auto castType = defaultUser.castTypeC();
	
	castType = 'Q';

	std::cout << "user type: " << castType << std::endl;

	_CrtDumpMemoryLeaks();
	return 0;
}