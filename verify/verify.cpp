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
#include "sieve_util.hpp"

#include <gmp.h>

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

const char doc_sieve_limit[] = R"EOF(
    Determine a reasonable max prime for sieve_interval

    Takes the same parameters (N, distance) and returns a good max_prime
)EOF";


void set_mpz_from_int_str(mpz_t *n, PyObject* n_str) {
    // Probably works for both int & str input.
    PyObject *s = PyObject_Str(n_str);
    //printf("Hi '%s'\n", s);
    //mpz_set_str(n, s, 10);

    Py_XDECREF(s);
}

PyObject*
sieve_interval(PyObject *self, PyObject *args)
{
    PyObject *start;
    int gap;
    int max_prime;

    if (!PyArg_ParseTuple(args, "Oii", &start, &gap, &max_prime))
        return NULL;

    if (max_prime <= 10) {
        return PyErr_Format(PyExc_ValueError, "bad max_prime(%d)", max_prime);
    }

    if (gap < 2) {
        return PyErr_Format(PyExc_ValueError, "bad gap(%d)", gap);
    }


    return PyLong_FromLong(gap + max_prime);
}


PyObject*
sieve_limit(PyObject *self, PyObject *args)
{
    double n_bits;
    int gap;

    if (!PyArg_ParseTuple(args, "di", &n_bits, &gap))
        return NULL;

    if (n_bits < 1 || n_bits > 100000) {
        return PyErr_Format(PyExc_ValueError, "bad n_bits(%d)", n_bits);
    }

    if (gap < 2) {
        return PyErr_Format(PyExc_ValueError, "bad gap(%d)", gap);
    }



    return PyLong_FromLong(sieve_util::calculate_sievelimit(n_bits, gap));
}
