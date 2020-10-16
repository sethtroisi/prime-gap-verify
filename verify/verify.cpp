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

#include "verify.hpp"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

const char doc_sieve_interval[] = R"EOF(
    Sieve an interval of numbers

    Parameters
    ----------
       N : start of interval
       distance : size of interval
       max_prime : remove all multiples of primes less than or equal

    Returns
    -------
       unknowns : array
           Offsets of numbers not known to be composite3
)EOF";

PyObject*
sieve_interval(PyObject *self, PyObject *args)
{
    PyObject *start;
    int gap;
    int max_prime;

    if (!PyArg_ParseTuple(args, "Oii", &start, &gap, &max_prime))
        return NULL;

    return PyLong_FromLong(gap + max_prime);
}
