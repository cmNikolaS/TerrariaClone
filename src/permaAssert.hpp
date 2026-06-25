#ifndef PERMA_ASSERT_HPP
#define PERMA_ASSERT_HPP

#include <string_view>

namespace core {

    // Updated to accept a custom message string
    void perma_assert_fail(std::string_view expression, std::string_view message, std::string_view file, int line, std::string_view function);

} // namespace core

// Standard permanent assert (passes an empty message)
#define PERMA_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            ::core::perma_assert_fail(#condition, "", __FILE__, __LINE__, __func__); \
        } \
    } while (false)

// Permanent assert with a custom message
#define PERMA_ASSERT_MSG(condition, message) \
    do { \
        if (!(condition)) { \
            ::core::perma_assert_fail(#condition, message, __FILE__, __LINE__, __func__); \
        } \
    } while (false)

#endif // PERMA_ASSERT_HPP