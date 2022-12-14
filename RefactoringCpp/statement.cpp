#include "statement.h"

#include <sstream>

using json = nlohmann::json;

std::string Statement(json invoice, json plays)
{
    double totalAmount = 0;
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
        auto& play = plays[perf["playID"]];
        double thisAmount = 0;

        if (play["type"].get<std::string>() == "tragedy")
        {
            thisAmount = 40000;
            if (perf["audience"].get<int>() > 30)
            {
                thisAmount += 1000 * (perf["audience"].get<int>() - 30);
            }
        }
        else if (play["type"].get<std::string>() == "comedy")
        {
            thisAmount = 30000;
            if (perf["audience"].get<int>() > 20)
            {
                thisAmount += 10000 + 500 * (perf["audience"].get<int>() - 20);
            }
            thisAmount += 300 * perf["audience"].get<int>();
        }
        else
        {
            throw std::domain_error("unknown type: " + play["type"].get<std::string>());
        }

        volumeCredits += std::max(perf["audience"].get<int>() - 30, 0);

        if (play["type"].get<std::string>() == "comedy")
        {
            volumeCredits += perf["audience"].get<int>() / 5;
        }
        
        result += " " + play["name"].get<std::string>() + ": $" + format(thisAmount/100) + " (" + std::to_string(perf["audience"].get<int>()) + " seats)\n";
        totalAmount += thisAmount;
    }

    result += "Amount owed is $" + format(totalAmount/100) + "\n";

    result += "You earned " + std::to_string(volumeCredits) + " credits\n";

    return result;
}