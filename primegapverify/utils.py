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
import subprocess

import gmpy2

import verify

def sieve(start, gap, max_prime=None):
    if max_prime is None or max_prime == 0:
        max_prime = verify.sieve_limit(math.log2(start), gap)

        # Needed if really small start
        if max_prime >= start:
            max_prime = (start - 1)

        if max_prime <= 2:
            max_prime = 3

    assert start > max_prime, (start, max_prime)

    return verify.sieve_interval(str(start), gap, max_prime)


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
        if i % 2 == 1:
            assert composite # all evens should be composite
        if not composite and gmpy2.is_prime(start + i):
            print ("Interior point is prime: start +", i)
            return False

    return True


def is_prime_large(num, str_num=None):
    """Determine if num is prime.

    Uses gmpy2.is_prime() or pfgw.
    gmpy2.is_prime() is ~5x slower on primes than composites.
    pfgw is same speed for primes & composites.

    for pfgw uses "-q(str_num or str(num))" when log2(num) > 8000

    https://github.com/sethtroisi/misc-scripts/tree/main/prime-time
    https://github.com/aleaxit/gmpy/issues/265
    """
    if gmpy2.num_digits(num, 2) > 8000:
        return _is_prime_pfgw(str_num or str(num))

    return gmpy2.is_prime(num)


def _is_prime_pfgw(num):
    # Overhead of subprocess calls seems to be ~0.03
    # Process seems to use more than 1 thread
    s = subprocess.getstatusoutput(f"pfgw64 -f0 -q'{num}'")
    #assert s[1].startswith('PFGW'), s
    assert "PFGW" in s[1], s
    return s[0] == 0


def check_pfgw_available():
    s = subprocess.getstatusoutput("pfgw64 -k -f0 -q'10^700 + 7'")
    if not (s[0] == 0 and "10^700 + 7 is 3-PRP! " in s[1]):
        return False

    t = subprocess.getstatusoutput("pfgw64 -k -f0 -q'10^700 + 3'")
    if not (t[0] == 1 and "10^700 + 3 is composite: RES64: [44B46CC0948A0831]" in t[1]):
        return False

    return True
