#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *
xor(PyObject * self, PyObject *args)
{
	Py_buffer src1, src2;

	if (!PyArg_ParseTuple(args, "y*y*", &src1, &src2))
		return NULL;
	if (src1.len != src2.len) {
		PyErr_SetString(PyExc_Exception,
		                "arguments have different length");
		return NULL;
	}

	PyObject *obj = PyBytes_FromStringAndSize(NULL, src1.len);
	if (!obj)
		return NULL;

	unsigned char *data = (unsigned char *)PyBytes_AsString(obj);
	unsigned char *data1 = (unsigned char *)src1.buf;
	unsigned char *data2 = (unsigned char *)src2.buf;

	for (Py_ssize_t i = 0; i < src1.len; ++i)
		*data++ = *data1++ ^ *data2++;

	PyBuffer_Release(&src1);
	PyBuffer_Release(&src2);
	return obj;
}

static PyObject *
xor_bytearray_range(PyObject *self, PyObject *args)
{
	PyObject *obj;
	Py_buffer view;
	Py_ssize_t offset, objsize;

	if (!PyArg_ParseTuple(args, "Yny*", &obj, &offset, &view))
		return NULL;

	objsize = PyByteArray_Size(obj);
	if (objsize < offset || objsize - offset < view.len) {
		PyErr_SetString(PyExc_Exception, "bytearray too small");
		return NULL;
	}

	unsigned char *data1 = (unsigned char *)PyByteArray_AsString(obj);
	unsigned char *data2 = (unsigned char *)view.buf;

	data1 += offset;
	for (Py_ssize_t i = 0; i < view.len; ++i)
		*data1++ ^= *data2++;

	PyBuffer_Release(&view);
	Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
	{"xor", xor, METH_VARARGS, "xor of bytes-like objects"},
	{"xor_bytearray_range", xor_bytearray_range, METH_VARARGS,
	 "in-place xor for bytearray"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
	PyModuleDef_HEAD_INIT,
	"xor",
	"",
	-1,
	methods,
};

PyMODINIT_FUNC PyInit_xor(void)
{
	return PyModule_Create(&module);
}
