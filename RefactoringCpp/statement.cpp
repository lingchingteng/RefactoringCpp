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

	if (PlayFor(aPerformance)["type"].get<std::string>() == "tragedy")
	{
		result = 40000;
		if (aPerformance["audience"].get<int>() > 30)
		{
			result += 1000 * (aPerformance["audience"].get<int>() - 30);
		}
	}
	else if (PlayFor(aPerformance)["type"].get<std::string>() == "comedy")
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
		throw std::domain_error("unknown type: " + PlayFor(aPerformance)["type"].get<std::string>());
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

	if (PlayFor(aPerformance)["type"].get<std::string>() == "comedy")
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

std::string StatementCreator::Statement(json invoice)
{
	int totalAmount = 0;
	int volumeCredits = 0;
	std::string result = "Statement for " + invoice["customer"].get<std::string>() + "\n";

	for (auto& perf : invoice["performances"])
	{
		volumeCredits += VolumeCreditsFor(perf);

		result += " " + PlayFor(perf)["name"].get<std::string>() + ": $" + Usd(AmountFor(perf)) + " (" + std::to_string(perf["audience"].get<int>()) + " seats)\n";
		totalAmount += AmountFor(perf);
	}

	result += "Amount owed is $" + Usd(totalAmount) + "\n";

	result += "You earned " + std::to_string(volumeCredits) + " credits\n";

	return result;
}
