#pragma once
#include <string>


std::string trim(const std::string& s); //delete space from start and end

bool validateName(const std::string& s); //name surname check

bool validateEmail(const std::string& s); //email check

bool validatePhone(const std::string& s); //phone check with formating

bool validateBirthDate(const std::string& s); //birthday check