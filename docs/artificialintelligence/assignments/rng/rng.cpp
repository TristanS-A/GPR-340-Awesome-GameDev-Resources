// add your imports here
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <unordered_map>
#include <vector>

//For Mersenne Twister
#define n 624 //The degree of recurrence
#define m 397 //The middle word that is above 0 and under n
#define f 1812433253UL
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

const std::string TEST_FOLDER = "\\tests\\";

uint32_t xorShift(uint32_t seed, int r1, int r2);

struct MersenneTwister {
  struct MTwisterState {
    std::vector<uint32_t> stateSet;
    int stateIndex;
  };

  MTwisterState state;

  void init(uint32_t seed) {
    state.stateSet.reserve(n);
    state.stateSet.push_back(seed);

    for (int i = 1; i < n; i++) {
      seed = f * (seed ^ (seed >> (w-2))) + i;
      state.stateSet.push_back(seed);
    }

    state.stateIndex = 0;
  }

  uint32_t randomInt() {
    int k = state.stateIndex;
    int j = k - (n - 1);
    if (j < 0) {
      j += n;
    }

    uint32_t x = (state.stateSet[k] & UMASK) | (state.stateSet[j] & LMASK);

    uint32_t xA = x >> 1;
    if (x == 0x00000001UL) {
      xA ^= x;
    }

    j = k - (n - m);
    if (j < 0) {
      j += n;
    }

    x = state.stateSet[j] ^ xA;
    state.stateSet[k++] = x;

    if (k >= n) {
      k = 0;
    }

    state.stateIndex = k;

    uint32_t y = x ^ (x >> u);
    y ^= ((y << s) & b);
    y ^= ((y << t) & c);
    uint32_t z = y ^ (y >> l);

    return z;
  }
};

int main(){

  uint32_t seed, N, min, max, result;
  std::cin >> seed >> N >> min >> max;

  //xorShiftCode
  /*for (int i = 0; i < N; i++) {
    seed = xorShift(seed, min, max);
    std::cout << min + (seed % (max - min + 1)) << std::endl;
  }*/

  //Mersenne Twister Code
  MersenneTwister mt;
  mt.init(seed);

  for (int i = 0; i < N; i++) {
    seed = mt.randomInt();
    std::cout << min + (seed % (max - min + 1)) << std::endl;
  }
}

//Tried writing xorShift myself
  uint32_t xorShift(uint32_t seed)
  {
    static std::unordered_map<uint32_t, uint32_t> stateMap;
    if (stateMap.contains(seed)) {
      std::cout << "REPEAT" << std::endl;
      return stateMap[seed];
    }
  
    uint32_t x = seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    stateMap[seed] = x;

    return x;
  }