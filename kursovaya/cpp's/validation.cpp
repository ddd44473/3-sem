#include "../hpps/validation.h"
#include <regex>
#include <cctype>
#include <chrono>
#include <ctime>

// ---------- helpers ----------
std::string trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace((unsigned char)s[start])) {
        ++start;
    }

    size_t end = s.size();
    while (end > start && std::isspace((unsigned char)s[end - 1])) {
        --end;
    }

    return s.substr(start, end - start);
}

static std::string removeSpaces(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (unsigned char ch : s) {
        if (!std::isspace(ch)) out.push_back((char)ch);
    }
    return out;
}

static bool isLeap(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

static int daysInMonth(int year, int month)
{
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeap(year) ? 29 : 28;
        default: return 0;
    }
}

// ---------- validators ----------
bool validateName(const std::string& s)
{
    std::string t = trim(s);
    if (t.empty()) return false;

    // first char must be a letter (latin OR "extended" bytes)
    unsigned char c0 = (unsigned char)t[0];
    bool firstIsLatin =
        (c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z');
    bool firstIsNonAsciiLetter = (c0 >= '\xC0' && c0 <= '\xFF'); // "примерно кириллица" в однобайтовой кодировке

    if (!(firstIsLatin || firstIsNonAsciiLetter)) return false;

    // cannot start/end with '-'
    if (t.front() == '-' || t.back() == '-') return false;

    // allowed: letters/digits, space, '-'
    for (unsigned char ch : t) {
        if (std::isalnum(ch)) continue;
        if (ch == ' ' || ch == '-') continue;
        return false;
    }

    return true;
}

bool validateEmail(const std::string& s)
{
    // по ТЗ: удалять пробелы, в т.ч. вокруг '@'
    std::string t = removeSpaces(trim(s));

    // user@domain (латиница+цифры)
    static const std::regex re(R"(^[A-Za-z0-9]+@[A-Za-z0-9]+$)");
    return std::regex_match(t, re);
}

bool validatePhone(const std::string& s)
{
    std::string t = trim(s);

    // строго под примеры из ТЗ (код 812)
    static const std::regex re(
        R"(^(\+7|8)(812|\(812\))(\d{7}|\d{3}-\d{2}-\d{2})$)"
    );

    return std::regex_match(t, re);
}

bool validateBirthDate(const std::string& s)
{
    std::string t = trim(s);

    // YYYY-MM-DD
    static const std::regex re(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(t, re)) return false;

    int year  = std::stoi(t.substr(0, 4));
    int month = std::stoi(t.substr(5, 2));
    int day   = std::stoi(t.substr(8, 2));

    if (month < 1 || month > 12) return false;

    int dim = daysInMonth(year, month);
    if (dim == 0) return false;
    if (day < 1 || day > dim) return false;

    // compare with today's date (midnight)
    std::tm tm_date{};
    tm_date.tm_year = year - 1900;
    tm_date.tm_mon  = month - 1;
    tm_date.tm_mday = day;
    tm_date.tm_hour = 0;
    tm_date.tm_min  = 0;
    tm_date.tm_sec  = 0;

    std::time_t birth_time = std::mktime(&tm_date);
    if (birth_time == -1) return false;

    using namespace std::chrono;
    std::time_t now_t = system_clock::to_time_t(system_clock::now());
    std::tm* now_tm = std::localtime(&now_t);

    now_tm->tm_hour = 0;
    now_tm->tm_min  = 0;
    now_tm->tm_sec  = 0;
    std::time_t today_midnight = std::mktime(now_tm);

    return birth_time < today_midnight;
}
