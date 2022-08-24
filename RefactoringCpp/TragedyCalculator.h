#pragma once
#include "PerformanceCalculator.h"

class TragedyCalculator : public PerformanceCalculator
{
public:
	TragedyCalculator(nlohmann::json aPerformance, nlohmann::json play);

	int Amount() override;
};
