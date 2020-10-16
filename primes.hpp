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

#pragma once

#include<memory>

#ifndef HANDROLLED
#include <primesieve.hpp>
#endif  // HANDROLLED


namespace primes {
    class PrimeIterator;

    bool isprime_brute(uint32_t n);

#ifdef HANDROLLED
    class iterator {
        public:
            // [De]Constructor must be defined after PrimeItator is complete
            iterator();
            ~iterator();

            uint64_t next();

        private:
            std::unique_ptr<PrimeIterator> prime_iter;
    };
#else
    // See README
    #include <primesieve.hpp>

    class iterator {
        public:
            iterator() = default;
            ~iterator() = default;

            uint64_t next() { return prime_iter.next_prime(); }
        private:
            primesieve::iterator prime_iter;
    };
#endif  // HANDROLLED
}
