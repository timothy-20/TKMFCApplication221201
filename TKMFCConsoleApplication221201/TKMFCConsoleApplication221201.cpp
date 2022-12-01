#include <iostream>
#include "TKWinApp.h"

int main()
{
	TKWinApp* pThread = AFTGetWinApp();

	if (pThread->InitInstance() == FALSE)
	{
		goto Failure;
	}

	pThread->Run();

Failure:
	return 0;
}