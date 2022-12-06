#pragma once
class TKNumberUtil
{
public:
	TKNumberUtil();
	~TKNumberUtil();
	constexpr static int factorialWithInt1(unsigned int n);
	constexpr static int factorialWithInt2(unsigned int n, int sum = 1);
	static int factorialWithInt3(unsigned int n);

	template<unsigned int T>
	struct TKFactorial
	{
		static const int value = T * TKFactorial<T - 1>::value;

		enum
		{
			enumValue = T * TKFactorial<T - 1>::enumValue,
		};
	};

	template<>
	struct TKFactorial<0>
	{
		static const int value = 1;

		enum
		{
			enumValue = 1,
		};
	};
};