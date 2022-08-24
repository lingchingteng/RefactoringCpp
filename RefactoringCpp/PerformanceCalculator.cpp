#include "PerformanceCalculator.h"

using json = nlohmann::json;

PerformanceCalculator::PerformanceCalculator(json aPerformance, json play)
{
	mPerformance = aPerformance;
	mPlay = play;
}

int PerformanceCalculator::Amount()
{
	int result = 0;

	if (mPlay["type"].get<std::string>() == "tragedy")
	{
		result = 40000;
		if (mPerformance["audience"].get<int>() > 30)
		{
			result += 1000 * (mPerformance["audience"].get<int>() - 30);
		}
	}
	else if (mPlay["type"].get<std::string>() == "comedy")
	{
		result = 30000;
		if (mPerformance["audience"].get<int>() > 20)
		{
			result += 10000 + 500 * (mPerformance["audience"].get<int>() - 20);
		}
		result += 300 * mPerformance["audience"].get<int>();
	}
	else
	{
		throw std::domain_error("unknown type: " + mPlay["type"].get<std::string>());
	}

	return result;
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