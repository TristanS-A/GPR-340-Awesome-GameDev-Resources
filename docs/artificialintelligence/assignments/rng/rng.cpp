// add your imports here
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <unordered_map>
#include <vector>

//For Mersenne Twister
#define n 1 //The degree of recurrence
#define m 1 //The middle word that is above 0 and under n
#define w 32 //Number of bits per int
#define r 31 //Number of bits of the lower bitmask
#define UMASK (0xffffffffUL << r)
#define LMASK (0xffffffffUL >> (w-r))
#define a 0x9908b0dfUL //Coefficient of the rational normal form twist matrix
#define u 11 //Additional tempered bit shift/mask
#define s 7 //Tempered bit shift
#define b 0x9d2c5680UL //Tempered bitmask
#define t 15 //Tempered bit shift
#define c 0xefc60000UL //Tempered bitmask
#define l 18 //Additional tempered bit shift/mask
#define f 1812433253UL

const std::string TEST_FOLDER = "\\tests\\";

//Modified Mersenne Twister code
struct MersenneTwister {
  uint16_t state;
  std::unordered_map<uint16_t, uint16_t> stateMap;

  void init(uint16_t seed) {
    seed = f * (seed ^ (seed >> (w-2)));
    state = seed;
  }

  uint16_t randomInt() {
    uint16_t x = (state & UMASK) | (state & LMASK);

    uint16_t xA = x >> 1;
    if (x & 0x00000001UL) {
      xA ^= a;
    }

    x = state ^ xA;
    state = x;

    uint16_t y = x ^ (x >> u);
    y ^= ((y << s) & b);
    y ^= ((y << t) & c);
    uint16_t z = y ^ (y >> l);

    return z;
  }
};

//Tried writing xorShift myself
struct XorShift {
  std::unordered_map<uint16_t, uint16_t> stateMap;
  uint16_t state;
  uint16_t getRandomNumber(uint16_t seed) {
    uint16_t x = seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return x;
  }
};

int main(){

  uint16_t seed, N, min, max;
  std::cin >> seed >> N >> min >> max;

  //xorShiftCode
  XorShift xorShift;
  for (int i = 0; i < N; i++) {
    uint16_t initialSeed = seed;
    if (xorShift.stateMap.contains(seed)) {
      std::cout << "Cycle has restarted" << std::endl;
      break;
    }
    seed = xorShift.getRandomNumber(seed);
    std::cout << min + (seed % (max - min + 1)) << std::endl;
    xorShift.stateMap.emplace(initialSeed, seed);
  }

  //Mersenne Twister Code
  /*MersenneTwister mt;
  mt.init(seed);

  for (int i = 0; i < N; i++) {
    uint16_t initialState = mt.state;
    if (mt.stateMap.contains(mt.state)) {
      std::cout << "Cycle has restarted" << std::endl;
      break;
    }

    int randomNumber = mt.randomInt();
    std::cout << min + (randomNumber % (max - min + 1)) << std::endl;
    mt.stateMap[initialState] = randomNumber;
  }*/
}