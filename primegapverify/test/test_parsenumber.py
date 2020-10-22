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

# Standard form         | m * P# / d +- a
# primorial d           | m * P# / d# +- a
# two component d       | m * P# / (d1 * d2) +- a
# two comp primorial d  | m * P# / (d# * d2) +- a
# No m                  | P# / d +- a

def test_parse():
    for num_str, components in (
        # Standard form
        ("5 * 7# / 3 - 13", (5, 7, 3, -13)),
        ("5 * 7# / 3 + 13", (5, 7, 3, 13)),
        ("5*7# / 3 - 13", (5, 7, 3, -13)),
        ("5*7# /3-13", (5, 7, 3, -13)),
        ("5    *7#/ 3 + 13", (5, 7, 3, 13)),
        # primorial d
        ("5 * 11# / 3# - 13", (5, 11, 6, -13)),
        # two component d
        ("5 * 11# / (3*2) - 13", (5, 11, 6, -13)),
        ("5 * 11# / (7*5) - 13", (5, 11, 35, -13)),
        # two component primorial d
        ("5 * 11# / (3# * 7) - 13", (5, 11, 2*3*7, -13)),
        ("5 * 11# / (2# * 5) - 13", (5, 11, 10, -13)),
        # no m
        ("11# / 5 - 13", (1, 11, 5, -13)),
        ("11# / 5 + 17", (1, 11, 5, 17)),
        ("11# / 5# + 17", (1, 11, 2*3*5, 17)),
    ):
        assert parsenumber.parse_primorial_standard_form(num_str) == components


def test_not_parse():
    for num_str in (
        "5 * 7# / 3 + - 13",
    ):
        assert parsenumber.parse_primorial_standard_form(num_str) is None


def test_parse():
    for num_str, n in (
        ("5 * 7# / 3 - 13", (5 * 7 * 5 * 2 - 13)),
        ("5 * 11# / (7*5) - 13", (5 * 11 * 3 * 2 - 13)),
        ("11# / 5# + 17", (11 * 7 + 17)),
        ("10^700 + 7", (10**700 + 7)),
        ("-10^700 + 7", None),
        ("123 * 155", None),
        ("250 / 125", None),
    ):
        assert parsenumber.parse(num_str) == n

