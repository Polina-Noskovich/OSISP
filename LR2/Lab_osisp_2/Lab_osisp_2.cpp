#include <iostream>
#include <windows.h>
#include <vector>
#include <string>

#define BUFFER_SIZE 1024

// Функция для симуляции задержки с различными временными интервалами
void simulate_delay(int milliseconds) {
    Sleep(milliseconds); // Задержка в миллисекундах
}

// Функция для создания файла и записи данных, если он не существует
void create_file_with_data(const wchar_t* file_name, const std::string& data) {
    HANDLE file_handle = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_handle != INVALID_HANDLE_VALUE) {
        DWORD bytes_written;
        WriteFile(file_handle, data.c_str(), data.size(), &bytes_written, NULL);
        CloseHandle(file_handle);
        std::cout << "File " << file_name << " created and data written." << std::endl;
    }
    else {
        std::cerr << "Error creating file: " << file_name << std::endl;
    }
}

int main() {
    // Инициализация файлов и их данных
    std::vector<const wchar_t*> file_names = { L"file1.txt", L"file2.txt", L"file3.txt" };
    std::vector<std::string> file_data = {
        "This is the content of file 1.",
        "This is the content of file 2.",
        "This is the content of file 3."
    };

    // Создаем файлы и записываем в них данные
    for (size_t i = 0; i < file_names.size(); ++i) {
        create_file_with_data(file_names[i], file_data[i]);
    }

    std::vector<HANDLE> file_handles;
    for (const auto& file_name : file_names) {
        HANDLE file_handle = CreateFile(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        if (file_handle == INVALID_HANDLE_VALUE) {
            std::cerr << "Error opening file: " << file_name << std::endl;
            return 1;
        }
        file_handles.push_back(file_handle);
    }

    char buffer[BUFFER_SIZE];
    OVERLAPPED overlapped[3]; // Массив для структур OVERLAPPED
    DWORD bytes_read;

    // Инициализация структур OVERLAPPED
    for (size_t i = 0; i < file_handles.size(); ++i) {
        ZeroMemory(&overlapped[i], sizeof(OVERLAPPED));
    }

    bool all_files_read = false;

    // Основной цикл чтения данных из файлов
    while (!all_files_read) {
        all_files_read = true; // Предполагаем, что все файлы прочитаны

        for (size_t i = 0; i < file_handles.size(); ++i) {
            // Моделируем задержку перед чтением файла
            simulate_delay(50 + i * 50); // Задержка увеличивается с каждым файлом

            // Асинхронное чтение из файла
            BOOL result = ReadFile(file_handles[i], buffer, BUFFER_SIZE - 1, &bytes_read, &overlapped[i]);

            if (result || GetLastError() == ERROR_IO_PENDING) {
                if (result && bytes_read > 0) {
                    buffer[bytes_read] = '\0'; // Завершение строки
                    std::cout << "Read from " << file_names[i] << ": " << buffer << std::endl;

                    // Моделируем задержку после успешного чтения
                    simulate_delay(100); // Задержка 100 мс после чтения
                }
                else if (GetLastError() == ERROR_IO_PENDING) {
                    // Ожидание завершения асинхронной операции
                    WaitForSingleObject(file_handles[i], INFINITE);
                    GetOverlappedResult(file_handles[i], &overlapped[i], &bytes_read, TRUE);
                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';
                        std::cout << "Asynchronous read completed from " << file_names[i] << ": " << buffer << std::endl;

                        // Моделируем задержку после завершения чтения
                        simulate_delay(100); // Задержка 100 мс после чтения
                    }
                }

                // Если прочитано 0 байт, значит файл прочитан полностью
                if (bytes_read == 0) {
                    all_files_read = false; // Прерываем цикл, если не все файлы прочитаны
                }
            }
            else {
                std::cerr << "Error reading from file " << file_names[i] << std::endl;
            }
        }

        if (!all_files_read) {
            // Моделируем задержку перед следующим циклом чтения
            simulate_delay(500); // Задержка 00 мс перед следующим циклом
        }
    }

    std::cout << "All files have been read." << std::endl;

    // Закрываем дескрипторы файлов
    for (auto& handle : file_handles) {
        CloseHandle(handle);
    }

    return 0;
}
