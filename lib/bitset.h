#pragma once

#include <initializer_list>

#include "avxoperation.h"
#include "operation.h"

#include "tt/dout/doutdef.h"

namespace tt
{
    namespace structures
    {
        namespace bitset_opt
        {
            template <size_t Size,
                      typename _Operation>
            class bitset
            {
            private:
                using Base_ = _Operation::Base;
                using BlockType_ = Base_::BlockType;
                constexpr static const size_t RSize_ = (Size - 1 + Base_::BitSize) / Base_::BitSize;
                
                std::array<BlockType_, RSize_> Data_;

            public:
                constexpr static const size_t SizeBytes = RSize_ * sizeof(BlockType_);
                constexpr static const size_t SizeBits = SizeBytes * 8;

                constexpr TT_ALWAYS_INLINE bitset() noexcept = default;

                constexpr TT_ALWAYS_INLINE uint8_t& GetByte(size_t index) noexcept {
                    return (reinterpret_cast<uint8_t*>(&Data_))[index];
                }

                constexpr TT_ALWAYS_INLINE const uint8_t& GetByte(size_t index) const noexcept {
                    return (reinterpret_cast<const uint8_t*>(&Data_))[index];
                }

                constexpr TT_ALWAYS_INLINE bool TestBit(size_t index) const noexcept {
                    return (GetByte(index >> 3) & (0x1 << (index & 0x7))) != 0;
                }

                constexpr TT_ALWAYS_INLINE void SwapBit(size_t index) noexcept {
                    GetByte(index >> 3) ^= 0x1 << (index & 0x7);
                }

                template<size_t _Size, typename _Op, bool __M_R = bitset<_Size, _Op>::RSize_ == RSize_>
                    requires __M_R
                void And(const bitset<_Size, _Op>& b) noexcept {
                    _Operation op;
                    for(size_t i = 0; i < RSize_; i++)
                        op.And(Data_[i], b.Data_[i]);
                }

                template<size_t _Size, typename _Op, bool __M_R = bitset<_Size, _Op>::RSize_ == RSize_>
                    requires __M_R
                void Or(const bitset<_Size, _Op>& b) noexcept {
                    _Operation op;
                    for(size_t i = 0; i < RSize_; i++)
                        op.Or(Data_[i], b.Data_[i]);
                }

                template<size_t _Size, typename _Op, bool __M_R = bitset<_Size, _Op>::RSize_ == RSize_>
                    requires __M_R
                void Xor(const bitset<_Size, _Op>& b) const noexcept {
                    _Operation op;
                    for(size_t i = 0; i < RSize_; i++)
                        op.Xor(Data_[i], b.Data_[i]);
                }

                int FindNext(int pos) const noexcept {
                    DBLOCK("FindNext");
                    DOUT << "Pos: " << pos << DEND;
                    _Operation op;

                    constexpr const int block_bits = sizeof(BlockType_) * 8;
                    int block = pos / block_bits;
                    int np = op.FindNext(Data_[block], pos % block_bits);

                    while(block < RSize_ && np == block_bits)
                    {
                        np = op.FindNext(Data_[block], 0);
                        block++;
                    }
                    DOUT << "Block: " << block << DEND;
                    DOUT << "Np: " << np;
                    DENDBLOCK;
                    return np + block * block_bits;
                }
            };
        }
    }
}