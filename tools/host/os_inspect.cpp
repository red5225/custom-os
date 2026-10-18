#include <iostream>
#include <string_view>

namespace {
constexpr std::string_view version = "CustomOS host tools 0.1";
}

int main(int argc, char** argv) {
    if (argc == 2 && std::string_view(argv[1]) == "--self-test") {
        if (version.empty()) return 1;
        std::cout << "C++ host utility: PASS\n";
        return 0;
    }
    if (argc == 2 && std::string_view(argv[1]) == "--version") {
        std::cout << version << '\n';
        return 0;
    }
    std::cerr << "usage: customos-inspect --version | --self-test\n";
    return 2;
}
