#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void addLoaderModule(py::module& m);

PYBIND11_MODULE(minimc, m){
    m.doc() = "MiniMC for Python";

    addLoaderModule(m);



}