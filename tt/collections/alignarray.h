#pragma once

#include <cstdint>
#include <initializer_list>
#include <array>

namespace tt {
    namespace collections {
        namespace generic {
            template<typename T, size_t size, size_t _align = alignof(T) * 8ULL>
                requires (_align / 8 >= alignof(T) && _align / 8 <= size * sizeof(T))
            struct alignas(_align / 8) align_array : public std::array<T, size> {};
        }
    }
}