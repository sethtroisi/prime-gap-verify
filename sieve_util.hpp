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

#include "primes.hpp"

namespace sieve_util {
    const uint64_t MAX_LIMIT = 10'000'000'000;

    uint64_t calculate_sievelimit(double n_bits, double gap);
    void sieve(uint64_t m, uint64_t p, uint64_t d, uint64_t a, uint64_t gap);
}
