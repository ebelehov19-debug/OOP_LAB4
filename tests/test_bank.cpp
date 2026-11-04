#include "Bank_Optional.h"
#include "Card.h"
#include "Credit.h"
#include "Deposit.h"

#include <gtest/gtest.h>

#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace {
constexpr double eps = 1e-6;

template <class T> T sample();
template <> Deposit sample<Deposit>() { return Deposit(10000, 0.1, 2, true); }
template <> Credit sample<Credit>() { return Credit(12000, 0, 2, true); }
template <> Card sample<Card>() { return Card(10000, 0.05); }

template <class T> const char* input();
template <> const char* input<Deposit>() { return "10000 0.1 2 1"; }
template <> const char* input<Credit>() { return "12000 0 2 1"; }
template <> const char* input<Card>() { return "10000 0.05"; }

template <class T> const char* name();
template <> const char* name<Deposit>() { return "Deposit:"; }
template <> const char* name<Credit>() { return "Credit:"; }
template <> const char* name<Card>() { return "Card:"; }

template <class T> class ProductTest : public testing::Test {};
using Products = testing::Types<Deposit, Credit, Card>;
TYPED_TEST_SUITE(ProductTest, Products);

TYPED_TEST(ProductTest, DefaultHasZeroMetric) {
    const TypeParam product;
    EXPECT_DOUBLE_EQ(product.metric(), 0);
}

TYPED_TEST(ProductTest, ReadsPlainFieldsThroughBaseReference) {
    TypeParam product;
    Bank_Optional& base = product;
    std::istringstream stream(input<TypeParam>());
    stream >> base;
    ASSERT_FALSE(stream.fail());
    EXPECT_NEAR(product.metric(), sample<TypeParam>().metric(), eps);
}

TYPED_TEST(ProductTest, OutputIncludesTypeAndMetric) {
    const auto product = sample<TypeParam>();
    const Bank_Optional& base = product;
    std::ostringstream stream;
    stream << base;
    EXPECT_NE(stream.str().find(name<TypeParam>()), std::string::npos);
    EXPECT_NE(stream.str().find("rate="), std::string::npos);
    EXPECT_NE(stream.str().find("metric="), std::string::npos);
}

TYPED_TEST(ProductTest, PrintedObjectCanBeReadBack) {
    const auto original = sample<TypeParam>();
    std::stringstream stream;
    stream << std::setprecision(std::numeric_limits<double>::max_digits10) << original;
    TypeParam restored;
    stream >> restored;
    ASSERT_FALSE(stream.fail());
    EXPECT_NEAR(restored.metric(), original.metric(), eps);
    EXPECT_EQ(restored, original);
}

TYPED_TEST(ProductTest, InvalidInputPreservesObject) {
    auto product = sample<TypeParam>();
    const double previous = product.metric();
    for (const char* text : {"oops", "", "-1 -1 -1 0", "1"}) {
        std::istringstream stream(text);
        stream >> product;
        EXPECT_TRUE(stream.fail()) << text;
        EXPECT_DOUBLE_EQ(product.metric(), previous);
    }
}

TYPED_TEST(ProductTest, DoubleConversionUsesVirtualMetric) {
    const auto product = sample<TypeParam>();
    const Bank_Optional& base = product;
    EXPECT_DOUBLE_EQ(static_cast<double>(base), product.metric());
}

TYPED_TEST(ProductTest, DynamicCastRecognizesType) {
    auto product = sample<TypeParam>();
    Bank_Optional* base = &product;
    EXPECT_EQ(dynamic_cast<TypeParam*>(base), &product);
    if constexpr (std::is_same_v<TypeParam, Card>) {
        EXPECT_EQ(dynamic_cast<Deposit*>(base), nullptr);
    } else {
        EXPECT_EQ(dynamic_cast<Card*>(base), nullptr);
    }
}

TEST(DepositTest, ComputesSimpleAndCompoundAnnualEffect) {
    EXPECT_NEAR(Deposit(10000, 0.1, 2, false).metric(), 1000, eps);
    EXPECT_NEAR(Deposit(10000, 0.1, 2, true).metric(), 1050, eps);
    EXPECT_DOUBLE_EQ(Deposit(10000, 0, 2, true).metric(), 0);
}

