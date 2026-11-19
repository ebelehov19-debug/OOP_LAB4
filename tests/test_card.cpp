#include "Bank_Optional.h"
#include "Card.h"
#include "Credit.h"
#include "Deposit.h"

#include <gtest/gtest.h>

#include <iomanip>
#include <limits>
#include <sstream>

#include <string>

constexpr double eps = 1e-6;

TEST(CardTest, DefaultHasZeroMetric) {
    const Card product;
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}

TEST(CardTest, ReadsPlainFieldsThroughBaseReference) {
    Card product;
    Bank_Optional& base = product;
    std::istringstream stream("10000 0.05");
    stream >> base;
    ASSERT_FALSE(stream.fail());
    EXPECT_NEAR(product.metric(), Card(10000, 0.05).metric(), eps);
}

TEST(CardTest, OutputIncludesTypeAndMetric) {
    const auto product = Card(10000, 0.05);
    const Bank_Optional& base = product;
    std::ostringstream stream;
    stream << base;
    EXPECT_NE(stream.str().find("Card:"), std::string::npos);
    EXPECT_NE(stream.str().find("rate="), std::string::npos);
    EXPECT_NE(stream.str().find("metric="), std::string::npos);
}

TEST(CardTest, RejectsIncorrectLabels) {
    Card product;
    std::istringstream stream("Card: wrong=100");
    stream >> product;
    EXPECT_TRUE(stream.fail());
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}
TEST(CardTest, InvalidInputPreservesObject) {
    auto product = Card(10000, 0.05);
    const double previous = product.metric();
    for (const char* text : {"oops", "", "1"}) {
        std::istringstream stream(text);
        stream >> product;
        EXPECT_TRUE(stream.fail()) << text;
        EXPECT_DOUBLE_EQ(product.metric(), previous);
    }
}

TEST(CardTest, DoubleConversionUsesVirtualMetric) {
    const auto product = Card(10000, 0.05);
    const Bank_Optional& base = product;
    EXPECT_DOUBLE_EQ(static_cast<double>(base), product.metric());
}

TEST(CardTest, DynamicCastRecognizesType) {
    auto product = Card(10000, 0.05);
    Bank_Optional* base = &product;
    EXPECT_EQ(dynamic_cast<Card*>(base), &product);
    EXPECT_EQ(dynamic_cast<Deposit*>(base), nullptr);
}

TEST(CardTest, ComputesCashback) {
    EXPECT_NEAR(Card(10000, 0.05).metric(), 500, eps);
    EXPECT_DOUBLE_EQ(Card(0, 0.05).metric(), 0);
    EXPECT_DOUBLE_EQ(Card(10000, 0).metric(), 0);
}


TEST(CardTest, OutputPreservesStreamPrecision) {
    const Card original = Card(12345.678901234, 0.123456789);
    std::stringstream stream;
    const auto precision = stream.precision();
    stream << original;
    EXPECT_EQ(stream.precision(), precision);
}

