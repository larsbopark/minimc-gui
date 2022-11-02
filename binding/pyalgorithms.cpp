#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include "algorithms/reachability/reachability.hpp"
#include "cpa/interface.hpp"

namespace py = pybind11;

void addAlgorithmModule(py::module& m){
    auto submodule = m.def_submodule("Algorithms");

    py::class_<MiniMC::Algorithms::Reachability::Reachability>(submodule, "Reachability")
            .def(py::init<MiniMC::CPA::AnalysisTransfer>())
            .def("search",[](MiniMC::Algorithms::Reachability::Reachability& self,const MiniMC::CPA::AnalysisState& state, MiniMC::Algorithms::Reachability::GoalFunction goal){
                return self.search(state, goal);
            });

    py::enum_<MiniMC::Algorithms::Reachability::Verdict>(submodule, "Verdict")
            .value("Found", MiniMC::Algorithms::Reachability::Verdict::Found)
            .value("NotFound",MiniMC::Algorithms::Reachability::Verdict::NotFound);
}