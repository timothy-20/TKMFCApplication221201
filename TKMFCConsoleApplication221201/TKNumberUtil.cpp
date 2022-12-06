#include "TKNumberUtil.h"

TKNumberUtil::TKNumberUtil()
{

}

TKNumberUtil::~TKNumberUtil()
{

}

constexpr int TKNumberUtil::factorialWithInt1(unsigned int n)
{
	if (n == 0)
		return 1;

	return n * factorialWithInt1(n - 1);
}

constexpr int TKNumberUtil::factorialWithInt2(unsigned int n, int sum)
{
	if (n == 1)
		return sum;

	return factorialWithInt2(n - 1, n * sum);
}

int TKNumberUtil::factorialWithInt3(unsigned int n)
{
	static int sum(n); // static ������ �ʱ�ȭ�� �ش� ������ �� ���� ����ȴ�.

	if (n == 1)
		return sum;

	sum *= n - 1;

	return factorialWithInt3(n - 1);
}