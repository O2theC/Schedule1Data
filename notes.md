all scripts are to be ran with PYTHONPATH=. assuming cwd is the root of this repo

std::format("{:x}",  n);   // ff
std::format("{:X}",  n);   // FF
std::format("{:#x}", n);   // 0xff
std::format("{:o}",  n);   // 377
std::format("{:b}",  n);   // 11111111  (binary, no printf equivalent)
std::format("{:#b}", n);   // 0b11111111

#include <print>   // C++23

std::print("Value: {}\n", 42);  // C++23, prints directly

// std::format
std::format("{:10}",  42);    //         42
std::format("{:<10}", 42);    // 42
std::format("{:>10}", 42);    //         42
std::format("{:^10}", 42);    //     42      (centered)
std::format("{:0>10}", 42);   // 0000000042  (pad with 0s)
std::format("{:*^10}", 42);   // ****42****  (pad with any character)

#include <format>
#include <locale>

// std::format with locale (C++20)
std::cout << std::format(std::locale(""), "{:L}\n", 1000000);
// 1,000,000  (uses system locale separator)

uint64_t bits = 0xDEADBEEFCAFEBABE;

std::format("{:016x}",  bits);   // deadbeefcafebabe   (16 hex chars, zero padded)
std::format("{:#018x}", bits);   // 0xdeadbeefcafebabe (16 + 2 for 0x prefix = 18)