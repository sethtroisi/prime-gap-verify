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

#include <Python.h>

#include "verify.hpp"


static PyMethodDef VerifyMethods[] = {
    {"sieve_interval",  sieve_interval, METH_VARARGS, doc_sieve_interval},
    {"sieve_limit",  sieve_limit, METH_VARARGS, doc_sieve_limit},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef verify_module = {
    PyModuleDef_HEAD_INIT,
    "verify",   /* name of module */
    NULL,     /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    VerifyMethods
};

PyMODINIT_FUNC
PyInit_verify(void)
{
    return PyModule_Create(&verify_module);
}
