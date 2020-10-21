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

#include "verify/primes.hpp"
#include "verify/sieve_util.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>

#include <gmp.h>

typedef long long ll;

void print_usage(char *name) {
    printf("Usage %s  m P d a gapsize [limit]\n\n", name);
    printf("Sieve and ouput numbers to check (including endpoints)\n\n");
}

void calc_N(mpz_t &N, ll m, ll p, ll d, ll a) {
    mpz_primorial_ui(N, p);
    mpz_mul_ui(N, N, m);

    // Verify d divides P#
    assert(0 == mpz_tdiv_q_ui(N, N, d));

    assert(-a > 0);
    mpz_sub_ui(N, N, -a);
}

int main(int argc, char ** argv) {
    if (argc < 6 || argc > 7) {
        print_usage(argv[0]);
        exit(1);
    }

    // Validate input
    ll m = atol(argv[1]);
    ll p = atol(argv[2]);
    ll d = atol(argv[3]);
    ll a = atol(argv[4]);
    ll gap = atol(argv[5]);
    uint64_t limit = 0;
    if (argc == 7) {
        limit = atol(argv[6]);
    }

    if (m <= 0 || m > INT32_MAX) {
        printf("Invalid m=%lld\n", m);
        exit(1);
    }

    if (p <= 50 || p > 40000) {
        printf("Invalid p=%lld\n", p);
        exit(1);
    }
    if (!primes::isprime_brute(p)) {
        printf("P(%lld) not prime!", p);
        exit(1);
    }

    if (d <= 0) {
        printf("Invalid d=%lld\n", d);
        exit(1);
    }

    if (a >= 0 || a < -6000000) {
        printf("Invalid a=%lld\n", a);
        exit(1);
    }

    if (gap <= a || gap > 7000000 || gap % 2 == 1) {
        printf("Invalid gap=%lld\n", gap);
        exit(1);
    }

    if (limit > 11'000'000'000'000) {
        printf("Invalid limit=%ld\n", limit);
    }

	fprintf(stderr, "sieving %lld * %lld# / %lld + [%lld, %lld]\n", m, p, d, a, a+gap);

    /* N = m * P# / d - a */
    mpz_t N;
    mpz_init(N);
    calc_N(N, m, p, d, a);

    /* Input stats */
    int bits = mpz_sizeinbase(N, 2);
    if (limit == 0) {
        limit = sieve_util::calculate_sievelimit(bits, gap);
    }
    fprintf(stderr, "bits: %5d  gap: %6lld  limit: %'ld\n", bits, gap, limit);
    fprintf(stderr, "expect ~~%.0f remaining\n", 1.0 * gap / (log(limit) * 1.7811));

    size_t prime_count = 0;
    auto composite = sieve_util::sieve(N, gap, limit, prime_count);
    size_t odds = gap / 2 + 1;
    assert(composite.size() == odds);

    /* Final stats */
    size_t unknowns = std::count(composite.begin(), composite.end(), 0);
    size_t count_c = gap - unknowns;
    fprintf(stderr, "%lld / %lld = %.2f composite, %ld remaining (primes %ld)\n",
            gap - unknowns, gap, 100.0 * count_c / gap, unknowns, prime_count);

    /* Output */
    for(size_t i = 0; i < composite.size(); i++) {
        if (!composite[i]) {
            printf("%lld * %lld# / %lld + %lld\n", m, p, d, a+2*i);
        }
    }
}
