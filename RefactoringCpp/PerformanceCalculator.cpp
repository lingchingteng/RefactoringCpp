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
	return std::max(mPerformance["audience"].get<int>() - 30, 0);
}