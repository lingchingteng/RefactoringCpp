#include "StatementData.h"

#include <sstream>

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

int StatementData::AmountFor(json& aPerformance)
{
	int result = 0;

	if (aPerformance["play"]["type"].get<std::string>() == "tragedy")
	{
		result = 40000;
		if (aPerformance["audience"].get<int>() > 30)
		{
			result += 1000 * (aPerformance["audience"].get<int>() - 30);
		}
	}
	else if (aPerformance["play"]["type"].get<std::string>() == "comedy")
	{
		result = 30000;
		if (aPerformance["audience"].get<int>() > 20)
		{
			result += 10000 + 500 * (aPerformance["audience"].get<int>() - 20);
		}
		result += 300 * aPerformance["audience"].get<int>();
	}
	else
	{
		throw std::domain_error("unknown type: " + aPerformance["play"]["type"].get<std::string>());
	}

	return result;
}

json StatementData::PlayFor(json& aPerformance)
{
	return mPlays[aPerformance["playID"]];
}

int StatementData::VolumeCreditsFor(json& aPerformance)
{
	int result = 0;

	result += std::max(aPerformance["audience"].get<int>() - 30, 0);

	if (aPerformance["play"]["type"].get<std::string>() == "comedy")
	{
		result += aPerformance["audience"].get<int>() / 5;
	}

	return result;
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
	json result = aPerformance;
	result["play"] = PlayFor(result);
	result["amount"] = AmountFor(result);
	result["volumeCredits"] = VolumeCreditsFor(result);

	return result;
}
