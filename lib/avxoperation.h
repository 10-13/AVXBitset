#pragma once

#include <immintrin.h>

#include "operationbase.h"
#include "tt/arch/simd_info.h"
#include "tt/def/macro.h"
#include "tt/arch/buildin.h"

#include "tt/dout/doutdef.h"

#include "iostream"

namespace tt
{
    namespace structures
    {
        namespace bitset_opt
        {
            template<template<size_t Size> typename _Base = operation_base,
                bool __M_SUPPORTED = tt::arch::simd_info::SIMDIsSupported(tt::arch::AVX)>
                requires __M_SUPPORTED
            struct avx_operation {
                private:
                __m256i tmpzero = _mm256_set1_epi64x(0);

                public:
                using Base = _Base<256>;

                constexpr TT_ALWAYS_INLINE avx_operation() {}

                static TT_ALWAYS_INLINE void And(Base::BlockType& a, const Base::BlockType& b) noexcept {
                    __m256i _a = _mm256_load_si256((__m256i*)a.data());
                    __m256i _b = _mm256_load_si256((__m256i*)b.data());
                    _a = _mm256_and_si256(_a, _b);
                    _mm256_store_si256((__m256i*)a.data(), _a);
                }
                
                static TT_ALWAYS_INLINE void Or(Base::BlockType& a, const Base::BlockType& b) noexcept {
                    __m256i _a = _mm256_load_si256((__m256i*)a.data());
                    __m256i _b = _mm256_load_si256((__m256i*)b.data());
                    _a = _mm256_or_si256(_a, _b);
                    _mm256_store_si256((__m256i*)a.data(), _a);
                }

                static TT_ALWAYS_INLINE void Xor(Base::BlockType& a, const Base::BlockType& b) noexcept {
                    __m256i _a = _mm256_load_si256((__m256i*)(&a));
                    __m256i _b = _mm256_load_si256((__m256i*)(&b));
                    _a = _mm256_xor_si256(_a, _b);
                    _mm256_store_si256((__m256i*)a.data(), _a);
                }

                constexpr TT_ALWAYS_INLINE int FindNext(const Base::BlockType& a, int pos ) const noexcept {
                    DBLOCK("FindNextOp");

                    DBLOCK("Args");
                    DOUT << "&A:  " << &a << DEND;
                    DOUT << "A:   " << a[3] << ' ' << a[2] << ' ' << a[1] << ' ' << a[0] << DEND;
                    DOUT << "Pos: " << pos;
                    DENDBLOCK;

                    __m256i _a = _mm256_load_si256((__m256i*)(&a));
                    if(_mm256_testz_si256(_a, _a) == 1) {
                        DOUT << "OUT: Zero reg";
                        DENDBLOCK;
                        return 8 * sizeof(typename Base::BlockType);
                    }
                    _a = _mm256_cmpeq_epi64(_a, tmpzero);
                    __m256d _b = _mm256_castsi256_pd(_a);
                    int block = pos / (sizeof(typename Base::BlockType) * 8);
                    unsigned msk = ~_mm256_movemask_pd(_b) & (~0x1 + 0x1 * (a[block] >> (pos % (sizeof(typename Base::BlockType) * 8)) != 0));
                    if(msk == 0) {
                        DOUT << "OUT: Zero msk";
                        DENDBLOCK;
                        return 8 * sizeof(typename Base::BlockType);
                    }
                    block += TT_COUNT_TRAILING_BITS(msk);
                    DOUT << "Block: " << block << DEND;
                    DOUT << "Pos: " << TT_COUNT_TRAILING_BITSLL(a[block]) << DEND;
                    DOUT << "OUT: Default";
                    DENDBLOCK;
                    return TT_COUNT_TRAILING_BITSLL(a[block]) + block * (sizeof(typename Base::BlockType) * 8);
                }
            };
        }
    }
}