#pragma once

#include "tt/arch/simd.h"
#include "tt/collections/alignarray.h"

namespace tt
{
    namespace structures
    {
        namespace bitset_opt
        {
            template <size_t _TargetBitSize,
                typename _StoreType = uint64_t>
            struct operation_base
            {
                constexpr static const size_t BitSize = _TargetBitSize;
                using StoreType = _StoreType;
                using BlockType = tt::collections::generic::align_array<StoreType, BitSize / sizeof(StoreType) / 8, _TargetBitSize>;
            };
        }
    }
}