# Copyright 2020 Seth Troisi
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import math
import verify as _verify

import gmpy2

def sieve(start, gap, max_prime=None):
    if max_prime is None or max_prime == 0:
        max_prime = _verify.sieve_limit(math.log2(start), gap)

        # Needed if really small start
        if max_prime >= start:
            max_prime = (start - 1)

        if max_prime <= 2:
            max_prime = 3

    assert start > max_prime, (start, max_prime)

    return _verify.sieve_interval(str(start), gap, max_prime)


def validate(start, gap, max_prime=None):
    """Validate start, start+gap are prime and the interior is composite"""

    # TODO: Verbose printing or return

    if not gmpy2.is_prime(start):
        print ("Start not prime!")
        return False

    if not gmpy2.is_prime(start + gap):
        print ("End not prime!")
        return False

    composites = sieve(start, gap, max_prime)
    for i, composite in enumerate(composites[1:-1], 1):
        if not composite and gmpy2.is_prime(start + 2 * i):
            print ("Interior point is prime: start +", 2 * i)
            return False


    return True
