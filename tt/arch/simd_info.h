#pragma once

#include <array>
#include <iostream>

#include "simd.h"

namespace tt
{
    namespace arch
    {
        enum SIMD
        {
            NONE = 0,
            SSE = 1,
            SSE2,
            SSE3,
            AVX,
            AVX2,
            FMA,
            AVX512F,
            AVX512BW,
            AVX512DQ,
            AVX512VL,
            AVX512VPOPCNTDQ
        };

        struct simd_info
        {
        private:
            constexpr static bool __SIMDIsSupported(SIMD simd)
            {
                return simd::support()[(size_t)simd];
            }
            template <typename... SIMDS>
            constexpr static bool __SIMDIsSupported(SIMD simd, SIMDS... target)
            {
                return simd::support()[(size_t)simd] && __SIMDIsSupported(target...);
            }

        public:
            constexpr static const char *SIMDNewest = simd::arch_newest_availeble();
            constexpr static const size_t SIMDFlag = simd::version_flag();
            constexpr static const size_t SIMDSupportedCount = simd::arch_supported_count();

            constexpr static std::array<const char *, SIMDSupportedCount> SIMDSupportedList()
            {
                std::array<const char *, SIMDSupportedCount> res{nullptr};
                int p = 0;
                for (int i = 0; i < 7; i++)
                    if (simd::support()[i])
                        res[p++] = simd::names()[i];
                return res;
            }

            constexpr static SIMD Newest()
            {
                for(int i = simd::lib_supported_count - 1; i > -1; i++) {
                    return (SIMD)i;
                }
                return SIMD::NONE;
            }

            template <typename... SIMDS>
            constexpr static bool SIMDIsSupported(SIMDS... target)
            {
                return __SIMDIsSupported(target...);
            }
        };

    } // namespace arch
} // namespace tt