TEST(CreditTest, ComputesSimpleAndAnnuityAnnualPayments) {
    EXPECT_NEAR(Credit(100000, 0.12, 1, false).metric(), 12000, eps);
    EXPECT_NEAR(Credit(100000, 0.12, 1, true).metric(), 106618.546414, 1e-5);
    EXPECT_DOUBLE_EQ(Credit(12000, 0, 2, true).metric(), 6000);
}

TEST(CardTest, ComputesCashback) {
    EXPECT_NEAR(Card(10000, 0.05).metric(), 500, eps);
    EXPECT_DOUBLE_EQ(Card(0, 0.05).metric(), 0);
    EXPECT_DOUBLE_EQ(Card(10000, 0).metric(), 0);
}

TEST(ProductsTest, SpecificMethodsAreCalledAfterCast) {
    auto deposit = std::make_unique<Deposit>(100, 0.1, 1, true);
    auto credit = std::make_unique<Credit>(100, 0.1, 1, true);
    auto card = std::make_unique<Card>(100, 0.1);
    Bank_Optional* objects[] = {deposit.get(), credit.get(), card.get()};
    auto* d = dynamic_cast<Deposit*>(objects[0]);
    auto* c = dynamic_cast<Credit*>(objects[1]);
    auto* k = dynamic_cast<Card*>(objects[2]);
    ASSERT_NE(d, nullptr);
    ASSERT_NE(c, nullptr);
    ASSERT_NE(k, nullptr);
    EXPECT_TRUE(d->isCumulative());
    EXPECT_TRUE(c->isAnnuity());
    EXPECT_TRUE(k->hasCashback());
    EXPECT_FALSE(Deposit().isCumulative());
    EXPECT_FALSE(Credit().isAnnuity());
    EXPECT_FALSE(Card().hasCashback());
}

TEST(ProductsTest, RejectsInvalidConstructorArguments) {
    for (double invalid : {-1.0, std::numeric_limits<double>::infinity(),
                           std::numeric_limits<double>::quiet_NaN()}) {
        EXPECT_THROW(Deposit(invalid, 0.1, 1, false), std::invalid_argument);
        EXPECT_THROW(Deposit(100, invalid, 1, false), std::invalid_argument);
        EXPECT_THROW(Credit(invalid, 0.1, 1, false), std::invalid_argument);
        EXPECT_THROW(Credit(100, invalid, 1, false), std::invalid_argument);
        EXPECT_THROW(Card(invalid, 0.1), std::invalid_argument);
        EXPECT_THROW(Card(100, invalid), std::invalid_argument);
    }
    for (int years : {0, -1}) {
        EXPECT_THROW(Deposit(100, 0.1, years, false), std::invalid_argument);
        EXPECT_THROW(Credit(100, 0.1, years, false), std::invalid_argument);
    }
}

TEST(ProductsTest, RejectsInvalidBooleanFieldsWithoutChangingObject) {
    Deposit deposit(100, 0.1, 1, false);
    Credit credit(100, 0.1, 1, false);
    for (const char* text : {"100 0.1 1 2", "100 0.1 1 -1"}) {
        std::istringstream d(text), c(text);
        d >> deposit;
        c >> credit;
        EXPECT_TRUE(d.fail());
        EXPECT_TRUE(c.fail());
        EXPECT_FALSE(deposit.isCumulative());
        EXPECT_FALSE(credit.isAnnuity());
    }
}

TEST(ProductsTest, EqualityComparesMetricAcrossTypesWithTolerance) {
    const Deposit deposit(100, 0.1, 1, false);
    EXPECT_TRUE(deposit == Card(100, 0.1));
    EXPECT_TRUE(deposit == Card(1, 10 + 0.5e-6));
    EXPECT_FALSE(deposit == Card(1, 10 + 2e-6));
    EXPECT_FALSE(deposit == Credit(100, 0.2, 1, false));
}

TEST(ProductsTest, DeletingViaBaseCallsDerivedDestructor) {
    class TrackedCard : public Card {
    public:
        explicit TrackedCard(bool& destroyed) : destroyed_(destroyed) {}
        ~TrackedCard() override { destroyed_ = true; }
    private:
        bool& destroyed_;
    };
    bool destroyed = false;
    {
        std::unique_ptr<Bank_Optional> object = std::make_unique<TrackedCard>(destroyed);
        EXPECT_FALSE(destroyed);
    }
    EXPECT_TRUE(destroyed);
}
} // namespace
