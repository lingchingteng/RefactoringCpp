#pragma once

#include <string>

#include <nlohmann/json.hpp>

class StatementCreator
{
public:
	StatementCreator(const nlohmann::json& plays);
	std::string Statement(nlohmann::json invoice);

private:
	nlohmann::json mPlay;
};
