#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>

#include "statement.h"
#include "StatementData.h"

TEST(StatementTest, StatementRenderPlainTextStatement)
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

TEST(StatementTest, HtmlStatementRenderHtmlStatement)
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
        "<h1>Statement for BigCo</h1>\n"
        "<table>"
        " <tr><th>play</th><th>seats</th><th>cost</th></tr>"
        " <tr><td>Hamlet</td><td>55 seats</td><td>$650.00</td></tr>\n"
        " <tr><td>As You Like It</td><td>35 seats</td><td>$580.00</td></tr>\n"
        " <tr><td>Othello</td><td>40 seats</td><td>$500.00</td></tr>\n"
        "</table>"
        "<p>Amount owed is <em>$1,730.00</em></p>\n"
        "<p>You earned <em>47</em> credits</p>\n",
        company.HtmlStatement(invoices[0])
    );
}

GTEST_API_ int main(int argc, char** argv) {
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}