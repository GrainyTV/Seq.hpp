#pragma once

#define ASSERT(expr)                                      \
    do {                                                  \
        if (!(expr)) {                                    \
            throw std::runtime_error("assert triggered"); \
        }                                                 \
    } while(0)
