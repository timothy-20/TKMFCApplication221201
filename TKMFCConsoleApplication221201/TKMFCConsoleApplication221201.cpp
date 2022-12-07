#include <stdio.h>
#include <iostream>

inline void IncreaseNumber(int*& pl)
{
	if (pl == nullptr)
	{
		int defalutValue = 0;
		pl = &defalutValue;
	}
	else
	{
		*pl += 10;
	}
}

int main()
{
	int* ptrValue = nullptr;

	::IncreaseNumber(ptrValue);
	std::cout << "result: " << *ptrValue << std::endl;
	::IncreaseNumber(ptrValue);
	std::cout << "result: " << *ptrValue << std::endl;

	return 0;
}