#pragma once
class TKNumberUtil
{
private:


public:
	TKNumberUtil();
	~TKNumberUtil();
	constexpr static int factorialWithInt1(unsigned int n);
	constexpr static int factorialWithInt2(unsigned int n, int sum = 1);
	static int factorialWithInt3(unsigned int n);
};