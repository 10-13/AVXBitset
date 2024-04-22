#pragma once

#include <array>
#include <cstdint>

namespace tt
{
    namespace arch
    {
        struct simd
        {
        public:
#ifdef __AVX512VPOPCNTDQ__
            constexpr static const bool AVX512VPOPCNTDQ = true;
#else
            constexpr static const bool AVX512VPOPCNTDQ = false;
#endif
#ifdef __AVX512VL__
            constexpr static const bool AVX512VL = true;
#else
            constexpr static const bool AVX512VL = false;
#endif
#ifdef __AVX512DQ__
            constexpr static const bool AVX512DQ = true;
#else
            constexpr static const bool AVX512DQ = false;
#endif
#ifdef __AVX512BW__
            constexpr static const bool AVX512BW = true;
#else
            constexpr static const bool AVX512BW = false;
#endif
#ifdef __AVX512F__
            constexpr static const bool AVX512F = true;
#else
            constexpr static const bool AVX512F = false;
#endif
#ifdef __FMA__
            constexpr static const bool FMA = true;
#else
            constexpr static const bool FMA = false;
#endif
#ifdef __AVX2__
            constexpr static const bool AVX2 = true;
#else
            constexpr static const bool AVX2 = false;
#endif
#ifdef __AVX__
            constexpr static const bool AVX = true;
#else
            constexpr static const bool AVX = false;
#endif
#ifdef __SSE3__
            constexpr static const bool SSE3 = true;
#else
            constexpr static const bool SSE3 = false;
#endif
#ifdef __SSE2__
            constexpr static const bool SSE2 = true;
#else
            constexpr static const bool SSE2 = false;
#endif
#ifdef __SSE__
            constexpr static const bool SSE = true;
#else
            constexpr static const bool SSE = false;
#endif
            constexpr static const size_t lib_supported_count = 11;

            constexpr static const std::array<const char *, lib_supported_count> names()
            {
                return {"SSE", "SSE2", "SSE3", "AVX", "AVX2", "FMA", "AVX512F", "AVX512BW", "AVX512DQ", "AVX512VL", "AVX512VPOPCNTDQ"};
            }

            constexpr static const std::array<bool, lib_supported_count> support()
            {
                return {SSE, SSE2, SSE3, AVX, AVX2, FMA, AVX512F, AVX512BW, AVX512DQ, AVX512VL, AVX512VPOPCNTDQ};
            }

            constexpr static const size_t version_flag()
            {
                size_t res = 0;
                for (size_t i = 0; i < lib_supported_count; i++)
                    if (support()[i])
                        res += (0x1 << i);
                return res;
            }

            constexpr static const size_t arch_supported_count()
            {
                size_t res = 0;
                for (size_t i = 0; i < lib_supported_count; i++)
                    if (support()[i])
                        res++;
                return res;
            }

            constexpr static const char *arch_newest_availeble()
            {
                for (int i = lib_supported_count - 1; i > -1; i--)
                    if (support()[i])
                        return names()[i];
                return "NONE";
            }
        };
    } // namespace arch
} // namespace tt
