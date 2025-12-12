#include "../hpps/service.h"

// Конструктор: настраиваем FileRepository и загружаем контакты из файла.
ContactService::ContactService(const std::string& filePath)
    : repo(filePath)
{
    loadFromFile();
}

// Чтение из файла в память + вычисление nextId.
void ContactService::loadFromFile()
{
    contacts = repo.loadAll();

    // Найдём максимальный id, чтобы выдавать новый +1
    int maxId = 0;
    for (const auto& c : contacts) {
        if (c.id > maxId) {
            maxId = c.id;
        }
    }
    nextId = maxId + 1;
}

void ContactService::saveToFile()
{
    repo.saveAll(contacts);
}

const std::vector<Contact>& ContactService::getAll() const
{
    return contacts;
}

bool ContactService::createContact(
    const std::string& firstname,
    const std::string& lastname,
    const std::string& middlename,
    const std::string& address,
    const std::string& birthday,
    const std::string& email,
    const std::vector<PhoneNumber>& phones,
    std::string& errorMessage
)
{
    // --- Проверки по ТЗ ---

    // Обязательные поля: имя, фамилия, email, хотя бы один номер
    if (!validateName(firstname)) {
        errorMessage = "Invalid first name";
        return false;
    }
    if (!validateName(lastname)) {
        errorMessage = "Invalid last name";
        return false;
    }

    // Отчество может быть пустым, но если не пустое — проверяем
    if (!middlename.empty() && !validateName(middlename)) {
        errorMessage = "Invalid middle name";
        return false;
    }

    // Дата может быть пустой, но если есть — проверяем
    if (!birthday.empty() && !validateBirthDate(birthday)) {
        errorMessage = "Invalid birth date (must be in format YYYY-MM-DD and < today)";
        return false;
    }

    if (!validateEmail(email)) {
        errorMessage = "Invalid email";
        return false;
    }

    if (phones.empty()) {
        errorMessage = "Contact must have at least one phone number";
        return false;
    }

    // Проверяем каждый номер по формату из задания
    for (const auto& p : phones) {
        if (!validatePhone(p.number)) {
            errorMessage = "Invalid phone number: " + p.number;
            return false;
        }
    }

    // --- Если всё ок, создаём контакт ---

    Contact c;
    c.id         = nextId++;
    c.firstname  = trim(firstname);
    c.lastname   = trim(lastname);
    c.middlename = trim(middlename);
    c.address    = trim(address);
    c.birthday   = trim(birthday);
    c.email      = trim(email);
    c.phones     = phones; // здесь уже проверенные номера

    contacts.push_back(c);
    saveToFile(); // сразу сохраняем изменения в файл

    errorMessage.clear();
    return true;
}

bool ContactService::deleteById(int id)
{
    for (auto it = contacts.begin(); it != contacts.end(); ++it) {
        if (it->id == id) {
            contacts.erase(it);
            saveToFile();
            return true;
        }
    }
    return false; // контакт с таким id не найден
}