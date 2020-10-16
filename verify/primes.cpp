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

#include "primes.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

namespace primes {
    /**
     * Cleanup version of
     * https://github.com/sethtroisi/misc-scripts/tree/main/prime-iterator
     * Inspiration from Bruce @ randomascii.wordpress.com about optimized code.
     */

    bool isprime_brute(uint32_t n) {
        if ((n & 1) == 0)
            return false;
        for (uint32_t p = 3; p * p <= n; p += 2)
            if (n % p == 0)
                return false;
        return true;
    }

#ifdef HANDROLLED
    class PrimeIterator {
        public:
            PrimeIterator() = default;
            ~PrimeIterator() = default;

            uint64_t next_prime() {
                /* Ideally find a way to avoid this */
                if (B == 1) {
                    B = 0;

                    is_prime.resize(ODD_BLOCKSIZE);
                    sieve_next_interval();
                    is_prime[0] = 0; // Skip 1
                    return 2;
                }

                while (true) {
                    // Search current block
                    for(; block_i < ODD_BLOCKSIZE; block_i++) {
                        if (is_prime[block_i]) {
                            //return B + 2 * block_i + 1;
                            // Set for next
                            block_i++;
                            return B + 2 * block_i - 1;
                        }
                    }

                    B += BLOCKSIZE;
                    sieve_next_interval();
                    block_i = 0;
                }
            }

        private:
            void sieve_next_interval() {
                uint64_t B_END = B + BLOCKSIZE - 1;

                // Make sure enough primes for B_END
                while (true) {
                    uint64_t lastp = primes.empty() ? 0 : primes.back();
                    if (lastp * lastp > B_END) break;

                    // Find a next prime via brute force.
                    uint64_t nextp = lastp == 0 ? 3 : lastp + 2;
                    for (; ; nextp += 2) {
                        bool isp = true;
                        for (uint32_t p : primes) {
                            if (nextp % p == 0) {
                                isp = false;
                                break;
                            }
                        }
                        if (isp) break;
                    }

                    primes.push_back(nextp);
                    if (B == 0) {
                        next_mod.push_back(nextp * nextp >> 1);
                    } else {
                        // Next odd multiple of nextp >= B
                        uint64_t mult = (B-1) / nextp + 1;
                        uint64_t first = (mult | 1) * nextp;
                        assert( first >= B );
                        assert( first / nextp % 2 == 1 );
                        first -= B;
                        next_mod.push_back(first >> 1);
                    }
                }

                std::fill(is_prime.begin(), is_prime.end(), true);

                for (uint32_t pi = 0; pi < primes.size(); pi++) {
                    const uint32_t prime = primes[pi];
                    uint32_t first = next_mod[pi];
                    for (; first < ODD_BLOCKSIZE; first += prime){
                        is_prime[first] = false;
                    }
                    next_mod[pi] = first - ODD_BLOCKSIZE;
                }
            }

            // Large enough to be fast and still fit in L1/L2 cache.
            const uint64_t BLOCKSIZE = 1 << 16;
            const uint64_t ODD_BLOCKSIZE = BLOCKSIZE >> 1;

            // Start of current block
            uint64_t B = 1;

            // Index for current interval in is_prime
            uint64_t block_i = 0;
            std::vector<char> is_prime;

            std::vector<uint32_t> primes;
            // First number in next block that primes[pi] divides.
            std::vector<int32_t> next_mod;
    };

    uint64_t iterator::next() {
        return prime_iter->next_prime();
    }

    iterator::iterator() {
        prime_iter.reset(new PrimeIterator());
    }
    iterator::~iterator() = default;
#endif  // HANDROLLED

}  // namespace primes
