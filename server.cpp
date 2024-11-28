#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <conio.h> // ��� ������� _kbhit() � _getch()
#include <cstdio> // ��� ������� std::remove

using namespace std;

struct Person {
    char name[25];
    int day;
    int month;
    int year;
} B;

// ��������� ��� �������� ��������
struct Age {
    int years;
    int months;
    int days;
};

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// ������� ��� ������� ��������
Age calculateAge(int day, int month, int year) {
    using namespace std::chrono;

    auto now = system_clock::now();
    time_t now_c = system_clock::to_time_t(now);
    tm now_tm;
    localtime_s(&now_tm, &now_c);

    int current_year = now_tm.tm_year + 1900;
    int current_month = now_tm.tm_mon + 1;
    int current_day = now_tm.tm_mday;

    Age age;
    age.years = current_year - year;
    age.months = current_month - month;
    age.days = current_day - day;

    // ���������� ������ ��� ������������� ���� � �������
    if (age.days < 0) {
        age.months--; // ��������� �����, ���� ������� ���� ������ ��� ��������
        // ��������� ���������� ���� � ���������� ������
        if (current_month == 1) {
            age.days += 31; // ������
        }
        else if (current_month == 3) {
            age.days += (isLeapYear(current_year) ? 29 : 28); // �������
        }
        else {
            age.days += (current_month % 2 == 0) ? 30 : 31; // ����, ���, ����, ������, �������, �������
        }
    }

    if (age.months < 0) {
        age.years--; // ��������� ���, ���� ������� ����� ������ ������ ��������
        age.months += 12; // ��������� 12 �������
    }

    return age;
}

void clearFiles(const char* reqPath, const char* ansPath) {
    // ������� ���������� ������
    ofstream reqFile(reqPath, ios::trunc); // ��������� ���� ��� �������
    reqFile.close();

    ofstream ansFile(ansPath, ios::trunc); // ��������� ���� ��� �������
    ansFile.close();
}

void clearFile(const char* Path) {
    // ������� ���������� ������
    ofstream reqFile(Path, ios::trunc); // ��������� ���� ��� �������
    reqFile.close();
}

int main() {
    using namespace std::chrono_literals;

    const char* reqPath = "Z:\\Infile.txt";
    const char* ansPath = "Z:\\Outfile.txt";

    cout << "Server is working. Press Esc to stop." << endl;

    streampos last_size = 0;

    while (true) {
        this_thread::sleep_for(100ms);

        // ���������, ���� �� ������ ������� Esc
        if (_kbhit() && _getch() == 27) { // 27 - ��� ������� Esc
            cout << "Server is stopping..." << endl;

            // ������� ����� ����� �����������
            clearFiles(reqPath, ansPath);
            cout << "Files cleared." << endl;
            break; // ����� �� �����
        }

        ifstream fR(reqPath, ios::binary | ios::ate); // ios::ate ����� ���������� ��������� � ����� �����
        if (!fR) {
            cerr << "Error opening req" << endl;
            return 1;
        }

        streampos current_size = fR.tellg();

        // ���������, ��������� �� ������ �����
        if (current_size > last_size) {
            clearFile(ansPath);
            fR.seekg(last_size, ios::beg);
            fR.read(reinterpret_cast<char*>(&B), sizeof(B));
            last_size = fR.tellg();
            fR.close();

            Age age = calculateAge(B.day, B.month, B.year);

            cout << "Received request - Name: " << B.name << ", Date of Birth: "
                << B.day << "-" << B.month << "-" << B.year << endl;

            ofstream fA(ansPath, ios::binary | ios::app);
            if (!fA) {
                cerr << "Error opening ans.txt" << endl;
                return 1;
            }
            fA.write(reinterpret_cast<char*>(&age), sizeof(age)); // ��������� ������ ���� ��� ���������
            fA.close();
        }
    }

    return 0;
}