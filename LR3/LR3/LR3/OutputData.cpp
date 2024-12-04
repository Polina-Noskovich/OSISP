#include <windows.h>
#include <iostream>
#include <vector>

void OutputData() {
    HANDLE hPipe;
    DWORD dwRead;
    std::vector<int> data(10);

    // Подключение к каналу
    while (true) {
        hPipe = CreateFile(
            TEXT("\\\\.\\pipe\\SortDataPipe"),
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "Output process: CreateFile failed: " << GetLastError() << std::endl;
        }
        else {
            std::cout << "Output process: Connected to the sort pipe." << std::endl;
            break;
        }
        Sleep(100);
    }
    // Чтение данных
    if (!ReadFile(hPipe, data.data(), data.size() * sizeof(int), &dwRead, NULL)) {
        std::cerr << "Output process: ReadFile sort failed: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        return;
    }
    // Вывод данных
    std::cout << "Output process: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    CloseHandle(hPipe);
}

//int main() {
//    OutputData();
//    return 0;
//}
