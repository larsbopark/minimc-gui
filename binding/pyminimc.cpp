#include <pybind11/pybind11.h>
#include "support/smt.hpp"

namespace py = pybind11;

void addLoaderModule(py::module& m);
void addInterpreterModule(py::module& m);
void addCPAModule(py::module& m);
void addModelModule(py::module& m);

PYBIND11_MODULE(minimc, m){
    m.doc() = "MiniMC for Python";
    addLoaderModule(m);
    addInterpreterModule(m);
    addCPAModule(m);
    addModelModule(m);

}