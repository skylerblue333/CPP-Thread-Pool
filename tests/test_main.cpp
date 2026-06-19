#include <cassert>
#include <string>

int process_data(const std::string& input) {
    return input.length();
}

int main() {
    assert(process_data("hello") == 5);
    assert(process_data("") == 0);
    return 0;
}
