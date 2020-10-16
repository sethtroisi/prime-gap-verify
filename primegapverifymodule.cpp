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

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "primegapverify.hpp"


static PyMethodDef PrimeGapVerifyMethods[] = {
    {"sieve",  spam_system, METH_VARARGS, "sieve gap interval"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef prime_gap_verify_module = {
    PyModuleDef_HEAD_INIT,
    "primegapverify",   /* name of module */
    NULL,     /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    PrimeGapVerifyMethods
};

PyMODINIT_FUNC
PyInit_primegapverify(void)
{
    return PyModule_Create(&prime_gap_verify_module);
}
