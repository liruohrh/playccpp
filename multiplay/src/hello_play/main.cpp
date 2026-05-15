#include <print>
#include <source_location>

int main() {
    constexpr auto loc = std::source_location::current();
    std::println("{}:{} — Hello, Play!", loc.file_name(), loc.line());
    return 0;
}
