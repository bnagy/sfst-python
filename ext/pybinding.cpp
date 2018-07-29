#include <Python.h>
#include "structmember.h"
#include "patchlevel.h" // To get version of current Python.

// The Python bindings, all in one file.

#include "fst.h"

// Python3 / Python2 compatibility macros.
#if PY_MAJOR_VERSION >= 3
  #define MOD_ERROR_VAL NULL
  #define MOD_SUCCESS_VAL(val) val
  #define MOD_INIT(name) PyMODINIT_FUNC PyInit_##name(void)
  #define MOD_DEF(ob, name, doc, methods) \
          static struct PyModuleDef moduledef = { \
            PyModuleDef_HEAD_INIT, name, doc, -1, methods, }; \
          ob = PyModule_Create(&moduledef);
#else
  #define MOD_ERROR_VAL
  #define MOD_SUCCESS_VAL(val)
  #define MOD_INIT(name) PyMODINIT_FUNC init##name(void)
  #define MOD_DEF(ob, name, doc, methods) \
          ob = Py_InitModule3(name, methods, doc);
#endif

bool PyObjectToString(PyObject* object, char* result) {
#if PY_MAJOR_VERSION == 2
  if (!PyString_Check(object)) {
    return false;
  }
  result = PyString_AsString(object);
#elif PY_MAJOR_VERSION == 3
  if (!PyUnicode_Check(object)) {
    return false;
  }
  result = PyUnicode_AsUTF8(object);
#else
  #error "Unsupported Python version"
#endif 
  if (result == NULL) {
    return false;
  }
  return true;
}

static PyObject* sfst_error;

typedef struct PyTransducer {
  PyObject_HEAD
  SFST::Transducer* transducer_;
} PyTransducer;

static PyObject* PyTransducer_new(PyTypeObject *type, PyObject* args, PyObject *kwds) {
  PyTransducer* self;
  self = (PyTransducer*) type->tp_alloc(type, 0);
  return (PyObject*) self;
}

static void PyTransducer_dealloc(PyTransducer* self) {
  if (self->transducer_ != NULL) {
    self->transducer_->clear();
  }
  delete self->transducer_;
  Py_TYPE(self)->tp_free((PyObject*)self);
}

static int PyTransducer_init(PyTransducer* self, PyObject* args, PyObject *kwds) {

  static char* kwlist[] = { (char*)"transducer", NULL };
  char* fn;
  FILE* fh;
  SFST::Transducer* t;

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, &fn)) {
    PyErr_SetString(sfst_error, "Expected string argument");
    return (int)NULL;
  }
  if ((fh = fopen((char *)fn,"rb")) == NULL) {
    PyErr_SetString(sfst_error, "Cannot open transducer file");
    return (int)NULL;
  }

  t = new SFST::Transducer(fh);
  fclose(fh);
  self->transducer_ = t;
  return 0;
}

static PyObject* PyTransducer__analyze_string(PyTransducer* self, PyObject* args, PyObject *kwds) {

  static char* keywords[] = {(char*)"s", (char*)"fd", NULL};

  char* s;
  int fd;
  FILE* fh;
  bool ok;

  if (!PyArg_ParseTupleAndKeywords(
    args,
    kwds,
    "si",
    keywords,
    &s,
    &fd)
    )
  {
    PyErr_SetString(sfst_error, "Failed to parse arguments");
    return NULL;
  }

  if ((fh = fdopen(fd,"w")) == NULL) {
    PyErr_SetString(sfst_error, "Cannot open output handle");
    return NULL;
  }

  ok = self->transducer_->analyze_string(s, fh, true);
  fclose(fh);
  if (!ok) {
    PyErr_SetString(sfst_error, "Analysis failed");
    return NULL;
  }

  Py_RETURN_NONE;

}

static PyObject* PyTransducer__version(PyTransducer* self) {
  return Py_BuildValue("s", SFSTVersion);
}

// No externally accessible members.
static PyMemberDef PyTransducer_members[] = {
    {NULL},
};

static PyMethodDef PyTransducer_methods[] = {
    { "version", (PyCFunction)PyTransducer__version, METH_VARARGS, NULL },
    { "analyze_string", (PyCFunction)PyTransducer__analyze_string, METH_VARARGS | METH_KEYWORDS, NULL },
    { NULL, NULL, 0, NULL },
};

static PyTypeObject PyTransducerType = []() -> PyTypeObject  {
  PyTypeObject type = {PyObject_HEAD_INIT(NULL) 0};
  type.tp_name = "sfst.Transducer";
  type.tp_basicsize = sizeof(PyTransducer);
  type.tp_dealloc = (destructor)PyTransducer_dealloc;
  type.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE;
  type.tp_doc = "A very limited interface to SFST, a library for NLP";
  type.tp_methods = PyTransducer_methods;
  type.tp_members = PyTransducer_members;
  type.tp_init = (initproc)PyTransducer_init;
  type.tp_new = &PyTransducer_new;
  return type;
}();

MOD_INIT(sfst) {
  PyObject *module;

  if (PyType_Ready(&PyTransducerType)) {
    return MOD_ERROR_VAL;
  }

  MOD_DEF(module, "sfst", "SFST Python Bindings", PyTransducer_methods);

  if (module == NULL) {
    return MOD_ERROR_VAL;
  }

  sfst_error = PyErr_NewException((char*)"sfst.error", NULL, NULL);
  Py_INCREF(sfst_error);
  PyModule_AddObject(module, "error", sfst_error);

  Py_INCREF(&PyTransducerType);
  PyModule_AddObject(module, "Transducer", (PyObject*)&PyTransducerType);
 
  return MOD_SUCCESS_VAL(module);
}