#include <iostream>
#include <limits>      // std::numeric_limits
#include "../hpps/service.h"

// вспомогательная функция, чтобы считать строку целиком
static void readLine(const std::string& prompt, std::string& out)
{
    std::cout << prompt;
    std::getline(std::cin, out);
}

int main()
{
    // Создаём сервис и указываем файл, в котором будут храниться контакты.
    // Можно назвать как угодно: "contacts.txt", "contacts.db" и т.п.
    ContactService service("contacts.txt");

    while (true) {
        std::cout << "\n==== PHONE BOOK ====\n";
        std::cout << "1. Add contact\n";
        std::cout << "2. List contacts\n";
        std::cout << "3. Delete contact by id\n";
        std::cout << "0. Exit\n";
        std::cout << "Select: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            // если ввод не число — очищаем и продолжаем
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
            continue;
        }

        // съедаем остаток строки после числа, чтобы getline работал корректно
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 0) {
            std::cout << "Goodbye!\n";
            break;
        }

        if (choice == 1) {
            // --- Добавление контакта ---
            std::string firstname, lastname, middlename;
            std::string address, birthday, email;
            std::string phoneNumber;

            readLine("First name: ", firstname);
            readLine("Last name: ", lastname);
            readLine("Middle name (optional): ", middlename);
            readLine("Address (optional): ", address);
            readLine("Birth date (YYYY-MM-DD, optional): ", birthday);
            readLine("Email: ", email);
            readLine("Phone number: ", phoneNumber);

            // Пока не спрашиваем тип телефона у пользователя,
            // просто считаем, что это рабочий номер.
            PhoneNumber p;
            p.type   = PhoneType::Work;
            p.number = phoneNumber;

            std::vector<PhoneNumber> phones;
            phones.push_back(p);

            std::string error;
            bool ok = service.createContact(
                firstname,
                lastname,
                middlename,
                address,
                birthday,
                email,
                phones,
                error
            );

            if (!ok) {
                std::cout << "Error: " << error << "\n";
            } else {
                std::cout << "Contact created.\n";
            }
        }
        else if (choice == 2) {
            // --- Вывод списка контактов ---
            const auto& all = service.getAll();
            if (all.empty()) {
                std::cout << "No contacts.\n";
            } else {
                std::cout << "\nContacts:\n";
                for (const auto& c : all) {
                    std::cout << "-------------------------\n";
                    std::cout << "ID: " << c.id << "\n";
                    std::cout << "Name: " << c.firstname << " " << c.lastname << "\n";
                    if (!c.middlename.empty())
                        std::cout << "Middle: " << c.middlename << "\n";
                    if (!c.address.empty())
                        std::cout << "Address: " << c.address << "\n";
                    if (!c.birthday.empty())
                        std::cout << "Birth: " << c.birthday << "\n";
                    std::cout << "Email: " << c.email << "\n";

                    std::cout << "Phones:\n";
                    for (const auto& p : c.phones) {
                        std::string typeStr;
                        switch (p.type) {
                            case PhoneType::Work:    typeStr = "Work";    break;
                            case PhoneType::Home:    typeStr = "Home";    break;
                            case PhoneType::Service: typeStr = "Service"; break;
                        }
                        std::cout << "  [" << typeStr << "] " << p.number << "\n";
                    }
                }
                std::cout << "-------------------------\n";
            }
        }
        else if (choice == 3) {
            // --- Удаление по id ---
            std::cout << "Enter id to delete: ";
            int id;
            if (!(std::cin >> id)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid id.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (service.deleteById(id)) {
                std::cout << "Contact deleted.\n";
            } else {
                std::cout << "Contact with this id not found.\n";
            }
        }
        else {
            std::cout << "Unknown option. Try again.\n";
        }
    }

    return 0;
}
