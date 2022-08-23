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

std::string StatementCreator::Statement(json invoice)
{
    int totalAmount = 0;
    int volumeCredits = 0;
    std::string result = "Statement for " + invoice["customer"].get<std::string>() + "\n";
    auto format = [](double money)
    {
        std::stringstream ss;
        ss.imbue(std::locale(""));
        ss << std::fixed << std::setprecision(2) << money;
        return ss.str();
    };

    for (auto& perf : invoice["performances"])
    {
	    volumeCredits += std::max(perf["audience"].get<int>() - 30, 0);

        if (PlayFor(perf)["type"].get<std::string>() == "comedy")
        {
            volumeCredits += perf["audience"].get<int>() / 5;
        }
        
        result += " " + PlayFor(perf)["name"].get<std::string>() + ": $" + format(AmountFor(perf)/100.0) + " (" + std::to_string(perf["audience"].get<int>()) + " seats)\n";
        totalAmount += AmountFor(perf);
    }

    result += "Amount owed is $" + format(totalAmount/100.0) + "\n";

    result += "You earned " + std::to_string(volumeCredits) + " credits\n";

    return result;
}
