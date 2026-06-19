#include <iostream>
#include <string>

int process_data(const std::string& input) {
    return input.length();
}

int main() {
    std::cout << "Starting CPP-Thread-Pool service..." << std::endl;
    std::string test_data = "test payload";
    int result = process_data(test_data);
    std::cout << "Processed " << result << " bytes successfully." << std::endl;
    return 0;
}
