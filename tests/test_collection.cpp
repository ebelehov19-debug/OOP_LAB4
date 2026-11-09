#include "BankCollection.h"
#include "Card.h"
#include "Credit.h"
#include "Deposit.h"

#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <utility>

void addProduct(BankCollection& collection, Bank_Optional* object)
{
    std::unique_ptr<Bank_Optional> product(object);
    collection.add(product.get());
    product.release();
}

BankCollection mixed()
{
    BankCollection collection;
    addProduct(collection, new Deposit(10000, 0.1, 2, true)); // 1050
    addProduct(collection, new Credit(12000, 0, 2, true));   // 6000
    addProduct(collection, new Card(10000, 0.05));          // 500
    return collection;
}

class TrackedCard : public Card {
public:
    explicit TrackedCard(int& live, bool fail = false)
        : Card(100, 0.1), live_(live), fail_(fail) { ++live_; }
    ~TrackedCard() override { --live_; }
    Bank_Optional* clone() const override {
        if (fail_) {
            throw std::runtime_error("Copy failure");
        }
        return new TrackedCard(live_, fail_);
    }
private:
    int& live_;
    bool fail_;
};

TEST(CollectionTest, EmptyCollectionHasZeroSizeAndMetric) {
    BankCollection collection;
    EXPECT_EQ(collection.size(), 0u);
    EXPECT_DOUBLE_EQ(collection.totalMetric(), 0);
    collection.sortByMetric();
    EXPECT_THROW(collection.at(0), std::out_of_range);
    EXPECT_THROW(collection.remove(0), std::out_of_range);
}

TEST(CollectionTest, StoresMixedTypesAndSumsMetric) {
    const auto collection = mixed();
    EXPECT_EQ(collection.size(), 3u);
    EXPECT_NEAR(collection.totalMetric(), 7550, 1e-6);
    EXPECT_NE(dynamic_cast<const Deposit*>(&collection.at(0)), nullptr);
    EXPECT_NE(dynamic_cast<const Credit*>(&collection.at(1)), nullptr);
    EXPECT_NE(dynamic_cast<const Card*>(&collection.at(2)), nullptr);
    EXPECT_THROW(collection.at(3), std::out_of_range);
}

TEST(CollectionTest, GrowthPreservesObjectsAndOrder) {
    BankCollection collection;
    for (int i = 0; i < 100; ++i) {
        addProduct(collection, new Card(i, 1));
    }
    EXPECT_EQ(collection.size(), 100u);
    EXPECT_DOUBLE_EQ(collection.totalMetric(), 4950);
    for (std::size_t i = 0; i < collection.size(); ++i) {
        EXPECT_DOUBLE_EQ(collection.at(i).metric(), static_cast<double>(i));
    }
}

TEST(CollectionTest, RejectsNullAndDuplicatePointers) {
    BankCollection collection;
    EXPECT_THROW(collection.add(nullptr), std::invalid_argument);
    addProduct(collection, new Card(100, 0.1));
    EXPECT_THROW(collection.add(&collection.at(0)), std::invalid_argument);
    EXPECT_EQ(collection.size(), 1u);
    EXPECT_DOUBLE_EQ(collection.totalMetric(), 10);
}

TEST(CollectionTest, RemoveDestroysExactlyOneObjectAndShiftsElements) {
    int live = 0;
    {
        BankCollection collection;
        addProduct(collection, new TrackedCard(live));
        addProduct(collection, new TrackedCard(live));
        addProduct(collection, new TrackedCard(live));
        auto* last = &collection.at(2);
        collection.remove(1);
        EXPECT_EQ(live, 2);
        EXPECT_EQ(&collection.at(1), last);
        EXPECT_THROW(collection.remove(2), std::out_of_range);
        collection.remove(0);
        collection.remove(0);
        EXPECT_EQ(live, 0);
        EXPECT_EQ(collection.size(), 0u);
        addProduct(collection, new TrackedCard(live));
    }
    EXPECT_EQ(live, 0);
}

TEST(CollectionTest, SortsAscendingAndPreservesIdentityAndTotal) {
    auto collection = mixed();
    auto* deposit = &collection.at(0);
    auto* credit = &collection.at(1);
    auto* card = &collection.at(2);
    collection.sortByMetric();
    EXPECT_EQ(&collection.at(0), card);
    EXPECT_EQ(&collection.at(1), deposit);
    EXPECT_EQ(&collection.at(2), credit);
    EXPECT_NEAR(collection.totalMetric(), 7550, 1e-6);
    collection.sortByMetric();
    EXPECT_EQ(&collection.at(0), card);
}

