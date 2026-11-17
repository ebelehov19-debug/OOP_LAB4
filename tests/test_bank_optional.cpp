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

TEST(BankOptionalTest, SpecificMethodsAreCalledAfterCast) {
    Deposit deposit(100, 0.1, 1, true);
    Credit credit(100, 0.1, 1, true);
    Card card(100, 0.1);
    Bank_Optional* objects[] = {&deposit, &credit, &card};
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

TEST(BankOptionalTest, RejectsInvalidConstructorArguments) {
    for (double invalid : {-1.0, -100.0}) {
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
    for (int flag : {-1, 2}) {
        EXPECT_THROW(Deposit(100, 0.1, 1, flag), std::invalid_argument);
        EXPECT_THROW(Credit(100, 0.1, 1, flag), std::invalid_argument);
    }
}

TEST(BankOptionalTest, RejectsInvalidBooleanFieldsWithoutChangingObject) {
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

TEST(BankOptionalTest, EqualityComparesMetricAcrossTypesWithTolerance) {
    const Deposit deposit(100, 0.1, 1, false);
    EXPECT_TRUE(deposit == Card(100, 0.1));
    EXPECT_TRUE(deposit == Card(1, 10 + 0.5e-6));
    EXPECT_FALSE(deposit == Card(1, 10 + 2e-6));
    EXPECT_FALSE(deposit == Credit(100, 0.2, 1, false));
}

TEST(BankOptionalTest, DeletingViaBaseCallsDerivedDestructor) {
    class TrackedCard : public Card {
    public:
        explicit TrackedCard(bool& destroyed) : destroyed_(destroyed) {}
        ~TrackedCard() override { destroyed_ = true; }
    private:
        bool& destroyed_;
    };
    bool destroyed = false;
    {
        Bank_Optional* object = new TrackedCard(destroyed);
        EXPECT_FALSE(destroyed);
        delete object;
    }
    EXPECT_TRUE(destroyed);
}
