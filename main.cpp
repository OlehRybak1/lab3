#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "PhoneBook.h"
#include "Exceptions.h"
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

std::string readLine(const std::string& prompt,
                     std::size_t maxBytes = std::string::npos) {
    std::cout << prompt;
    std::string text;
    char c = '\0';
    while (std::cin.get(c)) {
        if (c == '\n') {
            return text;
        }
        if (text.size() >= maxBytes) {
            // Прибираємо весь залишок довгого рядка без його збереження.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InvalidInputException(
                "Прізвище та ім'я можуть містити не більше 50 символів кожне!");
        }
        text += c;
    }
    if (text.empty() || !std::cin.eof()) {
        throw InputClosedException();
    }
    return text;
}

int readInteger(const std::string& prompt) {
    std::cout << prompt;
    int value = 0;
    if (!(std::cin >> value)) {
        if (std::cin.eof()) {
            throw InputClosedException();
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException("Потрібно ввести ціле число в межах типу int!");
    }

    // Забороняємо частково прочитані числа на кшталт 2abc або 1.5.
    std::string rest;
    std::getline(std::cin, rest);
    if (rest.find_first_not_of(" \t\r") != std::string::npos) {
        throw InvalidInputException("Введіть лише одне ціле число!");
    }
    return value;
}

void showContacts(const std::vector<Subscriber>& contacts) {
    if (contacts.empty()) {
        std::cout << "Телефонна книга порожня.\n";
        return;
    }
    std::cout << "№ | Прізвище | Ім'я | Номер\n"
              << "------------------------------------------------\n";
    for (std::size_t i = 0; i < contacts.size(); ++i) {
        std::cout << i + 1 << " | " << contacts[i].getSurname()
                  << " | " << contacts[i].getFirstName()
                  << " | " << contacts[i].getPhone() << '\n';
    }
}

void showMenu() {
    std::cout << "\n================ ТЕЛЕФОННА КНИГА ================\n"
              << "[1] Переглянути всі контакти\n"
              << "[2] Додати нового абонента\n"
              << "[3] Змінити номер абонента\n"
              << "[4] Пошук номера телефону за прізвищем\n"
              << "[5] Видалити абонента за порядковим номером\n"
              << "[0] Вихід\n"
              << "=================================================\n";
}

bool pauseAfterAction() {
    if (std::cin.eof()) {
        return false;
    }
#ifdef _WIN32
    std::cout << std::flush;
    std::system("pause");
    return true;
#else
    std::cout << "Натисніть Enter, щоб повернутися в меню...";
    std::string line;
    return static_cast<bool>(std::getline(std::cin, line));
#endif
}

int main() {
#ifdef _WIN32
    // Вихідні файли та параметр компілятора /utf-8 мають використовувати UTF-8.
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    PhoneBook book;
    bool running = true;
    while (running) {
        showMenu();
        try {
            const int choice = readInteger("Ваш вибір: ");
            switch (choice) {
            case 1:
                showContacts(book.getSubscribers());
                break;

            case 2: {
                // Один символ UTF-8 займає до 4 байтів.
                constexpr auto maxNameBytes = 4 * Subscriber::MAX_NAME_LENGTH;
                const std::string surname =
                    readLine("Прізвище (до 50 символів): ", maxNameBytes);
                Subscriber::validateName(surname);
                const std::string firstName =
                    readLine("Ім'я (до 50 символів): ", maxNameBytes);
                Subscriber::validateName(firstName);
                const std::string phone = readLine("Номер телефону (10 цифр): ");
                book = book + Subscriber(surname, firstName, phone);
                std::cout << "Абонента додано.\n";
                break;
            }

            case 3: {
                if (book.getSubscribers().empty()) {
                    throw ContactNotFoundException();
                }
                showContacts(book.getSubscribers());
                const int number = readInteger("Порядковий номер абонента: ");
                Subscriber& subscriber = book.at(number);
                const std::string phone = readLine("Новий номер (10 цифр): ");
                subscriber = phone; // Перевантажений оператор =.
                std::cout << "Номер телефону змінено.\n";
                break;
            }

            case 4: {
                std::cout << "[1] Точний збіг\n[2] Частковий збіг\n";
                const int mode = readInteger("Спосіб пошуку: ");
                if (mode != 1 && mode != 2) {
                    throw InvalidInputException("Оберіть спосіб пошуку 1 або 2!");
                }
                const std::string surname = readLine("Прізвище або його частина: ");
                const auto matches = mode == 1
                    ? book.findPhone(surname)
                    : book.findPhone(surname, true);
                std::cout << "Знайдені контакти (нумерація результатів):\n";
                showContacts(matches);
                break;
            }

            case 5: {
                if (book.getSubscribers().empty()) {
                    throw ContactNotFoundException();
                }
                showContacts(book.getSubscribers());
                const int number = readInteger("Порядковий номер для видалення: ");
                book.remove(number);
                std::cout << "Абонента видалено.\n";
                break;
            }

            case 0:
                running = false;
                break;

            default:
                throw InvalidInputException("Оберіть пункт меню від 0 до 5!");
            }
        }
        catch (const InputClosedException&) {
            running = false;
        }
        catch (const std::exception& error) {
            std::cout << "[ПОМИЛКА]: " << error.what() << '\n';
        }

        if (running && !pauseAfterAction()) {
            running = false;
        }
    }
    std::cout << "Роботу програми завершено.\n";
    return 0;
}
