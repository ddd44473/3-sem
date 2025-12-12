#include "../hpps/file_repository.h"
#include <fstream>   // std::ifstream, std::ofstream

// Сохраняем путь к файлу (например, "contacts.txt")
FileRepository::FileRepository(const std::string& filePath)
    : path(filePath)
{}

// Чтение всех контактов из файла
std::vector<Contact> FileRepository::loadAll() const
{
    std::vector<Contact> result;

    std::ifstream in(path);
    if (!in.is_open()) {
        return result;
    }

    std::string line;
    std::string block;

    while (std::getline(in, line)) {
        if (line == "----") {
            // закончили блок – парсим его
            if (!block.empty()) {
                Contact c = Contact::deserialize(block);
                result.push_back(std::move(c));
                block.clear();
            }
        } else {
            // накапливаем строки внутри блока
            if (!block.empty())
                block += '\n';
            block += line;
        }
    }

    // если файл не завершился "----", но блок есть – тоже распарсим
    if (!block.empty()) {
        Contact c = Contact::deserialize(block);
        result.push_back(std::move(c));
    }

    return result;
}

// Полная перезапись файла списком контактов
void FileRepository::saveAll(const std::vector<Contact>& contacts) const
{
    // открываем файл на запись с очисткой (trunc – удалить старое содержимое)
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) {
        // тут можно было бы кинуть исключение или залогировать ошибку,
        // но для курсовой достаточно просто "тихо" выйти
        return;
    }

    for (const auto& c : contacts) {
        // предполагаем, что serialize() возвращает строку
        // (например JSON или свой формат)
        out << c.serialize() << '\n';
    }
}