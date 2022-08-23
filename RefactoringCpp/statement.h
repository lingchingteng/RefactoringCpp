#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include "StatementData.h"

class Company
{
public:
	explicit Company(nlohmann::json plays);

	std::string Statement(nlohmann::json invoice);
	std::string HtmlStatement(nlohmann::json invoice);

private:
	StatementData mStatementData;

	std::string RenderPlainText(nlohmann::json data);
	std::string RenderHtml(nlohmann::json data);
};
