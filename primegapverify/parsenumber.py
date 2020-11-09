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

import re
import gmpy2

# Combination of logic used in prime-gap project as well as
# prime-gap-list and prime-gap-record-server

# Standard form         | m * P# / d +- a
MPDA_RE_1 = re.compile(r"^(\d+)\*\(?(\d+)#\)?/(\d+)([+-]\d+)$")
# primorial d           | m * P# / d# +- a
MPDA_RE_2 = re.compile(r"^(\d+)\*(\d+)#/(\d+)#([+-]\d+)$")
# two component d       | m * P# / (d1 * d2) +- a
MPDDA_RE_1 = re.compile(r"^(\d+)\*(\d+)#/\((\d+)\*(\d+)\)([+-]\d+)$")
# two comp primorial d  | m * P# / (d# * d2) +- a
MPDDA_RE_2 = re.compile(r"^(\d+)\*(\d+)#/\((\d+)#\*(\d+)\)([+-]\d+)$")
# No m                  | P# / d +- a
PDA_RE_1 = re.compile(r"^\(?(\d+)#\)?/(\d+)([+-]\d+)$")
# No m, primorial d     | P# / d# +- a
PDA_RE_2 = re.compile(r"^\(?(\d+)#\)?/(\d+)#([+-]\d+)$")
# No m, no d            | P# +- a
PA_RE_1 = re.compile(r"^(\d+)#([+-]\d+)$")

# power form            | b ^ p +- a
POWER_BPA_RE_1 = re.compile(r"^(\d+)\^(\d+)([+-]\d+)$")


def primorial(k):
    # TODO assert k is actually prime
    return int(gmpy2.primorial(k))


def parse(num_str):
    # Remove all spaces
    num_str = re.sub(r"\s+", "", num_str)

    if num_str.isdigit():
        return int(num_str)

    match = parse_primorial_standard_form(num_str)
    if match is not None:
        m, p, d, a = match
        P = primorial(p)
        K, rem = divmod(P, d)
        if rem != 0:
            return None
        return m * K + a

    num_match = POWER_BPA_RE_1.match(num_str)
    if num_match:
        b, p, a = map(int, num_match.groups())
        if b <= 0:
            # Order of operations is hard
            return None
        return b ** p + a

    return None


def parse_primorial_standard_form(num_str):
    '''Return (m, P, d, a) => m * P#/d + a, (with a < 0)'''

    # Remove all spaces
    num_str = re.sub(r"\s+", "", num_str)

    # The most cannonical form IMHO
    match = MPDA_RE_1.match(num_str)
    if match:
        m, p, d, a = map(int, match.groups())
        return (m, p, d, a)

    match = MPDA_RE_2.match(num_str)
    if match:
        m, p, dp, a = map(int, match.groups())
        D = primorial(dp)
        return (m, p, D, a)

    match = MPDDA_RE_1.match(num_str)
    if match:
        m, p, d1, d2, a = map(int, match.groups())
        D = d1 * d2
        return (m, p, D, a)

    match = MPDDA_RE_2.match(num_str)
    if match:
        m, p, d1, d2, a = map(int, match.groups())
        D = primorial(d1) * d2
        return (m, p, D, a)

    match = PDA_RE_1.match(num_str)
    if match:
        p, d, a = map(int, match.groups())
        return (1, p, d, a)

    match = PDA_RE_2.match(num_str)
    if match:
        p, dp, a = map(int, match.groups())
        D = primorial(dp)
        return (1, p, D, a)

    match = PA_RE_1.match(num_str)
    if match:
        p, a = map(int, match.groups())
        return (1, p, 1, a)

    return None
