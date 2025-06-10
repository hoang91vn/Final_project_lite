#ifndef MATPLOTLIBCPP_H
#define MATPLOTLIBCPP_H
#include <Python.h>
#include <string>
#include <vector>
#include <map>
#include <cstdio>

namespace matplotlibcpp {
namespace detail {
inline void ensure_interpreter() {
    static bool initialized = false;
    if (!initialized) {
        Py_Initialize();
        PyRun_SimpleString("import matplotlib; matplotlib.use('Agg'); import matplotlib.pyplot as plt");
        initialized = true;
    }
}
}

inline void figure_size(int w, int h) {
    detail::ensure_interpreter();
    char cmd[128];
    std::snprintf(cmd, sizeof(cmd), "plt.figure(figsize=(%.1f,%.1f))", w/100.0, h/100.0);
    PyRun_SimpleString(cmd);
}

inline PyObject* vector_double(const std::vector<double>& v) {
    PyObject* list = PyList_New(v.size());
    for(size_t i=0;i<v.size();++i)
        PyList_SetItem(list, i, PyFloat_FromDouble(v[i]));
    return list;
}

inline PyObject* vector_string(const std::vector<std::string>& v) {
    PyObject* list = PyList_New(v.size());
    for(size_t i=0;i<v.size();++i)
        PyList_SetItem(list, i, PyUnicode_FromString(v[i].c_str()));
    return list;
}

inline void plot(const std::vector<std::string>& x,
                 const std::vector<double>& y,
                 const std::map<std::string,std::string>& opts = {}) {
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* xlist = vector_string(x);
    PyObject* ylist = vector_double(y);
    PyObject* args = PyTuple_Pack(2, xlist, ylist);
    Py_DECREF(xlist);
    Py_DECREF(ylist);
    PyObject* kwargs = PyDict_New();
    for(const auto& kv: opts)
        PyDict_SetItemString(kwargs, kv.first.c_str(), PyUnicode_FromString(kv.second.c_str()));
    PyObject* func = PyObject_GetAttrString(plt, "plot");
    PyObject* res = PyObject_Call(func, args, kwargs);
    Py_XDECREF(res);
    Py_DECREF(func);
    Py_DECREF(args);
    Py_DECREF(kwargs);
}

inline void plot(const std::vector<std::string>& x,
                 const std::vector<double>& y,
                 const std::string& fmt) {
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* xlist = vector_string(x);
    PyObject* ylist = vector_double(y);
    PyObject* res = PyObject_CallMethod(plt, "plot", "OOs", xlist, ylist, fmt.c_str());
    Py_XDECREF(res);
    Py_DECREF(xlist);
    Py_DECREF(ylist);
}

inline void xlabel(const std::string& str){
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* res = PyObject_CallMethod(plt, "xlabel", "s", str.c_str());
    Py_XDECREF(res);
}
inline void ylabel(const std::string& str){
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* res = PyObject_CallMethod(plt, "ylabel", "s", str.c_str());
    Py_XDECREF(res);
}
inline void title(const std::string& str){
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* res = PyObject_CallMethod(plt, "title", "s", str.c_str());
    Py_XDECREF(res);
}
inline void legend(){
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* res = PyObject_CallMethod(plt, "legend", "");
    Py_XDECREF(res);
}
inline void save(const std::string& str){
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* res = PyObject_CallMethod(plt, "savefig", "s", str.c_str());
    Py_XDECREF(res);
}
inline void close(){
    detail::ensure_interpreter();
    PyObject* plt = PyImport_AddModule("matplotlib.pyplot");
    PyObject* res = PyObject_CallMethod(plt, "close", "");
    Py_XDECREF(res);
}

}
#endif
