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

TEST(DepositTest, DefaultHasZeroMetric) {
    const Deposit product;
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}

TEST(DepositTest, ReadsPlainFieldsThroughBaseReference) {
    Deposit product;
    Bank_Optional& base = product;
    std::istringstream stream("10000 0.1 2 1");
    stream >> base;
    ASSERT_FALSE(stream.fail());
    EXPECT_NEAR(product.metric(), Deposit(10000, 0.1, 2, true).metric(), eps);
}

TEST(DepositTest, OutputIncludesTypeAndMetric) {
    const auto product = Deposit(10000, 0.1, 2, true);
    const Bank_Optional& base = product;
    std::ostringstream stream;
    stream << base;
    EXPECT_NE(stream.str().find("Deposit:"), std::string::npos);
    EXPECT_NE(stream.str().find("rate="), std::string::npos);
    EXPECT_NE(stream.str().find("metric="), std::string::npos);
}

TEST(DepositTest, RejectsLabelledInput) {
    Deposit product;
    std::istringstream stream("Deposit: invalid");
    stream >> product;
    EXPECT_TRUE(stream.fail());
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}

TEST(DepositTest, InvalidInputPreservesObject) {
    auto product = Deposit(10000, 0.1, 2, true);
    const double previous = product.metric();
    for (const char* text : {"oops", "", "-1 -1 -1 0", "1"}) {
        std::istringstream stream(text);
        stream >> product;
        EXPECT_TRUE(stream.fail()) << text;
        EXPECT_DOUBLE_EQ(product.metric(), previous);
    }
}

TEST(DepositTest, DoubleConversionUsesVirtualMetric) {
    const auto product = Deposit(10000, 0.1, 2, true);
    const Bank_Optional& base = product;
    EXPECT_DOUBLE_EQ(static_cast<double>(base), product.metric());
}

TEST(DepositTest, DynamicCastRecognizesType) {
    auto product = Deposit(10000, 0.1, 2, true);
    Bank_Optional* base = &product;
    EXPECT_EQ(dynamic_cast<Deposit*>(base), &product);
    EXPECT_EQ(dynamic_cast<Card*>(base), nullptr);
}

TEST(DepositTest, ComputesSimpleAndCompoundAnnualEffect) {
    EXPECT_NEAR(Deposit(10000, 0.1, 2, false).metric(), 1000, eps);
    EXPECT_NEAR(Deposit(10000, 0.1, 2, true).metric(), 1050, eps);
    EXPECT_DOUBLE_EQ(Deposit(10000, 0, 2, true).metric(), 0);
}

