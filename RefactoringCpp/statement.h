#pragma once

#include <string>

#include <nlohmann/json.hpp>

std::string Statement(nlohmann::json invoice, nlohmann::json plays);