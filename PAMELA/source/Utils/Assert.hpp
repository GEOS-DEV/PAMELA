#pragma once

#include "Logger.hpp"


namespace PAMELA {
#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            LOGERROR(message); \
                } \
            } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif
} // namespace
