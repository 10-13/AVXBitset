#pragma once

#include "operationbase.h"
#include "tt/arch/simd_info.h"
#include "tt/def/macro.h"
#include "tt/arch/buildin.h"

#include "tt/dout/doutdef.h"

namespace tt
{
    namespace structures
    {
        namespace bitset_opt
        {
            template<template<size_t Size> typename _Base = operation_base>
            struct cpp_operation {
                using Base = _Base<64>;

                constexpr TT_ALWAYS_INLINE cpp_operation() {}

                static TT_ALWAYS_INLINE void And(Base::BlockType& a, const Base::BlockType& b) noexcept {
                    a[0] &= b[0];
                }
                
                static TT_ALWAYS_INLINE void Or(Base::BlockType& a, const Base::BlockType& b) noexcept {
                    a[0] |= b[0];
                }

                static TT_ALWAYS_INLINE void Xor(Base::BlockType& a, const Base::BlockType& b) noexcept {
                    a[0] ^= b[0];
                }

                constexpr TT_ALWAYS_INLINE int FindNext(const Base::BlockType& a, int pos ) const noexcept {
                    if(a[0] >> pos == 0) {
                        DOUT << "BLOCKEND" << DEND;
                        return 8 * sizeof(typename Base::BlockType);
                    }
                    DOUT << "Value: " << (a[0] >> pos) << DEND;
                    DOUT << "Pos: " << pos << DEND;
                    return TT_COUNT_TRAILING_BITSLL(a[0] >> pos) + pos;
                }
            };
        }
    }
}