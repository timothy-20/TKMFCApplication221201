#pragma once
#include <iostream>
#include <functional>
#include <time.h>
#include <chrono>
#include <iomanip>

namespace TKMeasureTime
{
	using TKChronoSystemClock = std::chrono::system_clock;
	template<typename TYPE, typename PERIOD = std::ratio<1>>
	using TKChronoDuration = std::chrono::duration<TYPE, PERIOD>;

	// Utils
	void PrintWithSeconds(std::function<void()> callback);
	void PrintWithMicroseconds(std::function<void()> callback);
}

