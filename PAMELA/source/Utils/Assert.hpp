#pragma once
#ifndef ASSERT_HPP_INCLUDED
#define ASSERT_HPP_INCLUDED

#include "Logger.hpp"

#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            LOGERROR(message); \
                } \
            } while (false);
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

#endif //ASSERT_HPP_INCLUDED
