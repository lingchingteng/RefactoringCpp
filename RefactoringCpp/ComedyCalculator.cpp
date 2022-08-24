#include "ComedyCalculator.h"

using json = nlohmann::json;

ComedyCalculator::ComedyCalculator(json aPerformance, json play)
	: PerformanceCalculator(aPerformance, play)
{
	
}