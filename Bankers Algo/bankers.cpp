/*
    CECS 326 Project 3: Banker's Algorithm
    @authors: Nolan Nguyen, Jordan Havelaar
    @description: This program creates a system for Banker's Algorithm.
    @version 1.0.0
    @date October 27, 2024

*/


#include <iostream>
#include <vector>

class BankersAlgorithm { //algortihm class for easy management
public:
    BankersAlgorithm(std::vector<int> available, std::vector<std::vector<int>> max_resources, std::vector<std::vector<int>> allocated)
        : available(available), max_resources(max_resources), allocated(allocated) {
        num_processes = max_resources.size();
        num_resources = available.size();
        need.resize(num_processes, std::vector<int>(num_resources));
        calculate_need();
    }

    void calculate_need() {
        for (int i = 0; i < num_processes; ++i) {
            for (int j = 0; j < num_resources; ++j) {
                need[i][j] = max_resources[i][j] - allocated[i][j];
            }
        }
    }

    bool is_safe(std::vector<int>& safe_sequence) {
        std::vector<int> work = available;
        std::vector<bool> finish(num_processes, false);
        safe_sequence.clear();

        while (safe_sequence.size() < num_processes) {
            bool found = false;
            for (int p = 0; p < num_processes; ++p) {
                if (!finish[p] && can_finish(p, work)) {
                    for (int j = 0; j < num_resources; ++j) {
                        work[j] += allocated[p][j];
                    }
                    finish[p] = true;
                    safe_sequence.push_back(p);
                    found = true;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }

    bool can_finish(int process, const std::vector<int>& work) {
        for (int j = 0; j < num_resources; ++j) {
            if (need[process][j] > work[j]) {
                return false;
            }
        }
        return true;
    }

    std::string request_resources(int process_number, const std::vector<int>& request) {
        for (int j = 0; j < num_resources; ++j) {
            if (request[j] > need[process_number][j]) {
                return "Error: Process has exceeded its maximum claim.";
            }
        }
        for (int j = 0; j < num_resources; ++j) {
            if (request[j] > available[j]) {
                return "Error: Not enough resources available.";
            }
        }

        // Temporarily allocate resources
        for (int j = 0; j < num_resources; ++j) {
            available[j] -= request[j];
            allocated[process_number][j] += request[j];
            need[process_number][j] -= request[j];
        }

        // Check system's state
        std::vector<int> safe_sequence;
        if (!is_safe(safe_sequence)) {
            for (int j = 0; j < num_resources; ++j) {
                available[j] += request[j];
                allocated[process_number][j] -= request[j];
                need[process_number][j] += request[j];
            }
            return "Error: System would enter an unsafe state."; // do not allocate
        }

        return "System is in a safe state.\nSafe Sequence: " + vector_to_string(safe_sequence) +
               "\nResources allocated to process " + std::to_string(process_number) + ".";
    }

    std::string vector_to_string(const std::vector<int>& vec) {
        std::string result = "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            result += std::to_string(vec[i]);
            if (i < vec.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }

private:
    std::vector<int> available;
    std::vector<std::vector<int>> max_resources;
    std::vector<std::vector<int>> allocated; //vectors in the algorithm
    std::vector<std::vector<int>> need;
    int num_processes;
    int num_resources;
};

int main() {
    std::vector<int> available = {3, 3, 2};
    std::vector<std::vector<int>> max_resources = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    std::vector<std::vector<int>> allocated = {
        {0, 1, 0},
        {1, 0, 0}, //original 2,0,0
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    BankersAlgorithm banker(available, max_resources, allocated);

    // Running initial safety test
    std::vector<int> safe_sequence;
    if (banker.is_safe(safe_sequence)) {
        std::cout << "Test case 1: Run safe test:\nSystem is in a safe state." << std::endl;
        std::cout << "Safe Sequence: " << banker.vector_to_string(safe_sequence) << std::endl;
    } else {
        std::cout << "Test case 1: System is in an unsafe state." << std::endl;
    }

    // Test case 2: Process 1 requests resources [1, 0, 2]
    std::vector<int> request1 = {1, 0, 1};
    std::cout << "\nTest case 2:\n" << banker.request_resources(1, request1) << std::endl;

    // Test case 3: Process 4 requests resources [3, 3, 1] : not safe [2,1,0] : safe
    std::vector<int> request2 = {2, 1, 0};
    std::cout << "\nTest case 3:\n" << banker.request_resources(4, request2) << std::endl;

    return 0;
}
