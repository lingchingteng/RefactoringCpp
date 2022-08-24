#pragma once

#include "nlohmann/json.hpp"

class PerformanceCalculator
{
public:
	PerformanceCalculator(nlohmann::json aPerformance, nlohmann::json play);

	nlohmann::json mPlay;
	int Amount();

private:
	nlohmann::json mPerformance;
};
