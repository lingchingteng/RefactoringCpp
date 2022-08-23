﻿#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>

#include "statement.h"
#include "StatementData.h"

TEST(StatementTest, ResultStringCorrect)
{
	const std::string filepathPlays = "plays.json";
	const std::string filepathInvoices = "invoices.json";

    ASSERT_TRUE(std::filesystem::exists(filepathPlays));
    ASSERT_TRUE(std::filesystem::exists(filepathInvoices));

	const auto plays = nlohmann::json::parse(std::ifstream(filepathPlays));
    const auto invoices = nlohmann::json::parse(std::ifstream(filepathInvoices));

    Company company(plays);

    ASSERT_EQ
    (
        "Statement for BigCo\n"
        " Hamlet: $650.00 (55 seats)\n"
        " As You Like It: $580.00 (35 seats)\n"
        " Othello: $500.00 (40 seats)\n"
        "Amount owed is $1,730.00\n"
        "You earned 47 credits\n",
        company.Statement(invoices[0])
    );
}

GTEST_API_ int main(int argc, char** argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}