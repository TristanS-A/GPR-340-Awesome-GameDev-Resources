// add your imports here
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>

const std::string TEST_FOLDER = "\\tests\\";
uint32_t xorShift(uint32_t seed, int r1, int r2);
int main(){
  uint32_t seed, N, min, max, result;
  std::cin >> seed >> N >> min >> max;

  for (int i = 0; i < N; i++) {
    seed = xorShift(seed, min, max);
    std::cout << min + (seed % (max - min + 1)) << std::endl;
  }
}
//The purpose of this function is to take the number and xor shift it to output a pseudo-random number
    uint32_t xorShift(uint32_t seed, int r1, int r2)
{
    uint32_t x = seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return x;
}
