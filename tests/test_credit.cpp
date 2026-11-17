#include "Bank_Optional.h"
#include "Card.h"
#include "Credit.h"
#include "Deposit.h"

#include <gtest/gtest.h>

#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

constexpr double eps = 1e-6;

TEST(CreditTest, DefaultHasZeroMetric) {
    const Credit product;
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}

TEST(CreditTest, ReadsPlainFieldsThroughBaseReference) {
    Credit product;
    Bank_Optional& base = product;
    std::istringstream stream("12000 0 2 1");
    stream >> base;
    ASSERT_FALSE(stream.fail());
    EXPECT_NEAR(product.metric(), Credit(12000, 0, 2, true).metric(), eps);
}

TEST(CreditTest, OutputIncludesTypeAndMetric) {
    const auto product = Credit(12000, 0, 2, true);
    const Bank_Optional& base = product;
    std::ostringstream stream;
    stream << base;
    EXPECT_NE(stream.str().find("Credit:"), std::string::npos);
    EXPECT_NE(stream.str().find("rate="), std::string::npos);
    EXPECT_NE(stream.str().find("metric="), std::string::npos);
}

TEST(CreditTest, RejectsLabelledInput) {
    Credit product;
    std::istringstream stream("Credit: invalid");
    stream >> product;
    EXPECT_TRUE(stream.fail());
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}

TEST(CreditTest, InvalidInputPreservesObject) {
    auto product = Credit(12000, 0, 2, true);
    const double previous = product.metric();
    for (const char* text : {"oops", "", "-1 -1 -1 0", "1"}) {
        std::istringstream stream(text);
        stream >> product;
        EXPECT_TRUE(stream.fail()) << text;
        EXPECT_DOUBLE_EQ(product.metric(), previous);
    }
}

TEST(CreditTest, DoubleConversionUsesVirtualMetric) {
    const auto product = Credit(12000, 0, 2, true);
    const Bank_Optional& base = product;
    EXPECT_DOUBLE_EQ(static_cast<double>(base), product.metric());
}

TEST(CreditTest, DynamicCastRecognizesType) {
    auto product = Credit(12000, 0, 2, true);
    Bank_Optional* base = &product;
    EXPECT_EQ(dynamic_cast<Credit*>(base), &product);
    EXPECT_EQ(dynamic_cast<Card*>(base), nullptr);
}

TEST(CreditTest, ComputesSimpleAndAnnuityAnnualPayments) {
    EXPECT_NEAR(Credit(100000, 0.12, 1, false).metric(), 12000, eps);
    EXPECT_NEAR(Credit(100000, 0.12, 1, true).metric(), 106618.546414, 1e-5);
    EXPECT_DOUBLE_EQ(Credit(12000, 0, 2, true).metric(), 6000);
}

