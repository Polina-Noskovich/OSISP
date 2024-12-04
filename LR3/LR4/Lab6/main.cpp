#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>

const int NUM_PHILOSOPHERS = 5;
std::mutex forks[NUM_PHILOSOPHERS];

void philosopher(int id) {
    std::mt19937 rng(id);
    std::uniform_int_distribution<int> dist(2000, 3000);

    int leftFork = id;
    int rightFork = (id + 1) % NUM_PHILOSOPHERS;

    while (true) {
        // Философ думает
        std::cout << "Philosopher " << id << " is thinking.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));

        // Захват вилок
        if (id % 2 == 0) {
            forks[leftFork].lock();
            forks[rightFork].lock();
        }
        else {
            forks[rightFork].lock();
            forks[leftFork].lock();
        }

        // Философ ест
        std::cout << "Philosopher " << id << " is eating.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));

        // Освобождение вилок
        forks[leftFork].unlock();
        forks[rightFork].unlock();
    }
}

int main() {
    std::vector<std::thread> philosophers;

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    for (auto& phil : philosophers) {
        phil.join();
    }

    return 0;
}