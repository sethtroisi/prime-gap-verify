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

import gmpy2

import parsenumber
import utils


def brute(s, g, mp):
    end = s + g
    composite = [n % 2 == 0 for n in range(s, end + 1)]

    if s <= 1 <= end:
        # 1 is composite
        composite[1 - s] = True

    if s <= 2 <= end:
        # 2 is prime
        composite[2 - s] = False

    last = min(int(math.sqrt(end)) + 1, mp)
    assert last == mp or last * last >= end

    # check if odds have small factors
    for p in range(3, mp+1, 2):
        # mark off all factors p*p + i*p
        p2 = p * p
        if p2 >= s:
            start = p2 - s
        else:
            # First odd multiple > s
            div, start = divmod(-s, p)
            if div % 2 == 0:
                start += p

        for m in range(start, g+1, 2*p):
            composite[m] = True

    return composite


def test_sieve():
    for s, g, mp in (
        (1, 100, 10),
        (11, 89, 10),
        (100, 100, 10),
        (1001, 100, 10),
        (1001, 1000, 10),
        (1001, 100, 50),
        (1001, 1000, 50),
        (5, 1000, 3),
   ):
        expect = brute(s, g, mp)
        result = utils.sieve(s, g, mp)
        assert len(expect) == len(result)
        #print([1 * v for v in expect])
        #print([1 * v for v in result])
        #print([s + i for i, v in enumerate(expect) if v])
        print([(s+i, a, b) for i, (a, b) in enumerate(zip(expect, result)) if a != b])
        assert expect == result, (s, g, mp)


def test_sieve_primepi():
    # Easy to generate these with `primesieve <start> <start + gap>
    for s, g, mp, expected in (
        (0,  100, 10, 25),
        (1,   99, 10, 25),
        (1,   99, 100, 25),
        (101, 100, 20, 21),
        (101, 100, 100, 21),
        (1001, 1000, 3, 334),
        (1001, 1000, 30, 151),
        (1001, 1000, 50, 135),
        (1000001, 10000, 1100, 753),
    ):
        assert expected == utils.sieve(s, g, mp).count(False)


def test_max_prime_overlaps_interval():
    # Test when max_prime > N
    for s, g, mp, expected in (
        (1, 99, 2, 50),
        (1, 99, 3, 34),
        (1, 99, 5, 28),
        (1, 99, 7, 25),
        (1, 99, 11, 25),
        (1, 99, 100, 25),
        (1, 99, 200, 25),
    ):
        #print (s, g, mp, expected, [s+i for i, v in enumerate(utils.sieve(s, g, mp)) if not v])
        assert expected == utils.sieve(s, g, mp).count(False)
        assert utils.sieve(s, g, mp) == brute(s, g, mp)

def test_limit_limitted():
    # Test that limit is decreased to sqrt(s + g)
    mp = 10 ** 10
    for s, g, expected in (
        (1, 99, 25),
        (1000, 1000, 135),
        (10 ** 6, 100, 6),
        (10 ** 9, 100, 7),
    ):
        composites = utils.sieve(s, g, mp)
        for i, c in enumerate(composites):
            assert gmpy2.is_prime(s + i) == (not c)
        assert expected == composites.count(False)

def test_zero_one_composite():
    # Test that 0, 1 are "composite" in our sieve
    for s, g in (
        (0, 10),
        (1, 5),
        (0, 1),
        (0, 2),
        (1, 1),
        (1, 2)
    ):
        composites = utils.sieve(s, g, 10)
        if s == 0:
            assert composites[0] == True
        if s + g - 1 >= 1:
            assert composites[1 - s] == True

def test_validate():
    for s, g in ((101, 2), (103, 4), (113, 14), (360653, 96),
                 (18361375334787046697, 1550),
                 (9691983639208775401081992556968666567067, 2982)):
        assert utils.validate(s, g)
        if g > 2:
            assert not utils.validate(s, g - 2)
        assert not utils.validate(s, g + 2)


def test_check_pfgw_available():
    assert utils.check_pfgw_available(), "Assumed to be true on dev machines"


NUM_STATUS = (
        ("31# +1", True),
        ("379# +1", True),
        ("1019# +1", True),
        ("2657# +1", True),
        ("503# -617", False),
        ("503# -631", False),
        ("503# -659", True)
)


def test_is_prime_large():
    for str_num, result in NUM_STATUS:
        num = parsenumber.parse(str_num)
        assert utils.is_prime_large(num) == result
        assert utils.is_prime_large(num, str_num) == result


def test__is_prime_pfgw():
    for str_num, result in NUM_STATUS:
        num = parsenumber.parse(str_num)
        assert utils._is_prime_pfgw(num) == result
        assert utils._is_prime_pfgw(str_num) == result
