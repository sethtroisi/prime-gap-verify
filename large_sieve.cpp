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

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <vector>

#include <gmp.h>

const uint64_t MAX_LIMIT = 10'000'000'000;

typedef long long ll;

void print_usage(char *name) {
    printf("Usage %s  m P d a gapsize\n\n", name);
    printf("Sieve and ouput numbers to check (including endpoints)\n\n");
}

void sieve(uint64_t m, uint64_t p, uint64_t d, uint64_t a, uint64_t gap);

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
    sieve(m, p, d, a, gap);
}


// Similiar math to mpz/nextprime.c
static uint64_t
calculate_sievelimit(double n_bits, double gap) {
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

static bool isprime_brute(uint32_t n) {
    if ((n & 1) == 0)
        return false;
    for (uint32_t p = 3; p * p <= n; p += 2)
        if (n % p == 0)
            return false;
    return true;
}

void get_sieve_primes_segmented_lambda(uint64_t n, std::function<void(uint64_t)> lambda) {
    // Large enough to be fast and still fit in L1/L2 cache.
    uint32_t BLOCKSIZE = 1 << 16;
    uint32_t ODD_BLOCKSIZE = BLOCKSIZE >> 1;
    std::vector<char> is_prime(ODD_BLOCKSIZE, true);

    lambda(2L);

    std::vector<int32_t> primes = {3};
    // First number in next block that primes[pi] divides.
    std::vector<int32_t> next_mod = {9 >> 1};

    uint32_t p_lim = 5;
    uint64_t p2_lim = p_lim * p_lim;

    for (uint64_t B = 0; B < n; B += BLOCKSIZE) {
        uint64_t B_END = B + BLOCKSIZE - 1;
        if (B_END > n) {
            BLOCKSIZE = (n - B);
            ODD_BLOCKSIZE = (n - B + 1) >> 1;
            B_END = n;
        }

        while (p2_lim <= B_END) {
            if (isprime_brute(p_lim)) {
                primes.push_back(p_lim);
                assert( p2_lim >= B );
                next_mod.push_back((p2_lim - B) >> 1);
            }
            p2_lim += 4 * p_lim + 4;
            p_lim += 2;
            //assert( p_lim * p_lim == p2_lim );
        }

        // reset is_prime
        std::fill(is_prime.begin(), is_prime.end(), true);
        if (B == 0) is_prime[0] = 0; // Skip 1

        // Can skip some large pi up to certain B (would have to set next_mod correctly)
        for (uint32_t pi = 0; pi < primes.size(); pi++) {
            const uint32_t prime = primes[pi];
            uint32_t first = next_mod[pi];
            for (; first < ODD_BLOCKSIZE; first += prime) {
                is_prime[first] = false;
            }
            next_mod[pi] = first - ODD_BLOCKSIZE;
        }
        for (uint32_t prime = 0; prime < ODD_BLOCKSIZE; prime++) {
            if (is_prime[prime]) {
                lambda(B + 2 * prime + 1);
            }
        }
    }
}


void sieve(uint64_t m, uint64_t p, uint64_t d, uint64_t a, uint64_t gap) {
    mpz_t N;
	mpz_init(N);

	// Verify P is prime
    assert(isprime_brute(p));

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

	// see prime-gap project

    // small primes can divide multiple numbers
    for (uint64_t prime = 3; prime <= gap; prime += 2) {
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
    fprintf(stderr, "%ld / %ld = %.2f composite, %ld remaining\n",
            gap - unknowns, gap, 100.0 * count_c / gap, unknowns);

    get_sieve_primes_segmented_lambda(limit, [&](const uint64_t prime) {
        uint64_t first = prime - mpz_fdiv_ui(N, prime);
        if ((first < gap) && ((first & 1) == 0)) {
            composite[first / 2] = true;
        }
    });

    assert(composite[0] == false);
    assert(composite[odds-1] == false);

    // Write out some status
    unknowns = std::count(composite.begin(), composite.end(), false);
    count_c = gap - unknowns;
    fprintf(stderr, "%ld / %ld = %.2f composite, %ld remaining\n",
            gap - unknowns, gap, 100.0 * count_c / gap, unknowns);

    for(size_t i = 0; i < odds; i++) {
        if (!composite[i]) {
	        printf("%ld * %ld# / %ld + %ld\n", m, p, d, a+2*i);
        }
    }
}
