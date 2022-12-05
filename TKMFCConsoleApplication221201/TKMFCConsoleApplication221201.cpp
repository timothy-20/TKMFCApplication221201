#include <stdio.h>
#include <iostream>
#include "TKNumberUtil.h"

//namespace TKNumberUtils
//{
//	template<unsigned int T>
//	struct TKFactorial
//	{
//		static const int value = T * TKFactorial<T - 1>::value;
//
//		enum
//		{
//			enumValue = T * TKFactorial<T - 1>::enumValue,
//		};
//	};
//
//	template<>
//	struct TKFactorial<0>
//	{
//		static const int value = 1;
//
//		enum
//		{
//			enumValue = 1,
//		};
//	};
//}

namespace TKStringUtils
{
	constexpr int getLength(const char* text)
	{
		int index = 0;

		while (*(text + index) != '\0')
			index++;

		return index;
	}
}

template<int T>
struct TKBox
{
	void operator() ()
	{
		std::cout << "value: " << T << std::endl;
	}
};




int main()
{
	//TKBox<TKNumberUtils::TKFactorial<3>::enumValue> box1;
	//TKBox<TKNumberUtils::TKFactorial<4>::value> box2;

	//box1();
	//box2();
	//std::cout << "result: " << factorialWithInt1(5) << std::endl;

	//char text1[factorialWithInt1(5)] = "Hello!";
	//char text2[TKNumberUtils::TKFactorial<6>::value] = "Timothy";


	std::cout << TKNumberUtil::factorialWithInt1(4) << std::endl;
	std::cout << TKNumberUtil::factorialWithInt2(4, 1) << std::endl;
	std::cout << TKNumberUtil::factorialWithInt3(4) << std::endl;

	//char text[TKNumberUtils::TKFactorial<3>::value] = "abcde";

	return 0;
}