#include <pybind11/pybind11.h>

namespace py = pybind11;

void addVMModule(py::module& m){
    auto submodule = m.def_submodule("VM");

}