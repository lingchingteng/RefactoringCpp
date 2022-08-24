#include "TragedyCalculator.h"

using json = nlohmann::json;

TragedyCalculator::TragedyCalculator(json aPerformance, json play)
	: PerformanceCalculator(aPerformance, play)
{

}