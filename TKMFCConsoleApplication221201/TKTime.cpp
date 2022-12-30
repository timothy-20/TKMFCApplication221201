#include "TKTime.h"

void TKTime::PrintNowTime()
{
	auto now(std::chrono::system_clock::now());
	std::time_t nowTime(std::chrono::system_clock::to_time_t(now));
	char nowTimeChar[30];

	::ctime_s(nowTimeChar, sizeof(nowTimeChar), &nowTime);

	std::cout << "Now time: " << nowTimeChar << std::endl;
}

void TKTime::PrintNowSummaryTime()
{
	std::time_t time(std::time(0));
	std::tm tm{};

	::localtime_s(&tm, &time);

	std::cout << "Hour: " << (tm.tm_hour - 12) << "h" << std::endl;
	std::cout << "Minute: " << tm.tm_min << "m" << std::endl;
}

std::function<std::chrono::duration<double>& (void)>  TKTime::GetDurationTime()
{
	auto startTime(std::chrono::system_clock::now());

	return [=]() -> std::chrono::duration<double>&{
		std::chrono::duration<double> duration((std::chrono::system_clock::now() - startTime));
		return duration;
	};
}

template<typename T, typename... Args>
void TKTime::Wait(uint32_t seconds, bool isAsync, T&& func, Args... parameters)
{
	std::function<typename std::result_of<T(Args...)>::type()> task(std::bind(std::forward<T>(func), std::forward<Args>(parameters)...));

	if (isAsync)
	{
		std::thread([seconds, task]() -> void
			{
				std::this_thread::sleep_for(std::chrono::seconds(seconds));
		task();
			}
		).detach();

		return;
	}

	std::this_thread::sleep_for(std::chrono::seconds(seconds));
	task();
}