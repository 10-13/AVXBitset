#include <array>
#include <tuple>
#include <bitset>
#include <catch2/catch_all.hpp>

#include "lib/bitset.h"

using namespace tt::structures;

/*
template<typename T, size_t Size>
void RandomizeBitset(tt::structures::bitset_opt::bitset<Size, T>& bs) {
  for(size_t i = 0; i < bs.SizeBytes; i++)
    bs.GetByte(i) = rand();
}

template<size_t Size, size_t SetSize = 1024>
using DataSet = std::array<std::tuple<CPPBitset<SetSize>, AVXBitset<SetSize>>, 1024>;

template<size_t Size, size_t SetSize = 1024>
DataSet<Size, SetSize> RandomData() {
  DataSet<Size, SetSize> data;
  for(size_t i = 0; i < Size; i++) {
    RandomizeBitset(std::get<0>(data[i]));
    RandomizeBitset(std::get<1>(data[i]));
  }
  return data;
}
*/

template<size_t Size, size_t SetSize = 1024>
using DataSet = std::array<std::tuple<CPPBitset<SetSize>, AVXBitset<SetSize>, std::bitset<SetSize>>, 1024>;

template<size_t SetSize>
void RandomizeStdBitset(std::bitset<SetSize>& bs) {
  for (size_t i = 0; i < SetSize; i++) {
    bs[i] = 1 & rand();
  }
}

template<typename T, size_t SetSize>
void Load(const std::bitset<SetSize>& bs, T& to) {
  for (size_t i = 0; i < SetSize; i += 8) {
    uint8_t a = 0;
    for (int j = i; j < 8; j++) {
      a <<= 1;
      a += bs[j];
    }
  }
}

template<size_t Size, size_t SetSize = 1024>
DataSet<Size, SetSize> RandomData() {
  DataSet<Size, SetSize> data;
  for(size_t i = 0; i < Size; i++) {
    RandomizeStdBitset<SetSize>(std::get<2>(data[i]));
    Load<CPPBitset<SetSize>, SetSize>(std::get<2>(data[i]), std::get<0>(data[i]));
    Load<AVXBitset<SetSize>, SetSize>(std::get<2>(data[i]), std::get<1>(data[i]));
  }
  return data;
}

TEST_CASE("Bitset And") {
  BENCHMARK_ADVANCED("AVX 1024")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30>();
    meter.measure([&data](int i) { std::get<1>(data[i % 30]).And(std::get<1>(data[i % 30])); return std::get<1>(data[i % 30]).TestBit(15); });
  };
  BENCHMARK_ADVANCED("CPP 1024")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30>();
    meter.measure([&data](int i) { std::get<0>(data[i % 30]).And(std::get<0>(data[(i + 13) % 30])); return std::get<0>(data[i % 30]).TestBit(15); });
  };
  BENCHMARK_ADVANCED("std::bitset<1024>")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30>();
    meter.measure([&data](int i) { std::get<2>(data[i % 30]) &= std::get<2>(data[(i + 13) % 30]); return std::get<2>(data[i % 30])[15]; });
  };

  BENCHMARK_ADVANCED("AVX 16384")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30, 16384>();
    meter.measure([&data](int i) { std::get<1>(data[i % 30]).And(std::get<1>(data[i % 30])); return std::get<1>(data[i % 30]).TestBit(15); });
  };
  BENCHMARK_ADVANCED("CPP 16384")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30, 16384>();
    meter.measure([&data](int i) { std::get<0>(data[i % 30]).And(std::get<0>(data[(i + 13) % 30])); return std::get<0>(data[i % 30]).TestBit(15); });
  };
  BENCHMARK_ADVANCED("std::bitset<16384>")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30, 16384>();
    meter.measure([&data](int i) { std::get<2>(data[i % 30]) &= std::get<2>(data[(i + 13) % 30]); return std::get<2>(data[i % 30])[15]; });
  };
}


TEST_CASE("Bitset Find Next") {
  BENCHMARK_ADVANCED("AVX 1024")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30>();
    meter.measure([&data](int i) { return std::get<1>(data[i % 30]).FindNext(i % 1024); });
  };
  BENCHMARK_ADVANCED("CPP 1024")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30>();
    meter.measure([&data](int i) { return std::get<0>(data[i % 30]).FindNext(i % 1024); });
  };

  BENCHMARK_ADVANCED("AVX 16384")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30, 16384>();
    meter.measure([&data](int i) { return std::get<1>(data[i % 30]).FindNext(i % 1024); });
  };
  BENCHMARK_ADVANCED("CPP 16384")(Catch::Benchmark::Chronometer meter) {
    auto data = RandomData<30, 16384>();
    meter.measure([&data](int i) { return std::get<0>(data[i % 30]).FindNext(i % 1024); });
  };
}