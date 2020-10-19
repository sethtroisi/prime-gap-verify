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

import primegapverify

def brute(s, g, mp):
    return [n % 2 == 0 or any(n % p == 0 for p in range(3, mp+1, 2))
        for n in range(s, s + g + 1, 2)]


def test_sieve():
    for s, g, mp in (
        (1001, 100, 10),
        (1001, 1000, 10),
        (1001, 100, 50),
        (1001, 1000, 50),
        (5, 1000, 3),
   ):
        expect = brute(s, g, mp)
        result = primegapverify.sieve(s, g, mp)
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
        assert expected == primegapverify.sieve(s, g, mp).count(False)

def test_validate():
    for s, g in ((101, 2), (103, 4), (113, 14), (360653, 96),
                 (18361375334787046697, 1550),
                 (9691983639208775401081992556968666567067, 2982)):
        assert primegapverify.validate(s, g)
        if g > 2:
            assert not primegapverify.validate(s, g - 2)
        assert not primegapverify.validate(s, g + 2)
