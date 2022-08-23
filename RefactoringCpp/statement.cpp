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
	return std::accumulate(
		data["performances"].begin(),
		data["performances"].end(),
		0,
		[](int total, json& aPerformance) { return total + aPerformance["volumeCredits"].get<int>(); }
	);
}

int StatementCreator::TotalAmount(json data)
{
	return std::accumulate(
		data["performances"].begin(),
		data["performances"].end(),
		0,
		[](int total, json& aPerformance) { return total + aPerformance["amount"].get<int>(); }
	);
}

std::string StatementCreator::RenderPlainText(json data)
{
	std::string result = "Statement for " + data["customer"].get<std::string>() + "\n";

	for (auto& perf : data["performances"])
	{
		result += " " + perf["play"]["name"].get<std::string>() + ": $" + Usd(perf["amount"]) + " (" + std::to_string(perf["audience"].get<int>()) + " seats)\n";
	}

	result += "Amount owed is $" + Usd(data["totalAmount"]) + "\n";

	result += "You earned " + std::to_string(data["totalVolumeCredits"].get<int>()) + " credits\n";

	return result;
}

json StatementCreator::EnrichPerformance(json& aPerformance)
{
	json result = aPerformance;
	result["play"] = PlayFor(result);
	result["amount"] = AmountFor(result);
	result["volumeCredits"] = VolumeCreditsFor(result);

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
	statementData["totalAmount"] = TotalAmount(statementData);
	statementData["totalVolumeCredits"] = TotalVolumeCredits(statementData);
	return RenderPlainText(statementData);
}
