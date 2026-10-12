#include "PhoneBook.h"
#include "Exceptions.h"

std::size_t PhoneBook::checkedIndex(int number) const {
    if (number <= 0 || static_cast<std::size_t>(number) > subscribers_.size()) {
        throw ContactNotFoundException();
    }
    return static_cast<std::size_t>(number - 1);
}

PhoneBook PhoneBook::operator+(const Subscriber& subscriber) const {
    PhoneBook result(*this); // Виклик конструктора копіювання.
    result.subscribers_.push_back(subscriber);
    return result;
}

Subscriber& PhoneBook::at(int number) {
    return subscribers_[checkedIndex(number)];
}

void PhoneBook::remove(int number) {
    const auto index = checkedIndex(number);
    subscribers_.erase(subscribers_.begin() +
                       static_cast<std::vector<Subscriber>::difference_type>(index));
}

const std::vector<Subscriber>& PhoneBook::getSubscribers() const {
    return subscribers_;
}

std::vector<Subscriber> PhoneBook::findPhone(const std::string& surname) const {
    return findPhone(surname, false);
}

std::vector<Subscriber> PhoneBook::findPhone(const std::string& surname,
                                           bool partialMatch) const {
    if (surname.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw InvalidInputException("Введіть прізвище або його частину!");
    }

    std::vector<Subscriber> matches;
    for (const auto& subscriber : subscribers_) {
        const bool found = partialMatch
            ? subscriber.getSurname().find(surname) != std::string::npos
            : subscriber.getSurname() == surname;
        if (found) {
            matches.push_back(subscriber);
        }
    }
    if (matches.empty()) {
        throw ContactNotFoundException();
    }
    return matches;
}
