cc_binary(
    name = "bitset_bench",
    srcs = [ "bench_main.cpp" ],
    deps = [
        "//lib:bitset",
        "//utils/catch2:catch2",
    ],
    copts = [
        "-std=c++20",
        "-march=native",
        "-O3"
    ]
)