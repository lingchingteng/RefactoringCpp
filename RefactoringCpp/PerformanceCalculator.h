#pragma once

#include "nlohmann/json.hpp"

class PerformanceCalculator
{
public:
	PerformanceCalculator(nlohmann::json aPerformance, nlohmann::json play);

	nlohmann::json mPlay;

private:
	nlohmann::json mPerformance;
};
