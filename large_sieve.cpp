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

#include <cstdint>
#include <cstdio>

typedef long long ll;

void print_usage(char *name) {
    printf("Usage %s  m P d a gapsize\n\n", name);
    printf("Sieve and ouput numbers to check (including endpoints)\n\n");
}

int main(int argc, char ** argv) {
    if (argc != 6) {
        print_usage(argv[0]);
        exit(1);
    }

    // Validate input
    ll m = atol(argv[1]);
    ll p = atol(argv[2]);
    ll d = atol(argv[3]);
    ll a = atol(argv[4]);
    ll gap = atol(argv[5]);

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

    if (gap <= a || gap > 7000000) {
        printf("Invalid gap=%lld\n", gap);
        exit(1);
    }

	fprintf(stderr, "sieving %lld * %lld# / %lld + [%lld, %lld]\n", m, p, d, a, a+gap);
    sieve_util::sieve(m, p, d, a, gap);
}
