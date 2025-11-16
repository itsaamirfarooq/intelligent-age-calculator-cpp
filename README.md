# Intelligent Age Calculator & Life Stage Classifier (C++)

This project is a console-based intelligent age calculation system developed using Object-Oriented Programming (OOP) in modern C++. It accurately computes the user’s exact age in years, months, days, and total days, based on the provided date of birth. The program integrates real-time system date APIs to ensure accurate and current age calculation without requiring manual date input.
The application also identifies the day of the week on which the user was born, using a mathematical day-of-week algorithm (Sakamoto's Algorithm) for high precision and reliability. Additionally, it classifies the user into a relevant life stage category such as Baby, Child, Teen, Adult, or Senior based on their computed age.
A strong emphasis is placed on robust input validation. The program accepts multiple date formats (DD-MM-YYYY, DD/MM/YYYY, DD MM YYYY) and performs comprehensive validation including leap year checks, invalid date detection, future date prevention, and age realism verification (maximum 130 years). This ensures accuracy, user safety, and a professional user experience.
The overall design follows clean OOP architecture, separating functionality into dedicated classes such as Date, Age, AgeCalculator, and Input. This modular approach improves readability, reusability, and maintainability of the code. The project serves as an excellent demonstration of practical C++ skills, real-world algorithm application, and structured programming design.

# Features
- Exact age calculation (Years/Months/Days + Total Days)
- Birth-day detection using Sakamoto’s Algorithm
- Real-time system date integration
- Life stage classification (Baby → Senior)
- Robust input validation for date formats and edge cases
- Leap-year support and Julian Day Number (JDN) for accurate day counts
- Clean OOP architecture (Date, Age, Input, AgeCalculator)

# Technologies
- C++
- <ctime> for system date
- OOP principles and algorithms

# How to Run
Compile with any C++ compiler:

# g++ main.cpp -o agecalc
./agecalc

# Purpose
This project is suitable for beginners learning OOP, students building academic projects, and developers exploring date/time calculations in C++. To demonstrate date handling, algorithmic logic, and OOP design in C++.