TEST(CollectionTest, SortingIsStableForEqualMetrics) {
    BankCollection collection;
    addProduct(collection, new Card(10, 1));
    addProduct(collection, new Deposit(100, 0.1, 1, false));
    addProduct(collection, new Card(0, 0));
    auto* first = &collection.at(0);
    auto* second = &collection.at(1);
    collection.sortByMetric();
    EXPECT_EQ(&collection.at(1), first);
    EXPECT_EQ(&collection.at(2), second);
}

TEST(CollectionTest, SingleElementSortIsValid) {
    BankCollection collection;
    addProduct(collection, new Card(10, 1));
    auto* original = &collection.at(0);
    collection.sortByMetric();
    EXPECT_EQ(&collection.at(0), original);
}

TEST(CollectionTest, CopyConstructorCreatesIndependentObjectsOfSameTypes) {
    auto original = mixed();
    BankCollection copy(original);
    EXPECT_NE(&copy.at(0), &original.at(0));
    EXPECT_NE(dynamic_cast<Deposit*>(&copy.at(0)), nullptr);
    EXPECT_NE(dynamic_cast<Credit*>(&copy.at(1)), nullptr);
    EXPECT_NE(dynamic_cast<Card*>(&copy.at(2)), nullptr);
    EXPECT_DOUBLE_EQ(copy.totalMetric(), original.totalMetric());
    copy.remove(0);
    EXPECT_EQ(original.size(), 3u);
    EXPECT_EQ(copy.size(), 2u);
}

TEST(CollectionTest, CopyAssignmentReplacesOwnedObjectsAndSupportsSelfAssignment) {
    auto original = mixed();
    int live = 0;
    BankCollection copy;
    addProduct(copy, new TrackedCard(live));
    copy = original;
    EXPECT_EQ(live, 0);
    EXPECT_NE(&copy.at(0), &original.at(0));
    auto* first = &copy.at(0);
    auto& alias = copy;
    copy = alias;
    EXPECT_EQ(copy.size(), 3u);
    EXPECT_DOUBLE_EQ(copy.totalMetric(), original.totalMetric());
    // Self-copy may replace objects, but must preserve values and ownership.
    (void)first;
}

TEST(CollectionTest, MoveConstructorTransfersObjectsAndLeavesReusableSource) {
    auto source = mixed();
    auto* first = &source.at(0);
    BankCollection moved(std::move(source));
    EXPECT_EQ(&moved.at(0), first);
    EXPECT_EQ(source.size(), 0u);
    EXPECT_DOUBLE_EQ(source.totalMetric(), 0);
    addProduct(source, new Card(1, 1));
    EXPECT_EQ(source.size(), 1u);
}

TEST(CollectionTest, MoveAssignmentDestroysOldObjectsAndSupportsSelfMove) {
    auto source = mixed();
    auto* first = &source.at(0);
    int live = 0;
    BankCollection destination;
    addProduct(destination, new TrackedCard(live));
    destination = std::move(source);
    EXPECT_EQ(live, 0);
    EXPECT_EQ(source.size(), 0u);
    EXPECT_EQ(&destination.at(0), first);
    auto& alias = destination;
    destination = std::move(alias);
    EXPECT_EQ(destination.size(), 3u);
    EXPECT_EQ(&destination.at(0), first);
}

TEST(CollectionTest, CopiesAndMovesEmptyCollections) {
    BankCollection empty;
    BankCollection copy(empty);
    BankCollection moved(std::move(copy));
    auto populated = mixed();
    populated = empty;
    EXPECT_EQ(populated.size(), 0u);
    populated = std::move(moved);
    EXPECT_DOUBLE_EQ(populated.totalMetric(), 0);
}

TEST(CollectionTest, SwapExchangesOwnershipAndHandlesSelfSwap) {
    auto first = mixed();
    BankCollection second;
    auto* object = &first.at(0);
    first.swap(second);
    EXPECT_EQ(first.size(), 0u);
    EXPECT_EQ(&second.at(0), object);
    second.swap(second);
    EXPECT_EQ(second.size(), 3u);
}

TEST(CollectionTest, FailedCopyConstructorCleansAlreadyClonedObjects) {
    int live = 0;
    BankCollection source;
    addProduct(source, new TrackedCard(live));
    addProduct(source, new TrackedCard(live, true));
    EXPECT_THROW(BankCollection copy(source), std::runtime_error);
    EXPECT_EQ(live, 2);
    EXPECT_EQ(source.size(), 2u);
}

TEST(CollectionTest, FailedCopyAssignmentPreservesDestination) {
    int live = 0;
    BankCollection source;
    addProduct(source, new TrackedCard(live));
    addProduct(source, new TrackedCard(live, true));
    auto destination = mixed();
    auto* first = &destination.at(0);
    EXPECT_THROW(destination = source, std::runtime_error);
    EXPECT_EQ(live, 2);
    EXPECT_EQ(destination.size(), 3u);
    EXPECT_EQ(&destination.at(0), first);
    EXPECT_NEAR(destination.totalMetric(), 7550, 1e-6);
}
