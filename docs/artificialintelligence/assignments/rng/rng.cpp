// add your imports here
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <unordered_map>
#include <vector>

//For Mersenne Twister
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
  uint32_t state;
  std::unordered_map<uint32_t, uint32_t> stateMap;

  void init(uint32_t seed) {
    seed = f * (seed ^ (seed >> (w-2))); //Initializes state
    state = seed;
  }

  uint32_t randomInt() {
    uint32_t x = (state & UMASK) | (state & LMASK); //Converts previous state into new bit shifted num

    //Sets up current state
    uint32_t xA = x >> 1;
    if (x & 0x00000001UL) {
      xA ^= a;
    }

    //Updates current state
    x = state ^ xA;
    state = x;

    //Tempering for random number result
    uint32_t y = x ^ (x >> u);
    y ^= ((y << s) & b);
    y ^= ((y << t) & c);
    uint32_t z = y ^ (y >> l);

    return z;
  }
};

//Tried writing xorShift myself
struct XorShift {
  std::unordered_map<uint32_t, uint32_t> stateMap;
  uint32_t state;
  uint32_t getRandomNumber(uint32_t seed) {
    uint32_t x = seed;

    //xOrShifts 3 times
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return x;
  }
};

int main(){

  uint32_t seed, N, min, max;
  std::cin >> seed >> N >> min >> max; //Gets input values

  int count = 0; //Count variable to count cycle length

  //xorShiftCode
  /*XorShift xorShift;
  for (int i = 0; i < N; i++) { //Runs through numbers to generate
    uint32_t initialSeed = seed; //Saves initial seed

    //Checks if the cycle has repeated and breaks loop
    if (xorShift.stateMap.contains(seed)) {
      std::cout << "Cycle has restarted" << std::endl;
      std::cout << "Cycle length: " << count << std::endl;
      break;
    }
    seed = xorShift.getRandomNumber(seed); //Gets random number and updates the seed
    std::cout << min + (seed % (max - min + 1)) << std::endl; //Clamps result to specified range and outputs it
    xorShift.stateMap.emplace(initialSeed, seed); //Adds result to stateMap with the initial seed as the key
    count++; //Increments the cycle count
  }*/

  //Mersenne Twister Code
  MersenneTwister mt;
  mt.init(seed); //Initiates Twister

  //Runs through numbers to generate
  for (int i = 0; i < N; i++) {
    uint32_t initialState = mt.state; //Saves initial seed

    //Checks if the cycle has repeated and breaks loop
    if (mt.stateMap.contains(mt.state)) {
      std::cout << "Cycle has restarted" << std::endl;
      std::cout << "Cycle length: " << count << std::endl;
      break;
    }

    int randomNumber = mt.randomInt(); //Gets random number
    std::cout << min + (randomNumber % (max - min + 1)) << std::endl; //Clamps result to specified range and outputs it
    mt.stateMap[initialState] = randomNumber; //Adds result to stateMap with the initial seed as the key
    count++; //Increments cycle count
  }
}