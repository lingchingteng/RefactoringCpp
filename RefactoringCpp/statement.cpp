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

std::string Company::HtmlStatement(json invoice)
{
	return RenderHtml(mStatementData.CreateStatementData(invoice));
}

std::string Company::RenderHtml(json data)
{
	std::string result = "<h1>Statement for " + data["customer"].get<std::string>() + "</h1>\n";
	result += "<table>";
	result += " <tr><th>play</th><th>seats</th><th>cost</th></tr>";
	for (auto& perf : data["performances"])
	{
		result += " <tr><td>" + perf["play"]["name"].get<std::string>() + "</td><td>" + std::to_string(perf["audience"].get<int>()) + " seats</td><td>$" + Usd(perf["amount"]) + "</td></tr>\n";
	}
	result += "</table>";
	result += "<p>Amount owed is <em>$" + Usd(data["totalAmount"]) + "</em></p>\n";

	result += "<p>You earned <em>" + std::to_string(data["totalVolumeCredits"].get<int>()) + "</em> credits</p>\n";

	return result;
}
