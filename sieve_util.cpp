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

#include "sieve_util.hpp"
#include "primes.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <gmp.h>

namespace sieve_util {

    // Similiar math to mpz/nextprime.c
    uint64_t calculate_sievelimit(double n_bits, double gap) {
        // Average gap = bits * ln(2)
        double adj = gap / (n_bits * log(2));

        double limit = adj * pow(1.0 * n_bits, 2.5) / 124.0;
        if (limit > MAX_LIMIT) {
            fprintf(stderr, "LARGE LIMIT=%.2e\n", limit);
            limit = MAX_LIMIT;
        }
        assert(limit > 1000);
        return limit;
    }


    void sieve(uint64_t m, uint64_t p, uint64_t d, uint64_t a, uint64_t gap) {
        mpz_t N;
        mpz_init(N);

        // Verify P is prime
        assert(primes::isprime_brute(p));

        // Verify d divides P#
        mpz_primorial_ui(N, p);
        assert(0 == mpz_tdiv_q_ui(N, N, d));

        mpz_mul_ui(N, N, m);

        assert(-a > 0);
        mpz_sub_ui(N, N, -a);

        int bits = mpz_sizeinbase(N, 2);
        uint64_t limit = calculate_sievelimit(bits, gap);
        fprintf(stderr, "bits: %5d  gap: %6ld  limit: %'ld\n", bits, gap, limit);
        fprintf(stderr, "expect ~~%.0f remaining\n", 1.0 * gap / (log(limit) * 1.7811));

        // Sieve
        size_t odds = gap / 2 + 1;
        std::vector<bool> composite(odds, 0);

        size_t prime_count = 1;
        primes::iterator iter;
        uint64_t prime = iter.next();
        assert(prime == 2);  // Skip  2

        // small primes can divide multiple numbers
        for (; prime <= gap; prime = iter.next()) {
            prime_count++;
            uint64_t mod = mpz_fdiv_ui(N, prime);
            uint64_t first = prime - mod;
            if (first % 2 == 1) {
                first += prime;
            }

            for (uint32_t d = first; d <= gap; d += 2 * prime) {
                composite[d / 2] = true;
            }
        }

        size_t unknowns = std::count(composite.begin(), composite.end(), false);
        size_t count_c = gap - unknowns;
        fprintf(stderr, "%ld / %ld = %.2f composite, %ld remaining (primes %ld)\n",
                gap - unknowns, gap, 100.0 * count_c / gap, unknowns, prime_count);

        for (; prime < limit; prime = iter.next()) {
            prime_count++;
            uint64_t first = prime - mpz_fdiv_ui(N, prime);
            if ((first < gap) && ((first & 1) == 0)) {
                composite[first / 2] = true;
            }
        }

        assert(composite[0] == false);
        assert(composite[odds-1] == false);

        // Write out some status
        unknowns = std::count(composite.begin(), composite.end(), false);
        count_c = gap - unknowns;
        fprintf(stderr, "%ld / %ld = %.2f composite, %ld remaining (primes %ld)\n",
                gap - unknowns, gap, 100.0 * count_c / gap, unknowns, prime_count);

        for(size_t i = 0; i < odds; i++) {
            if (!composite[i]) {
                printf("%ld * %ld# / %ld + %ld\n", m, p, d, a+2*i);
            }
        }
    }
}  // namespace sieve_util
