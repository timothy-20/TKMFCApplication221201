#define _CTRDBG_MAP_ALLOC 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);
	
	_CrtDumpMemoryLeaks();
	return 0;
}