#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>

#define BUFFER_SIZE 512

void GenerateData() {
    HANDLE hPipeGenerate;
    DWORD dwWritten;
    int dataCount = 10;
    std::vector<int> data(dataCount);

    std::cout << "Generate process: Before create generate pipe" << std::endl;
    hPipeGenerate = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\GenerateDataPipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        BUFFER_SIZE,
        BUFFER_SIZE,
        0,
        NULL);

    if (hPipeGenerate == INVALID_HANDLE_VALUE) {
        std::cerr << "Generate process: CreateNamedPipe generate failed: " << GetLastError() << std::endl;
        return;
    }
    std::cout << "Generate process: After create generate pipe" << std::endl;

    // Ожидание подключения клиента
    std::cout << "Generate process: Waiting for client to connect generate..." << std::endl;
    if (!ConnectNamedPipe(hPipeGenerate, NULL)) {
        std::cerr << "Generate process: ConnectNamedPipe generate failed: " << GetLastError() << std::endl;
        CloseHandle(hPipeGenerate);
        return;
    }
    std::cout << "Generate process: Client connected to generate." << std::endl;

    // Генерация данных
    std::cout << "Generate process: Generating data..." << std::endl;
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (int& num : data) {
        num = std::rand() % 100;
    }
    std::cout << "Generate process: Data has been generated" << std::endl;
    std::cout << "Data: ";
    for (int num : data) {
        std::cout << num << " ";
    }
    std::cout << std::endl;



    if (!WriteFile(hPipeGenerate, data.data(), data.size() * sizeof(int), &dwWritten, NULL)) {
        std::cerr << "Generate process: WriteFile failed: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Generate process: Data has been sent." << std::endl;
    }

    CloseHandle(hPipeGenerate);
}

//int main() {
//    GenerateData();
//    return 0;
//}
