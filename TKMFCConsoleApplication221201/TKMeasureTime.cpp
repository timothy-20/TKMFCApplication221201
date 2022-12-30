#include "TKMeasureTime.h"

void TKMeasureTime::PrintWithSeconds(std::function<void()> callback)
{
	clock_t start(clock());
	callback();
	std::cout << "실행에 소요한 초: " << ((int)clock() - start) / (CLOCKS_PER_SEC / 1000) << "'s" << std::endl;
}

void TKMeasureTime::PrintWithMicroseconds(std::function<void()> callback)
{
	TKChronoSystemClock::time_point startPoint(TKChronoSystemClock::now());

	callback();

	TKChronoSystemClock::time_point endPoint(TKChronoSystemClock::now());
	double sDuration(TKChronoDuration<double>(endPoint - startPoint).count());
	double mDuration(TKChronoDuration<double, std::milli>(endPoint - startPoint).count());

	std::cout << std::setprecision(7);
	std::cout << std::fixed; // std::cout.unsetf(std::ios::scientific);
	std::cout << "실행에 소요한 초: " << sDuration << "(s)" << std::endl;
	std::cout << std::setprecision(2);
	std::cout << "실행에 소요한 밀리초: " << mDuration << "(ms)" << std::endl;
}