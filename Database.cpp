#include<iostream>
#include<map>
#include<fstream>
#include<string>
#include<exception>
#include<vector>
#include<iomanip>
#include<sstream>
#include<set>


class Date {
public:
    Date() {}
    Date(int new_year, int new_month, int new_day) {
        if (new_month < 1 || new_month > 12) {
            throw std::invalid_argument("Month value is invalid: " + std::to_string(new_month));
        }
        if (new_day < 1 || new_day > 31) {
            throw std::invalid_argument("Day value is invalid: " + std::to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
private:
    int year;
    int month;
    int day;
};


Date ParseDate(const std::string& date_str) {
    std::stringstream is(date_str);
    int y, m, d;
    char c, ch, chr;
    std::string str;
    if (is) {
        is >> y >> c >> m >> ch >> d;
        if (is) {
            if (c == '-' && ch == '-' && is.peek() == EOF) { 
                return Date(y, m, d);
            }
            else {
                //is.setstate(std::ios_base::failbit);
                throw std::invalid_argument("Wrong date format: " + date_str);
            }
        }
        else { throw std::invalid_argument("Wrong date format: " + date_str); }

    }
}

std::ostream& operator << (std::ostream& os, const Date& d) {
    return os << std::setfill('0') << std::setw(4) << d.GetYear() << '-' <<
        std::setfill('0') << std::setw(2) << d.GetMonth() << '-' <<
        std::setfill('0') << std::setw(2) << d.GetDay();
}

bool operator==(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() == rhs.GetDay();
        }
    }
    return false;
}
bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        }
        else { return lhs.GetMonth() < rhs.GetMonth(); }

    }
    return lhs.GetYear() < rhs.GetYear();
}

class Database {
public:
    void AddEvent(const Date& date, const std::string& event) {
        m[date].insert(event);

    }
    bool DeleteEvent(const Date& date, const std::string& event) {
        bool fl = false;
        std::string s;
        for (const auto& it : m) {
            if (it.first == date) {
                for (const auto& x : it.second) {
                    if (x == event) { fl = true; s = event; }
                }
            }
        }
        m[date].erase(s);
        return fl;
    }
    int  DeleteDate(const Date& date) {
        int counter = 0;
        auto it = this->m.find(date);
        if (it != this->m.end()) {
            counter = it->second.size();
            this->m.erase(it);
        }
        return counter;
    }

    void Find(const Date& date) const {
        auto it = this->m.find(date);
        if (it != this->m.end()) {
            for (const auto& x : it->second) {
                std::cout << x << std::endl;
            }
        }
    }

    void Print() const {
        for (const auto& item : m) {
            for (const auto& it : item.second) {
                std::cout << item.first << " " << it << std::endl;
            }
        }
    }
private:
    std::map < Date, std::set <std::string >> m;
};

int main() {
    // Database dt;

    std::string command;
    try {
        Database dt;
        while (getline(std::cin, command)) {
            std::stringstream ss(command);
            std::string strin;
            ss >> strin;
            if (strin == "Add") {
                std::string date;
                std::string str;
                ss >> date;
                Date d = ParseDate(date);
                ss >> str;
                dt.AddEvent(d, str);
            }
            else if (strin == "Del") {
                std::string date;
                ss >> date;
                Date d = ParseDate(date);
                std::string str;
                ss >> str;
                if (!str.empty()) {
                    bool b = dt.DeleteEvent(d, str);
                    if (b == true) {
                        std::cout << "Deleted successfully" << std::endl;
                    }
                    else { std::cout << "Event not found" << std::endl; }
                }
                else {
                    int a = dt.DeleteDate(d);
                    std::cout << "Deleted " << a << " events" << std::endl;
                }
            }
            else if (strin == "Find") {
                std::string date;
                ss >> date;
                Date d = ParseDate(date);
                dt.Find(d);
            }
            else if (strin == "Print") {
                dt.Print();
            }
            else if (strin != "") {
                throw std::invalid_argument("Unknown command: " + strin);
                
            }

        }
    }
    catch (std::invalid_argument & ia) {
        std::cout << ia.what() << std::endl;
    }

    return 0;
}