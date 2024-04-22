#include <iostream>

#include "tt/arch/simd_info.h"

#include "lib/avxoperation.h"
#include "lib/operation.h"
#include "lib/bitset.h"

#include "tt/dout/dout.h"

using AVX_OP = tt::structures::bitset_opt::avx_operation<>;
using CPP_OP = tt::structures::bitset_opt::cpp_operation<>;
using bitset512 = tt::structures::bitset_opt::bitset<512, AVX_OP>;

using namespace tt::util::debug;

template<typename T, size_t Size>
void RandomizeBitset(tt::structures::bitset_opt::bitset<Size, T>& bs) {
    for(size_t i = 0; i < bs.SizeBytes; i++)
        bs.GetByte(i) = rand();
}

int main() {
    dout::init(&std::cout) << "Test";
    dout::instance() << block{"main"};
    bitset512 bs1{};
    bs1.GetByte(0) = 0x88;
    dout::instance() << bs1.FindNext(bs1.FindNext(0) + 1);
    dout::instance() << end_block{};
    return 0;
}