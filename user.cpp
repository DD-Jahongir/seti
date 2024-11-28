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
    ofstream ansFile(ansPath, ios::trunc); // Открываем файл для очистки
    ansFile.close();
}

int main() {
    setlocale(LC_ALL, "rus");
    const char* reqPath = "Z:\\Infile.txt";
    const char* ansPath = "Z:\\Outfile.txt";

    Person A;

    while (true) {
        cout << "Введите данные (Имя, День, Месяц, Год) или 'exit' для выхода: ";
        cin >> A.name;

        // Проверка на выход
        if (strcmp(A.name, "exit") == 0) {
            break;
        }

        cin >> A.day >> A.month >> A.year;

        // Запись данных в файл запросов
        ofstream f_req(reqPath, ios::app | ios::binary);
        if (!f_req) {
            cerr << "Ошибка при открытии файла запросов." << endl;
            continue;
        }
        f_req.write(reinterpret_cast<char*>(&A), sizeof(A));
        f_req.close();

        clearFiles(ansPath);

        cout << "Запрос отправлен. Ожидание ответа..." << endl;

        // Ожидание ответа от сервера
        struct Age {
            int years;
            int months;
            int days;
        } age;

        // Ожидание и чтение ответа
        ifstream f_ans(ansPath, ios::binary);
        if (!f_ans) {
            cerr << "Ошибка при открытии файла ответов." << endl;
            continue;
        }

        // Ожидание, пока ответ не будет доступен
        while (true) {
            f_ans.seekg(0, ios::end);
            if (f_ans.tellg() > 0) {
                f_ans.seekg(0, ios::beg);
                f_ans.read(reinterpret_cast<char*>(&age), sizeof(age));
                break; // Выход из цикла, когда ответ прочитан
            }
            // Используем короткое ожидание, чтобы избежать чрезмерной загрузки процессора
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        f_ans.close();

        // Вывод результата для пользователя
        cout << "Возраст: " << age.years << " лет, " << age.months << " месяцев, " << age.days << " дней." << endl;
    }

    return 0;
}