#include "../hpps/validation.h"
#include <regex>
#include <cctype>  
#include <chrono>   
#include <ctime>


std::string trim(const std::string& s)
{
    size_t start = 0;

    while (start < s.size() && std::isspace((unsigned char)s[start])) { //delete space from start
        start++;
    }

    size_t end = s.size();

    while (end > start && std::isspace((unsigned char)s[end - 1])) { //delete space from end
        end--;
    }

    return s.substr(start, end - start); //returning string without spaces
}


bool validateName(const std::string& s)
{
    std::string t = trim(s);
    if (t.empty()) return false;

    unsigned char c0 = (unsigned char)t[0]; //first symbol must starts with letter
    if (!((c0 >= 'A' && c0 <= 'Z') || (c0 >= 'a' && c0 <= 'z') ||
          (c0 >= '\xC0' && c0 <= '\xFF'))) { //cyrillic or not check
        return false;
    }

    if (t.back() == '-') return false; //can't end with '-'

    //allowing symbols 
    for (unsigned char ch : t) { 
        if (std::isalnum(ch)) continue; //letters/numbers
        if (ch == ' ' || ch == '-') continue; //space or '-'
        return false;
    }

    return true;
}



bool validateEmail(const std::string& s)
{
    std::string t = trim(s);

    if (t.find(' ') != std::string::npos) return false; //disable spaces

    static const std::regex re(R"(^[A-Za-z0-9]+@[A-Za-z0-9]+$)"); //only latin letters and numbers

    return std::regex_match(t, re);
}


//allowing formats from exercise:
//+78121234567
//88121234567
//+7(812)1234567
//8(812)1234567
//+7(812)123-45-67
//8(812)123-45-67

bool validatePhone(const std::string& s)
{
    std::string t = trim(s);

    static const std::regex re(
        R"(^(\+7|8)(812|\(812\))(\d{7}|\d{3}-\d{2}-\d{2})$)" //regex - creating expression
    );

    return std::regex_match(t, re);
}



static bool isLeap(int year)
{
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0); //leap year check
}

bool validateBirthDate(const std::string& s)
{
    std::string t = trim(s);

    //YYYY-MM-DD chtck
    static const std::regex re(R"(^\d{4}-\d{2}-\d{2}$)"); 
    if (!std::regex_match(t, re)) return false;

    int year  = std::stoi(t.substr(0, 4));
    int month = std::stoi(t.substr(5, 2));
    int day   = std::stoi(t.substr(8, 2));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    int daysInMonth;
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            daysInMonth = 31; break;
        case 4: case 6: case 9: case 11:
            daysInMonth = 30; break;
        case 2:
            daysInMonth = isLeap(year) ? 29 : 28; break;
        default:
            return false;
    }
    if (day > daysInMonth) return false;

    //date < today check
    using namespace std::chrono;
    std::tm tm_date{};
    tm_date.tm_year = year - 1900;
    tm_date.tm_mon  = month - 1;
    tm_date.tm_mday = day;

    std::time_t birth_time = std::mktime(&tm_date);
    if (birth_time == -1) return false;

    auto now      = system_clock::now();
    std::time_t tnow = system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&tnow);

    //reset nowday
    now_tm->tm_hour = 0;
    now_tm->tm_min  = 0;
    now_tm->tm_sec  = 0;
    std::time_t today_midnight = std::mktime(now_tm);

    //birthday must be > nowday
    return birth_time < today_midnight;
}