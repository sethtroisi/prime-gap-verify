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
        assert(limit > 1000);
        return limit;
    }


    std::vector<bool> sieve(mpz_t &N, uint64_t gap, uint64_t limit, size_t &prime_count) {
        // Sieve
        size_t odds = gap / 2 + 1;
        std::vector<bool> composite(odds, 0);

        prime_count = 1;
        primes::iterator iter;
        uint64_t prime = iter.next();
        assert(prime == 2);  // Skip  2
        prime = iter.next();
        assert(prime == 3);

        // small primes can divide multiple numbers
        for (; prime <= gap && prime <= limit; prime = iter.next()) {
            prime_count++;
            uint64_t first = mpz_cdiv_ui(N, prime);
            if (first % 2 == 1) {
                first += prime;
            }

            for (uint32_t d = first; d <= gap; d += 2 * prime) {
                composite[d / 2] = true;
            }
        }

        // TODO make sure limit * limit < N

        /* Think about returning this for debug or something
        size_t unknowns = std::count(composite.begin(), composite.end(), false);
        size_t count_c = gap - unknowns;
        fprintf(stderr, "%ld / %ld = %.2f composite, %ld remaining (primes %ld)\n",
                gap - unknowns, gap, 100.0 * count_c / gap, unknowns, prime_count);
        */

        for (; prime <= limit; prime = iter.next()) {
            prime_count++;
            uint64_t first = mpz_cdiv_ui(N, prime);
            if ((first < gap) && ((first & 1) == 0)) {
                composite[first / 2] = true;
            }
        }

        assert(composite[0] == false);
        assert(composite[odds-1] == false);

        // Possible a vector copy, but fast in the overall scheme of things.
        return composite;
    }
}  // namespace sieve_util
