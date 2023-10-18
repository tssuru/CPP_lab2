#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <thread>

template <class Container, class UnaryPredicate>
bool my_any_of(Container first, Container last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return true;
        }
    }
    return false;
}

bool isEven(int n) {
    return n % 2 == 0;
}

bool runAnyOf(const std::vector<int>& container, int numThreads) {
    std::vector<std::thread> threads;
    std::vector<bool> threadResults(numThreads);
    int batchSize = container.size() / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int start = i * batchSize;
        int end = (i == numThreads - 1) ? container.size() : start + batchSize;
        threads.push_back(std::thread([start, end, &container, &threadResults, i]() {
            threadResults[i] = my_any_of(container.begin() + start, container.begin() + end, isEven);
        }));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    return std::any_of(threadResults.begin(), threadResults.end(), [](bool result) { return result; });
}



int main() {
    std::vector<int> numbers(1000000, 1); // Create a vector with 1 million elements, all set to 1

    int numThreads[] = {1, 2, 4, 8,32}; // Test with different numbers of threads

    for (int numThread : numThreads) {
        auto start = std::chrono::high_resolution_clock::now();
        bool result = runAnyOf(numbers, numThread);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Number of threads: " << numThread << std::endl;
        std::cout << "Result: " << result << std::endl;
        std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
        std::cout << "--------------------------" << std::endl;
    }

    return 0;
}

//
//int main() { //Second case
//    std::ofstream outputFile("../execution_times.txt");
//
//    for (int dataSize = 0; dataSize <= 1000000; dataSize += 100000) {
//        std::vector<int> numbers;
//        for (int i = 0; i < dataSize; i++) {
//            numbers.push_back(i);
//        }
//
//        auto start = std::chrono::high_resolution_clock::now();
//        bool result = my_any_of(numbers.begin(), numbers.end(), isEven);
//        auto end = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> duration = end - start;
//
//        outputFile << dataSize << " " << duration.count() << std::endl;
//    }
//
//    outputFile.close();
//
//    return 0;
//}