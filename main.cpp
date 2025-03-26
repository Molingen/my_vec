#include <iostream>
#include "MyVec.h"

int main() {
    MyVec vec = {1, 2, 3, 4};
    vec.clean();
    if (vec.empty()) {
        std::cout << "asd" << std::endl;
    }
    vec.insert(0, {1, 2, 3});
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    vec.erase(0);
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    vec.push_back(11);
    vec.push_back(12);
    vec.push_back(12);
    std::cout << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    vec.resize(2);

    return 0;
}