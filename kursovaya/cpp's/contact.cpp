#include "../hpps/contact.h"
#include <utility> 
#include <cstdint>   
#include <sstream>   

//static counters
std::atomic<uint64_t> Contact::created_count{0}; //created contacts count
std::atomic<uint64_t> Contact::copy_count{0}; //copy contacts count
std::atomic<uint64_t> Contact::move_count{0}; //move contacts count


Contact::Contact() {
    ++created_count;
}

Contact::~Contact() = default; //default destructor

Contact::Contact(const Contact& other) //copy constructor
    : id(other.id),
      firstname(other.firstname),
      lastname(other.lastname),
      middlename(other.middlename),
      address(other.address),
      birthday(other.birthday),
      email(other.email),
      phones(other.phones)
{
    ++copy_count;
}

Contact::Contact(Contact&& other) noexcept //move constructor
    : id(other.id),
      firstname(std::move(other.firstname)),
      lastname(std::move(other.lastname)),
      middlename(std::move(other.middlename)),
      address(std::move(other.address)),
      birthday(std::move(other.birthday)),
      email(std::move(other.email)),
      phones(std::move(other.phones))
{
    ++move_count;
    other.id = 0;
}

Contact& Contact::operator=(const Contact& other) { //copy assignment
    if (this != &other) {
        ++copy_count;
        id         = other.id;
        firstname  = other.firstname;
        lastname   = other.lastname;
        middlename = other.middlename;
        address    = other.address;
        birthday   = other.birthday;
        email      = other.email;
        phones     = other.phones;
    }
    return *this;
}

Contact& Contact::operator=(Contact&& other) noexcept { //move assignment
    if (this != &other) {
        ++move_count;
        id         = other.id;
        firstname  = std::move(other.firstname);
        lastname   = std::move(other.lastname);
        middlename = std::move(other.middlename);
        address    = std::move(other.address);
        birthday   = std::move(other.birthday);
        email      = std::move(other.email);
        phones     = std::move(other.phones);
        other.id = 0;
    }
    return *this;
}


std::string Contact::serialize() const { return {}; }
Contact Contact::deserialize(const std::string&) { return {}; }
