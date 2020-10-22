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

import parsenumber
import utils

def brute(s, g, mp):
    return [n % 2 == 0 or any(n % p == 0 for p in range(3, mp+1, 2))
        for n in range(s, s + g + 1)]


def test_sieve():
    for s, g, mp in (
        (1001, 100, 10),
        (1001, 1000, 10),
        (1001, 100, 50),
        (1001, 1000, 50),
        (5, 1000, 3),
   ):
        expect = brute(s, g, mp)
        result = utils.sieve(s, g, mp)
        print (len(expect), len(result))
        print([1 * v for v in expect])
        print([1 * v for v in result])
        assert expect == result


def test_sieve_primepi():
    # Easy to generate these with `primesieve <start> <start + gap>
    for s, g, mp, expected in (
        (101, 100, 20, 21),
        (101, 100, 100, 21),
        (1001, 1000, 3, 334),
        (1001, 1000, 30, 151),
        (1001, 1000, 50, 135),
        (1000001, 10000, 1100, 753),
    ):
        assert expected == utils.sieve(s, g, mp).count(False)


def test_validate():
    for s, g in ((101, 2), (103, 4), (113, 14), (360653, 96),
                 (18361375334787046697, 1550),
                 (9691983639208775401081992556968666567067, 2982)):
        assert utils.validate(s, g)
        if g > 2:
            assert not utils.validate(s, g - 2)
        assert not utils.validate(s, g + 2)


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
        print ("Hi", str_num, num)
        assert utils.is_prime_large(num) == result
        assert utils.is_prime_large(num, str_num) == result
