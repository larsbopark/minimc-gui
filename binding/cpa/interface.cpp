#include <pybind11/pybind11.h>

#include "cpa/interface.hpp"
#include "cpa/state.hpp"
#include "cpa/concrete.hpp"
#include "cpa/location.hpp"

namespace py = pybind11;

void addCPAModule(py::module& m) {
    auto submodule = m.def_submodule("CPA");

    submodule.def("makeBuilder", []() {
        MiniMC::CPA::AnalysisBuilder builder{std::make_shared<MiniMC::CPA::Location::CPA>()};

        builder.addDataCPA(std::make_shared<MiniMC::CPA::Concrete::CPA>());
        return builder;
    });

    py::class_<MiniMC::CPA::AnalysisState>(submodule,"AnalysisState");

    py::class_<MiniMC::CPA::AnalysisBuilder>(submodule, "AnalysisBuilder")
            .def("makeTransfer", &MiniMC::CPA::AnalysisBuilder::makeTransfer)
            .def("makeInitialState", &MiniMC::CPA::AnalysisBuilder::makeInitialState);

    py::class_<MiniMC::CPA::InitialiseDescr>(submodule, "IntialiseDescr")
            .def(py::init<std::vector<MiniMC::Model::Function_ptr>, MiniMC::Model::HeapLayout, MiniMC::Model::InstructionStream, const MiniMC::Model::Program &>())
            .def("FromProgram", [](MiniMC::Model::Program& prgm){
                return MiniMC::CPA::InitialiseDescr{prgm.getEntryPoints(), prgm.getHeapLayout(),prgm.getInitialiser(), prgm};
            });

    py::class_<MiniMC::CPA::AnalysisTransfer>(submodule, "AnalysisTransfer")
            .def("Transfer", &MiniMC::CPA::AnalysisTransfer::Transfer);

}