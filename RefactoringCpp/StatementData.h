#pragma once

#include <nlohmann/json.hpp>

class StatementData
{
public:
	explicit StatementData(nlohmann::json plays);
	nlohmann::json CreateStatementData(nlohmann::json invoice);

private:
	nlohmann::json mPlays;

	int AmountFor(nlohmann::json& aPerformance);
	nlohmann::json PlayFor(nlohmann::json& aPerformance);
	int VolumeCreditsFor(nlohmann::json& aPerformance);
	int TotalVolumeCredits(nlohmann::json data);
	int TotalAmount(nlohmann::json data);
	nlohmann::json EnrichPerformance(nlohmann::json& aPerformance);
};
