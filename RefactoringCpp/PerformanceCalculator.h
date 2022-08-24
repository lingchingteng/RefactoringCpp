﻿#pragma once

#include "nlohmann/json.hpp"

class PerformanceCalculator
{
public:
	PerformanceCalculator(nlohmann::json aPerformance, nlohmann::json play);

	nlohmann::json mPlay;
	virtual int Amount();
	virtual int VolumeCredits();

protected:
	nlohmann::json mPerformance;
};
