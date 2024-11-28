#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <locale.h>

using namespace std;

struct Person {
    char name[25];
    int day;
    int month;
    int year;
};

void clearFiles(const char* ansPath) {
    ofstream ansFile(ansPath, ios::trunc); // ��������� ���� ��� �������
    ansFile.close();
}

int main() {
    setlocale(LC_ALL, "rus");
    const char* reqPath = "Z:\\Infile.txt";
    const char* ansPath = "Z:\\Outfile.txt";

    Person A;

    while (true) {
        cout << "������� ������ (���, ����, �����, ���) ��� 'exit' ��� ������: ";
        cin >> A.name;

        // �������� �� �����
        if (strcmp(A.name, "exit") == 0) {
            break;
        }

        cin >> A.day >> A.month >> A.year;

        // ������ ������ � ���� ��������
        ofstream f_req(reqPath, ios::app | ios::binary);
        if (!f_req) {
            cerr << "������ ��� �������� ����� ��������." << endl;
            continue;
        }
        f_req.write(reinterpret_cast<char*>(&A), sizeof(A));
        f_req.close();

        clearFiles(ansPath);

        cout << "������ ���������. �������� ������..." << endl;

        // �������� ������ �� �������
        struct Age {
            int years;
            int months;
            int days;
        } age;

        // �������� � ������ ������
        ifstream f_ans(ansPath, ios::binary);
        if (!f_ans) {
            cerr << "������ ��� �������� ����� �������." << endl;
            continue;
        }

        // ��������, ���� ����� �� ����� ��������
        while (true) {
            f_ans.seekg(0, ios::end);
            if (f_ans.tellg() > 0) {
                f_ans.seekg(0, ios::beg);
                f_ans.read(reinterpret_cast<char*>(&age), sizeof(age));
                break; // ����� �� �����, ����� ����� ��������
            }
            // ���������� �������� ��������, ����� �������� ���������� �������� ����������
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        f_ans.close();

        // ����� ���������� ��� ������������
        cout << "�������: " << age.years << " ���, " << age.months << " �������, " << age.days << " ����." << endl;
    }

    return 0;
}