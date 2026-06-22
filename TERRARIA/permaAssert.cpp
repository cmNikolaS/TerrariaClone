#include "permaAssert.hpp"
#include <iostream>
#include <cstdlib>

namespace core {

    void perma_assert_fail(std::string_view expression, std::string_view message, std::string_view file, int line, std::string_view function) {
        std::cerr << "[PERMA_ASSERT FAILED]\n"
            << "  Expression: " << expression << "\n";

        // Only print the message row if a custom message was actually provided
        if (!message.empty()) {
            std::cerr << "  Message:    " << message << "\n";
        }

        std::cerr << "  File:       " << file << "\n"
            << "  Line:       " << line << "\n"
            << "  Function:   " << function << "\n";

        std::abort();
    }

} // namespace core