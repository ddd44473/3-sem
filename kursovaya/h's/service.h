#pragma once
#include <vector>
#include <string>
#include "contact.h"
#include "file_repository.h"
#include "validation.h"

// Сервисный класс: вся логика работы с контактами.
// Хранит контакты в памяти и сохраняет их в файл через FileRepository.
class ContactService {
public:
    // В конструктор передаём путь к файлу (тот же, что в FileRepository).
    explicit ContactService(const std::string& filePath);

    // Получить все контакты (для отображения в консоли/GUI).
    const std::vector<Contact>& getAll() const;

    // Создать новый контакт из полей.
    // Возвращает true, если всё ок. Если ошибка — false и заполняет errorMessage.
    bool createContact(
        const std::string& firstname,
        const std::string& lastname,
        const std::string& middlename,
        const std::string& address,
        const std::string& birthday,
        const std::string& email,
        const std::vector<PhoneNumber>& phones,
        std::string& errorMessage
    );

    // Удалить контакт по id. Возвращает true, если контакт найден и удалён.
    bool deleteById(int id);

private:
    FileRepository repo;          // работа с файлом
    std::vector<Contact> contacts; // контакты в памяти
    int nextId = 1;               // следующий id для нового контакта

    void loadFromFile();  // загрузить contacts из файла
    void saveToFile();    // сохранить contacts в файл
};