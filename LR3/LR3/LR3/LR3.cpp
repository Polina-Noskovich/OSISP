
#include <windows.h>
#include <iostream>

int main() {
    // Создаем три процесса для каждого этапа обработки
    STARTUPINFO si[3];  // Массив структур STARTUPINFO для хранения информации о запуске процессов
    PROCESS_INFORMATION pi[3];  // Массив структур PROCESS_INFORMATION для хранения информации о созданных процессах
    std::wstring commands[3] = {  // Массив строк с командами для запуска процессов
        L"GenerateData.exe",  // Первая команда для запуска процесса: генерирование данных
        L"SortData.exe",      // Вторая команда для запуска процесса: сортировка данных
        L"OutputData.exe"     // Третья команда для запуска процесса: вывод данных
    };

    // Цикл по трем процессам
    for (int i = 0; i < 3; ++i) {
        // Обнуляем память в структурах STARTUPINFO и PROCESS_INFORMATION для безопасного использования
        ZeroMemory(&si[i], sizeof(si[i]));  // Обнуляем структуру STARTUPINFO
        si[i].cb = sizeof(si[i]);  // Устанавливаем размер структуры STARTUPINFO
        ZeroMemory(&pi[i], sizeof(pi[i]));  // Обнуляем структуру PROCESS_INFORMATION

        // Запуск процессов с помощью CreateProcess
        if (!CreateProcess(NULL, const_cast<LPWSTR>(commands[i].c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si[i], &pi[i])) {
            // Если запуск процесса не удался, выводим ошибку и выходим
            std::cerr << "CreateProcess failed: " << GetLastError() << " " << i << std::endl;
            return 1;  // Завершаем программу с кодом ошибки
        }
    }

    // После того как все процессы запущены, закрываем дескрипторы процессов и потоков
    for (int i = 0; i < 3; ++i) {
        CloseHandle(pi[i].hProcess);  // Закрываем дескриптор процесса
        CloseHandle(pi[i].hThread);   // Закрываем дескриптор потока
    }

    return 0;  // Завершаем программу успешно
}

