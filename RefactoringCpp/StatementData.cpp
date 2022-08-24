#include "StatementData.h"

#include <sstream>

#include "PerformanceCalculator.h"

using json = nlohmann::json;

StatementData::StatementData(json plays)
{
	mPlays = plays;
}

json StatementData::CreateStatementData(json invoice)
{
	json statementData;
	statementData["customer"] = invoice["customer"];
	std::transform(
		invoice["performances"].begin(),
		invoice["performances"].end(),
		std::back_inserter(statementData["performances"]),
		[this](json& aPerformance)
		{
			return this->EnrichPerformance(aPerformance);
		});
	statementData["totalAmount"] = TotalAmount(statementData);
	statementData["totalVolumeCredits"] = TotalVolumeCredits(statementData);

	return statementData;
}

json StatementData::PlayFor(json& aPerformance)
{
	return mPlays[aPerformance["playID"]];
}

int StatementData::VolumeCreditsFor(json& aPerformance)
{
	PerformanceCalculator performanceCalculator(aPerformance, PlayFor(aPerformance));
	return performanceCalculator.VolumeCredits();
}

int StatementData::TotalVolumeCredits(json data)
{
	return std::accumulate(
		data["performances"].begin(),
		data["performances"].end(),
		0,
		[](int total, json& aPerformance) { return total + aPerformance["volumeCredits"].get<int>(); }
	);
}

int StatementData::TotalAmount(json data)
{
	return std::accumulate(
		data["performances"].begin(),
		data["performances"].end(),
		0,
		[](int total, json& aPerformance) { return total + aPerformance["amount"].get<int>(); }
	);
}

json StatementData::EnrichPerformance(json& aPerformance)
{
	PerformanceCalculator performanceCalculator(aPerformance, PlayFor(aPerformance));
	json result = aPerformance;
	result["play"] = performanceCalculator.mPlay;
	result["amount"] = performanceCalculator.Amount();
	result["volumeCredits"] = VolumeCreditsFor(result);

	return result;
}
