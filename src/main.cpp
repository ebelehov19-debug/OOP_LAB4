#include "BankCollection.h"

#include "Card.h"
#include "Credit.h"
#include "Deposit.h"

#include <clocale>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void printCollection(const BankCollection& collection)
{
    for (std::size_t i = 0; i < collection.size(); ++i) {
        const Bank_Optional& product = collection.at(i);
        std::cout << i << ": " << product << '\n';

        if (const Deposit* deposit = dynamic_cast<const Deposit*>(&product)) {
            std::cout << "  isCumulative (0 - false, 1 - true): " << deposit->isCumulative() << '\n';
        }
        else if (const Credit* credit = dynamic_cast<const Credit*>(&product)) {
            std::cout << "  isAnnuity (0 - false, 1 - true): " << credit->isAnnuity() << '\n';
        }
        else if (const Card* card = dynamic_cast<const Card*>(&product)) {
            std::cout << "  hasCashback (0 - false, 1 - true): " << card->hasCashback() << '\n';
        }
    }
    std::cout << "Суммарная метрика: " << std::fixed << std::setprecision(2)
              << collection.totalMetric() << std::defaultfloat << std::setprecision(6) << '\n';
}

void showSpecificMethod(const Bank_Optional& product)
{
    if (const Deposit* deposit = dynamic_cast<const Deposit*>(&product)) {
        std::cout << "isCumulative (0 - false, 1 - true): " << deposit->isCumulative() << '\n';
    }
    else if (const Credit* credit = dynamic_cast<const Credit*>(&product)) {
        std::cout << "isAnnuity (0 - false, 1 - true): " << credit->isAnnuity() << '\n';
    }
    else if (const Card* card = dynamic_cast<const Card*>(&product)) {
        std::cout << "hasCashback (0 - false, 1 - true): " << card->hasCashback() << '\n';
    }
}

bool readNumber(const char* prompt, int& number)
{
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        return false;
    }

    std::istringstream input(line);
    if (!(input >> number) || !(input >> std::ws).eof()) {
        std::cout << "Введите одно целое число.\n";
        return false;
    }
    return true;
}

void createProduct(BankCollection& collection)
{
    int type;
    if (!readNumber("Что создать: 1 — вклад, 2 — кредит, 3 — карта: ", type)) {
        return;
    }

    Bank_Optional* product = nullptr;
    if (type == 1) 
    {
        std::cout << "Введите сумму, ставку, срок в годах и капитализацию (0/1): ";
        product = new Deposit;
    }
    else if (type == 2) 
    {
        std::cout << "Введите сумму, ставку, срок в годах и аннуитет (0/1): ";
        product = new Credit;
    }
    else if (type == 3) 
    {
        std::cout << "Введите расходы и ставку кешбэка: ";
        product = new Card;
    }
    else 
    {
        std::cout << "Неизвестный тип продукта.\n";
        return;
    }

    std::string line;
    if (!std::getline(std::cin, line)) {
        delete product;
        return;
    }
    std::istringstream input(line);
    if (!(input >> *product) || !(input >> std::ws).eof()) {
        delete product;
        std::cout << "Введите все поля продукта числами в одной строке.\n";
        return;
    }

    collection.add(product);
    std::cout << "Продукт добавлен. Индекс: " << collection.size() - 1 << '\n';
}

bool readIndex(const BankCollection& collection, std::size_t& index)
{
    if (collection.size() == 0) {
        std::cout << "Коллекция пуста.\n";
        return false;
    }

    int value;
    if (!readNumber("Введите индекс продукта: ", value)) {
        return false;
    }
    if (value < 0 || static_cast<std::size_t>(value) >= collection.size()) {
        std::cout << "Нет продукта с таким индексом.\n";
        return false;
    }
    index = static_cast<std::size_t>(value);
    return true;
}

int main()
{
    std::setlocale(LC_ALL, "");
    BankCollection collection;
    std::cout << "Банковские продукты. Ставки вводятся долями: 0.1 означает 10%.\n";
    while (std::cin.good()) {
        std::cout << "\n1 — Создать продукт\n"
                  << "2 — Показать коллекцию\n"
                  << "3 — Показать суммарную метрику\n"
                  << "4 — Сортировать по метрике\n"
                  << "5 — Удалить продукт\n"
                  << "6 — Вызвать специфичный метод\n"
                  << "7 — Показать метрику через преобразование в double\n"
                  << "8 — Сравнить метрики двух продуктов\n"
                  << "0 — Выход\n";
        int choice;
        if (!readNumber("Ваш выбор: ", choice)) {
            continue;
        }
        if (choice == 0) 
        {
            break;
        }
        switch (choice) 
        {
        case 1:
            createProduct(collection);
            break;
        case 2:
            printCollection(collection);
            break;
        case 3:
            std::cout << "Суммарная метрика: " << std::fixed << std::setprecision(2)
                      << collection.totalMetric() << std::defaultfloat << std::setprecision(6) << '\n';
            break;
        case 4:
            collection.sortByMetric();
            std::cout << "Коллекция отсортирована по возрастанию метрики.\n";
            break;
        case 5:
        case 6:
        case 7: {
            std::size_t index;
            if (!readIndex(collection, index)) {
                break;
            }
            if (choice == 5) {
                collection.remove(index);
                std::cout << "Продукт удалён.\n";
            }
            else if (choice == 6) {
                showSpecificMethod(collection.at(index));
            }
            else {
                std::cout << static_cast<double>(collection.at(index)) << '\n';
            }
            break;
        }
        case 8: {
            std::size_t first, second;
            if (!readIndex(collection, first) || !readIndex(collection, second)) {
                break;
            }
            std::cout << "Метрики равны (0 — нет, 1 — да): "
                      << (collection.at(first) == collection.at(second)) << '\n';
            break;
        }
        default:
            std::cout << "Неизвестный пункт меню.\n";
        }
    }
}
