#pragma once
#include "PerformanceCalculator.h"

class ComedyCalculator : public PerformanceCalculator
{
public:
	ComedyCalculator(nlohmann::json aPerformance, nlohmann::json play);

	int Amount() override;
	int VolumeCredits() override;
};
