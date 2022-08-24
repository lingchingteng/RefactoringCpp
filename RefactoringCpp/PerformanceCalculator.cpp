#include "PerformanceCalculator.h"

using json = nlohmann::json;

PerformanceCalculator::PerformanceCalculator(json aPerformance, json play)
{
	mPerformance = aPerformance;
	mPlay = play;
}

int PerformanceCalculator::Amount()
{
	throw std::exception("subclass responsibility");
}

int PerformanceCalculator::VolumeCredits()
{
	int result = 0;

	result += std::max(mPerformance["audience"].get<int>() - 30, 0);

	if (mPlay["type"].get<std::string>() == "comedy")
	{
		result += mPerformance["audience"].get<int>() / 5;
	}

	return result;
}