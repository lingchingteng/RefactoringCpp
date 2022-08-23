#include "statement.h"

#include <sstream>

using json = nlohmann::json;

StatementCreator::StatementCreator(const json& plays)
{
	mPlay = plays;
}

int StatementCreator::AmountFor(json& aPerformance)
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

json StatementCreator::PlayFor(json& aPerformance)
{
	return mPlay[aPerformance["playID"]];
}

int StatementCreator::VolumeCreditsFor(json& aPerformance)
{
	int result = 0;

	result += std::max(aPerformance["audience"].get<int>() - 30, 0);

	if (aPerformance["play"]["type"].get<std::string>() == "comedy")
	{
		result += aPerformance["audience"].get<int>() / 5;
	}

	return result;
}

std::string StatementCreator::Usd(int money)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << std::setprecision(2) << money / 100.0;
	return ss.str();
}

int StatementCreator::TotalVolumeCredits(json data)
{
	int volumeCredits = 0;

	for (auto& perf : data["performances"])
	{
		volumeCredits += VolumeCreditsFor(perf);
	}

	return volumeCredits;
}

int StatementCreator::TotalAmount(json data)
{
	int result = 0;

	for (auto& perf : data["performances"])
	{
		result += AmountFor(perf);
	}

	return result;
}

std::string StatementCreator::RenderPlainText(json data)
{
	std::string result = "Statement for " + data["customer"].get<std::string>() + "\n";

	for (auto& perf : data["performances"])
	{
		result += " " + perf["play"]["name"].get<std::string>() + ": $" + Usd(AmountFor(perf)) + " (" + std::to_string(perf["audience"].get<int>()) + " seats)\n";
	}

	result += "Amount owed is $" + Usd(TotalAmount(data)) + "\n";

	result += "You earned " + std::to_string(TotalVolumeCredits(data)) + " credits\n";

	return result;
}

json StatementCreator::EnrichPerformance(json& aPerformance)
{
	json result = aPerformance;
	result["play"] = PlayFor(result);

	return result;
}

std::string StatementCreator::Statement(json invoice)
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
	return RenderPlainText(statementData);
}
