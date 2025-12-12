#pragma once
#include <string>
#include <vector>
#include "contact.h"

// Класс, который отвечает за работу с файлом контактов.
// На первом этапе у нас одно хранилище – обычный текстовый файл.
class FileRepository {
public:
    // Путь к файлу, где лежат контакты (например, "contacts.txt" или "contacts.json").
    explicit FileRepository(const std::string& filePath);

    // Считать все контакты из файла.
    // Если файл пустой или его нет – вернётся пустой список.
    std::vector<Contact> loadAll() const;

    // Полностью перезаписать файл списком контактов.
    // Обычно используется после добавления/удаления/редактирования.
    void saveAll(const std::vector<Contact>& contacts) const;

private:
    std::string path; // путь к файлу на диске
};