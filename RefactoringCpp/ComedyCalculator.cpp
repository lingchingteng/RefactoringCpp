#include "ComedyCalculator.h"

using json = nlohmann::json;

ComedyCalculator::ComedyCalculator(json aPerformance, json play)
	: PerformanceCalculator(aPerformance, play)
{
	
}

int ComedyCalculator::Amount()
{
	int result = 30000;

	if (mPerformance["audience"].get<int>() > 20)
	{
		result += 10000 + 500 * (mPerformance["audience"].get<int>() - 20);
	}

	result += 300 * mPerformance["audience"].get<int>();

	return result;
}

int ComedyCalculator::VolumeCredits()
{
	return PerformanceCalculator::VolumeCredits() + mPerformance["audience"].get<int>() / 5;;
}
