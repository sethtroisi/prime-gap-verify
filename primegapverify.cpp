#include "primegapverify.hpp"

#include <Python.h>

PyObject*
spam_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    if (sts < 0) {
        PyErr_SetString(PyExc_RuntimeError, "System 2command failed");
        return NULL;
    }
    return PyLong_FromLong(sts);
}
