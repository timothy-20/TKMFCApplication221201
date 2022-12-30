#pragma once
#include <iostream>
#include <functional>
#include <chrono>
#include <thread>

namespace TKTime
{
	void PrintNowTime();
	void PrintNowSummaryTime();
	std::function<std::chrono::duration<double>& (void)> GetDurationTime();
	template<typename T, typename... Args>
	void Wait(uint32_t seconds, bool isAsync, T&& func, Args... parameters);
}

