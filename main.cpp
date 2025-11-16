#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

// Date class: holds a Gregorian date and basic date utilities
class Date {
private:
    int day, month, year;

public:
    Date() : day(1), month(1), year(1970) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    int  getDay()   const { return day;   }
    int  getMonth() const { return month; }
    int  getYear()  const { return year;  }

    static bool isLeapYear(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    static int daysInMonth(int m, int y) {
        static const int dm[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
        if (m == 2 && isLeapYear(y)) return 29;
        return dm[m - 1];
    }

    bool isValid() const {
        if (year < 1 || month < 1 || month > 12 || day < 1) return false;
        if (day > daysInMonth(month, year)) return false;
        return true;
    }

    std::string toString() const {
        std::ostringstream os;
        os << std::setfill('0')
           << std::setw(2) << day << "-"
           << std::setw(2) << month << "-"
           << std::setw(4) << year;
        return os.str();
    }

    // Day of week using Sakamoto's algorithm: 0=Sunday..6=Saturday
    int dayOfWeek() const {
        int y = year, m = month;
        static const int t[12] = {0,3,2,5,0,3,5,1,4,6,2,4};
        y -= (m < 3);
        int w = (y + y/4 - y/100 + y/400 + t[m-1] + day) % 7;
        if (w < 0) w += 7;
        return w;
    }

    std::string dayName() const {
        static const char* names[7] = {
            "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
        };
        return names[dayOfWeek()];
    }

    // Convert to Julian Day Number (Gregorian). Useful for total days.
    long long toJDN() const {
        int a = (14 - month) / 12;
        int y = year + 4800 - a;
        int m = month + 12 * a - 3;
        long long jdn = day
                      + (153LL * m + 2) / 5
                      + 365LL * y + y / 4 - y / 100 + y / 400
                      - 32045;
        return jdn;
    }

    // Get system "today" using <ctime> (local time).
    static Date today() {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    }

    // Comparison for validation (<=)
    bool operator<=(const Date& other) const {
        if (year  != other.year)  return year  < other.year;
        if (month != other.month) return month < other.month;
        return day <= other.day;
    }
};

// Age result "data class"
struct Age {
    int years;
    int months;
    int days;
    long long totalDays;
};

// AgeCalculator: computes age breakdown & life stage
class AgeCalculator {
public:
    // Compute Y-M-D age and total days
    static Age compute(const Date& dob, const Date& today) {
        Age result{0,0,0,0};

        // Total days via JDN difference (robust and fast)
        long long diff = today.toJDN() - dob.toJDN();
        if (diff < 0) diff = 0; // safety
        result.totalDays = diff;

        // Years-Months-Days calculation with borrowing
        int y = today.getYear()  - dob.getYear();
        int m = today.getMonth() - dob.getMonth();
        int d = today.getDay()   - dob.getDay();

        if (d < 0) {
            // Borrow days from the previous month of 'today'
            int pm = today.getMonth() - 1;
            int py = today.getYear();
            if (pm == 0) { pm = 12; py -= 1; }
            d += Date::daysInMonth(pm, py);
            m -= 1;
        }
        if (m < 0) {
            m += 12;
            y -= 1;
        }
        if (y < 0) { y = m = d = 0; } // future date guard (shouldn't happen after validation)

        result.years  = y;
        result.months = m;
        result.days   = d;
        return result;
    }

    // Life Stage classification (based on completed years)
    static std::string classify(int years) {
        if (years <= 2)  return "Baby (0�2)";
        if (years <= 12) return "Child (3�12)";
        if (years <= 19) return "Teen (13�19)";
        if (years <= 59) return "Adult (20�59)";
        return "Senior (60+)";
    }
};

// Simple input helper for robust DOB entry & checks
class Input {
public:
    static Date readDOB() {
        Date today = Date::today();

        // Clear any leftover newline before getline (optional safety)
        if (std::cin.peek() == '\n') std::cin.ignore();

        while (true) {
            std::cout << "Enter your Date of Birth "
                         "(DD-MM-YYYY or DD/MM/YYYY or DD MM YYYY): ";

            std::string line;
            std::getline(std::cin, line);
            if (line.empty()) {
                std::cout << "Please type something.\n";
                continue;
            }

            // Allow '-', '/', '.', or space as separators
            for (char& c : line)
                if (c == '-' || c == '/' || c == '.') c = ' ';

            int d = 0, m = 0, y = 0;
            std::istringstream iss(line);
            if (!(iss >> d >> m >> y)) {
                std::cout << "Invalid format. Example: 07 02 2005 or 07-02-2005\n";
                continue;
            }

            Date dob(d, m, y);
            if (!dob.isValid()) {
                std::cout << "That date is not valid on the calendar.\n";
                continue;
            }
            if (!(dob <= today)) {
                std::cout << "Date of birth cannot be in the future.\n";
                continue;
            }
            if (today.getYear() - y > 130) {
                std::cout << "Unrealistic age (>130 years). Please re-enter.\n";
                continue;
            }

            return dob;
        }
    }
};

// Program entry
int main() {
    std::cout << "=== Intelligent Age Calculator & Life Stage Classifier ===\n\n";

    // 1) Input DOB with validation
    Date dob = Input::readDOB();

    // 2) Get current date from system (real-time)
    Date today = Date::today();

    // 3) Compute age & life stage
    Age age = AgeCalculator::compute(dob, today);
    std::string stage = AgeCalculator::classify(age.years);

    // 4) Display as requested (simple, clear formatting)
    std::cout << "\n---------------------------------------------\n";
    std::cout << "DOB: " << dob.toString()
              << "   Day of Birth: " << dob.dayName() << "\n";

    std::cout << "Current Date: " << today.toString()
              << "   Current Day: " << today.dayName() << "\n";

    std::cout << "Stage of life: " << stage << "\n";

    std::cout << "Age of the User: "
              << age.years  << " years, "
              << age.months << " months, "
              << age.days   << " days\n";

    std::cout << "Age of the user in days: "
              << age.totalDays << " (total days)\n";
    std::cout << "---------------------------------------------\n";

    std::cout << "Thank you!\n";
    return 0;
}
