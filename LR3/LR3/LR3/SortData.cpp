#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define BUFFER_SIZE 512

void SortData() {
    HANDLE hPipeRead, hPipeWrite;
    DWORD dwRead, dwWritten;
    std::vector<int> data(10);

    while (true) {
        hPipeRead = CreateFile(
            TEXT("\\\\.\\pipe\\GenerateDataPipe"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (hPipeRead != INVALID_HANDLE_VALUE) {
            std::cout << "Sorting process: Connected to the generate pipe." << std::endl;
            break; // Успешное подключение
        }
        else {
            std::cout << "Sorting process: Failed connect to the generate pipe." << GetLastError() << std::endl;
        }

        Sleep(100);
    }
    // Чтение данных
    if (!ReadFile(hPipeRead, data.data(), data.size() * sizeof(int), &dwRead, NULL)) {
        std::cerr << "Sorting process: ReadFile generate failed: " << GetLastError() << std::endl;
        CloseHandle(hPipeRead);
        return;
    }
    CloseHandle(hPipeRead);
    // Сортировка данных
    std::sort(data.begin(), data.end());
    std::cout << "Sorting process: Data has been sorted." << std::endl;
    hPipeWrite = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\SortDataPipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        BUFFER_SIZE,
        BUFFER_SIZE,
        0,
        NULL);
    if (hPipeWrite == INVALID_HANDLE_VALUE) {
        std::cerr << "S: CreateNamedPipe sort failed: " << GetLastError() << std::endl;
        return;
    }
    std::cout << "Sorting process: Waiting for client to connect sort..." << std::endl;
    if (!ConnectNamedPipe(hPipeWrite, NULL)) {
        std::cerr << "Sorting process: ConnectNamedPipe sort failed: " << GetLastError() << std::endl;
        CloseHandle(hPipeWrite);
        return;
    }
    std::cout << "Sorting process: Client connected to sort." << std::endl;
    // Отправка отсортированных данных
    if (!WriteFile(hPipeWrite, data.data(), data.size() * sizeof(int), &dwWritten, NULL)) {
        std::cerr << "Sorting process: WriteFile sort failed: " << GetLastError() << std::endl;
    }

    CloseHandle(hPipeWrite);
}
//
//int main() {
//    SortData();
//    return 0;
//}
