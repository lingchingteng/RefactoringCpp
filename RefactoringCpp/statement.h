#pragma once

#include <string>

#include <nlohmann/json.hpp>

class StatementCreator
{
public:
	StatementCreator(const nlohmann::json& plays);
	std::string Statement(nlohmann::json invoice);

private:
	nlohmann::json mPlay;

	int AmountFor(nlohmann::json& aPerformance);
	nlohmann::json PlayFor(nlohmann::json& aPerformance);
	int VolumeCreditsFor(nlohmann::json& aPerformance);
	std::string Usd(int money);
	int TotalVolumeCredits(nlohmann::json invoice);
};
