#include "statement.h"

#include <sstream>

using json = nlohmann::json;

std::string Usd(int money)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << std::setprecision(2) << money / 100.0;
	return ss.str();
}

Company::Company(json plays)
	: mStatementData(plays)
{
}

std::string Company::Statement(json invoice)
{
	return RenderPlainText(mStatementData.CreateStatementData(invoice));
}

std::string Company::RenderPlainText(json data)
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

