#pragma once

#include <nlohmann/json.hpp>

#include "PerformanceCalculator.h"

class StatementData
{
public:
	explicit StatementData(nlohmann::json plays);
	nlohmann::json CreateStatementData(nlohmann::json invoice);

private:
	nlohmann::json mPlays;

	PerformanceCalculator CreatePerformanceCalculator(nlohmann::json& aPerformance, nlohmann::json play);

	nlohmann::json PlayFor(nlohmann::json& aPerformance);
	int TotalVolumeCredits(nlohmann::json data);
	int TotalAmount(nlohmann::json data);
	nlohmann::json EnrichPerformance(nlohmann::json& aPerformance);
};
