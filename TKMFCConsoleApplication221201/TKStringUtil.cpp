#include "TKStringUtil.h"

int TKStringUtil::getTextLength(const char* text)
{
	int index(0);

	while (*(text + index) != '\0')
		index++;

	return index;
}