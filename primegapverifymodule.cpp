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
