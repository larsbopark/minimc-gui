#include <pybind11/pybind11.h>

#include "interpreter/parser.hpp"
#include "cpa/interface.hpp"
#include "interpreter/statemap.hpp"

namespace py = pybind11;

void addInterpreterModule(py::module& m){
    auto submodule = m.def_submodule("Interpreter");

    py::class_<MiniMC::Interpreter::Parser>(submodule,"Parser")
            .def(py::init<MiniMC::Interpreter::StateMap*,MiniMC::CPA::AnalysisTransfer>())
            .def("__call__",[](MiniMC::Interpreter::Parser& self, const std::string& s){
                self(s);
            });

    py::class_<MiniMC::Interpreter::StateMap>(submodule,"StateMap")
            .def(py::init<MiniMC::CPA::AnalysisState>());
}

