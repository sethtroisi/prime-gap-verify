/* large_sieve.cpp
 * $ g++ -O2 script/large_sieve.cpp -o bin/large_sieve -lm -lgmp
 * $ time bin/large_sieve 77803 10709 46410 -216906 263222 > tmp/test.txt
 2*
 * Author: Seth Troisi
 * Modified: 2020
 *
 * Take input gap (m, p, d, a, gap)
 *      start = (m * p#/d) -a
 *      end   = (m * p#/d) -a + gap
 *
 * Sieve up to some reasonable bound [1]
 * print to stdout numbers that need to be checked
 *
 * [1] See math in next_prime.c in gmp-lib (by Seth)
 */

#pragma once

#include "primes.hpp"

namespace sieve_util {
    const uint64_t MAX_LIMIT = 10'000'000'000;

    uint64_t calculate_sievelimit(double n_bits, double gap);
    void sieve(uint64_t m, uint64_t p, uint64_t d, uint64_t a, uint64_t gap);
}
