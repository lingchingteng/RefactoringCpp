#include "StatementData.h"

#include <sstream>

#include "ComedyCalculator.h"
#include "PerformanceCalculator.h"
#include "TragedyCalculator.h"

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

std::shared_ptr<PerformanceCalculator> StatementData::CreatePerformanceCalculator(json& aPerformance, json play)
{
	if (play["type"].get<std::string>() == "tragedy")
	{
		return std::make_shared<TragedyCalculator>(aPerformance, play);
	}
	else if (play["type"].get<std::string>() == "comedy")
	{
		return std::make_shared<ComedyCalculator>(aPerformance, play);
	}
	else
	{
		throw std::domain_error("unknown type: " + play["type"].get<std::string>());
	}
}

json StatementData::EnrichPerformance(json& aPerformance)
{
	std::shared_ptr<PerformanceCalculator> performanceCalculator = CreatePerformanceCalculator(aPerformance, PlayFor(aPerformance));
	json result = aPerformance;
	result["play"] = performanceCalculator->mPlay;
	result["amount"] = performanceCalculator->Amount();
	result["volumeCredits"] = performanceCalculator->VolumeCredits();

	return result;
}
