#include <iostream>

#include "tt/arch/simd_info.h"

#include "lib/avxoperation.h"
#include "lib/operation.h"
#include "lib/bitset.h"

#include "tt/dout/dout.h"

using namespace tt::structures;

template<typename T, size_t Size>
void RandomizeBitset(tt::structures::bitset_opt::bitset<Size, T>& bs) {
    for(size_t i = 0; i < bs.SizeBytes; i++)
        bs.GetByte(i) = rand();
}

int main() {
    std::cout << std::hex;
    //tt::util::debug::dout::init(&std::cout);
    CPPBitset<512> a;
    a.Xor(a);
    AVXBitset<512> b;
    b.Xor(b);
    for (int i = 0; i <= 1e8; i++) {
        //std::cout << "RND" << std::endl;
        RandomizeBitset(a);
        for (size_t i = 0; i < a.SizeBytes; i++) {
            b.GetByte(i) = a.GetByte(i);
        }
        //std::cout << "RND1" << std::endl;

        int pos = rand() % 512;

        //std::cout << b.GetByte(0) << "\n";
        if (a.FindNext(pos) != b.FindNext(pos)) {
            std::cout << "Error\n";
            return 0;
        }
    }

    std::cout << "Ok\n";

    return 0;
}