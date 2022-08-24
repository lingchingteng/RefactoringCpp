#include "PerformanceCalculator.h"

using json = nlohmann::json;

PerformanceCalculator::PerformanceCalculator(json aPerformance, json play)
{
	mPerformance = aPerformance;
	mPlay = play;
}