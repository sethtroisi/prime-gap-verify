// Copyright 2020 Seth Troisi
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* sieve_util.cpp
 * [1] See math in next_prime.c in gmp-lib (My first pass at this logic)
 *    https://gmplib.org/list-archives/gmp-devel/2020-March/005767.html
 *    https://gmplib.org/repo/gmp-6.2/file/tip/mpz/nextprime.c
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
        return std::max((uint64_t) limit, 2UL);
    }


    std::vector<char> sieve(mpz_t &N, uint64_t gap, uint64_t limit, size_t &prime_count) {
        // 10GB would be a lot ram.
        if ((gap < 0) || (gap > (1L << 40))) { return {}; }
        if (limit > (1L << 50)) { return {}; }

        // Sieve of [N, N+gap]
        gap += 1;
        std::vector<char> composite(gap, 0);

        // primes >= N_int, should avoid marking themselves off.
        uint64_t N_int = mpz_cmp_ui(N, limit) <= 0 ? mpz_get_ui(N) : limit + 1;
        // only needed when limit > N
        uint64_t N_end = N_int + gap;

        // Mark 0 as composite because.
        if ((N_int == 0) && (N_end >= 0)) {
            composite[0] = true;
        }

        // Mark 1 as composite because.
        if ((N_int <= 1) && (N_end >= 1)) {
            composite[1 - N_int] = true;
        }

        // limit doesn't need to exceed sqrt(N + gap)
        if (mpz_sizeinbase(N, 2) < 128) {
            mpz_t temp;
            mpz_init(temp);

            mpz_add_ui(temp, N, gap);
            mpz_sqrt(temp, temp);
            if (mpz_cmp_ui(temp, limit) < 0) {
                if (!mpz_fits_ulong_p(temp)) { return {}; }
                limit = mpz_get_ui(temp);
            }

            mpz_clear(temp);
        }

        if (limit > N_end) { return {}; }

        // Remove all evens
        for (uint32_t d = mpz_cdiv_ui(N, 2); d < gap; d += 2) {
            composite[d] = true;
        }
        if ((N_int <= 2) && (N_end >= 2)) {
            // Go back and mark 2 as prime
            composite[2 - N_int] = false;
        }

        prime_count = 1;
        primes::iterator iter;
        uint64_t prime = iter.next();
        assert(prime == 2);  // Skip  2
        prime = iter.next();
        assert(prime == 3);

        // small primes can divide multiple numbers
        for (; prime < gap && prime <= limit; prime = iter.next()) {
            prime_count++;

            uint64_t two_p = 2 * prime;
            uint64_t first = mpz_cdiv_ui(N, two_p);
            first += prime;
            if (first >= two_p) first -= two_p;

            for (uint32_t d = first; d < gap; d += two_p) {
                composite[d] = true;
            }

            if (N_int <= prime) {
                // mark prime as prime
                composite[prime - N_int] = false;
            }
        }

        for (; prime <= limit; prime = iter.next()) {
            prime_count++;

            // Only one in the interval
            // either N_int <= prime (the number is the prime)
            // or N_int > prime (and we mark off some multiple
            if (N_int > prime) {

                uint64_t two_p = 2 * prime;
                uint64_t first = mpz_cdiv_ui(N, two_p);
                first += prime;
                if (first >= two_p) first -= two_p;
                if (first < gap) {
                    composite[first] = true;
                }
            }
        }

        // Possible a vector copy, but fast in the overall scheme of things.
        return composite;
    }
}  // namespace sieve_util
