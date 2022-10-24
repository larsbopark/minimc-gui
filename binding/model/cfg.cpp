#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "model/cfg.hpp"

namespace py = pybind11;

void addModelModule(py::module& m){
    auto submodule = m.def_submodule("CFG");


    py::class_<MiniMC::Model::Program>(submodule,"Program")
            .def(py::init<MiniMC::Model::TypeFactory_ptr&, MiniMC::Model::ConstantFactory_ptr&>());
}