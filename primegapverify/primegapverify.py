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

import verify


def parse_primorial(number):
    pass


def sieve(start, gap, max_prime=None):
    if max_prime is None or max_prime == 0:
        max_prime = verify.sieve_limit(math.log2(start), gap)

    return verify.sieve_interval(start, gap, max_prime)


def test(start, gap, max_prime=None):
    pass
