#include "TragedyCalculator.h"

using json = nlohmann::json;

TragedyCalculator::TragedyCalculator(json aPerformance, json play)
	: PerformanceCalculator(aPerformance, play)
{

}

int TragedyCalculator::Amount()
{
	int result = 40000;

	if (mPerformance["audience"].get<int>() > 30)
	{
		result += 1000 * (mPerformance["audience"].get<int>() - 30);
	}

	return result;
}
