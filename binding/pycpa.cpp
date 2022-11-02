#include <pybind11/pybind11.h>

#include "cpa/interface.hpp"
#include "cpa/state.hpp"
#include "cpa/concrete.hpp"
#include "cpa/location.hpp"
#include "cpa/pathformula.hpp"

namespace py = pybind11;

void addCPAModule(py::module& m) {
    auto submodule = m.def_submodule("CPA");

    submodule.def("makeBuilder", [](int i) {
        MiniMC::CPA::AnalysisBuilder builder{std::make_shared<MiniMC::CPA::Location::CPA>()};
        switch (i) {
            case 3:
                //builder.addDataCPA(std::make_shared<MiniMC::CPA::PathFormula::CPA>(selsmt));
                break;
            case 2:
                builder.addDataCPA(std::make_shared<MiniMC::CPA::Concrete::CPA>());
                break;
        }
        return builder;
    });

    py::class_<MiniMC::CPA::AnalysisState>(submodule,"AnalysisState")
            .def("__str__", [](MiniMC::CPA::AnalysisState state){
                std::stringstream ss;
                ss << state;
                return ss.str();
            }, "Converting a State to its string representation")
            .def("getCFAState", &MiniMC::CPA::AnalysisState::getCFAState);

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

    py::class_<MiniMC::CPA::CFAState, std::shared_ptr<MiniMC::CPA::CFAState>>(submodule,"CFAState")
            .def("getLocationState", &MiniMC::CPA::CFAState::getLocationState, py::return_value_policy::reference);

    py::class_<MiniMC::CPA::LocationInfo>(submodule, "LocationInfo")
            .def("assertViolated", &MiniMC::CPA::LocationInfo::assertViolated);


}