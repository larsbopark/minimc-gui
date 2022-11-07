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
    }, "Returns a builder on index i, the current version does only support Concrete CPA");



    py::class_<MiniMC::CPA::AnalysisBuilder>(submodule, "AnalysisBuilder")
            .def("makeTranFsfer", &MiniMC::CPA::AnalysisBuilder::makeTransfer)
            .def("makeInitialState", &MiniMC::CPA::AnalysisBuilder::makeInitialState);

    py::class_<MiniMC::CPA::InitialiseDescr>(submodule, "IntialiseDescr")
            .def(py::init<std::vector<MiniMC::Model::Function_ptr>, MiniMC::Model::HeapLayout, MiniMC::Model::InstructionStream, const MiniMC::Model::Program &>())
            .def("FromProgram", [](MiniMC::Model::Program& prgm){
                return MiniMC::CPA::InitialiseDescr{prgm.getEntryPoints(), prgm.getHeapLayout(),prgm.getInitialiser(), prgm};
            });

    py::class_<MiniMC::CPA::AnalysisTransfer>(submodule, "AnalysisTransfer", "The transfer executes an given edge")
            .def("Transfer", [](MiniMC::CPA::AnalysisTransfer& self,MiniMC::CPA::AnalysisState state,  MiniMC::Model::Edge* edge) {
                MiniMC::CPA::AnalysisState newstate;
                MiniMC::proc_t proc{0};
                self.Transfer(state, edge, proc, newstate);
                return newstate;
            }, "From a given state execute the edge on this state and return the new state")
            .def("Transfer", [](MiniMC::CPA::AnalysisTransfer& self,MiniMC::CPA::AnalysisState state,  MiniMC::Model::Edge* edge, int i) {
                MiniMC::CPA::AnalysisState newstate;
                MiniMC::proc_t proc{i};
                self.Transfer(state, edge, proc, newstate);
                return newstate;
            }, "From a given state execute the edge in the given process on this state and return the new state");

    /* state */
    py::enum_<MiniMC::CPA::Solver::Feasibility>(submodule, "Feasibility")
            .value("Feasible", MiniMC::CPA::Solver::Feasibility::Feasible)
            .value("Infreasible", MiniMC::CPA::Solver::Feasibility::Infeasible)
            .value("Unknown", MiniMC::CPA::Solver::Feasibility::Unknown);

    py::class_<MiniMC::CPA::Solver, std::shared_ptr<MiniMC::CPA::Solver>>(submodule, "Solver")
            .def("isFeasible", &MiniMC::CPA::Solver::isFeasible)
            .def("evaluate", &MiniMC::CPA::Solver::evaluate, "Given a QueryExpr evaluate it");

    py::class_<MiniMC::CPA::LocationInfo>(submodule, "LocationInfo")
            .def("assertViolated", &MiniMC::CPA::LocationInfo::assertViolated)
            .def("hasLocationAttribute", &MiniMC::CPA::LocationInfo::hasLocationAttribute)
            .def("nbOfProcesses", &MiniMC::CPA::LocationInfo::nbOfProcesses)
            .def("getLocation", &MiniMC::CPA::LocationInfo::getLocation);

    py::class_<MiniMC::CPA::CommonState> commonstate(submodule, "CommonState");
    commonstate.def("hash", [](MiniMC::CPA::CommonState& self){
        return (uint64_t) self.hash();
    });


    py::class_<MiniMC::CPA::CFAState, std::shared_ptr<MiniMC::CPA::CFAState>>(submodule,"CFAState", commonstate)
            .def("getLocationState", &MiniMC::CPA::CFAState::getLocationState, py::return_value_policy::reference);

    py::class_<MiniMC::CPA::DataState, std::shared_ptr<MiniMC::CPA::DataState>>(submodule, "DataState", commonstate)
            .def("getConcretizer", &MiniMC::CPA::DataState::getConcretizer)
            .def("getBuilder", &MiniMC::CPA::DataState::getBuilder);

    py::class_<MiniMC::CPA::AnalysisState>(submodule,"AnalysisState")
            .def("__str__", [](MiniMC::CPA::AnalysisState state){
                std::stringstream ss;
                ss << state;
                return ss.str();
            }, "Converting a State to its string representation")
            .def("getCFAState", &MiniMC::CPA::AnalysisState::getCFAState, "Retrieving the CFA state")
            .def("getDataState", &MiniMC::CPA::AnalysisState::getDataState, "Retrieve data state at index i");

    /* query */
    py::class_<MiniMC::CPA::QueryExpr, std::unique_ptr<MiniMC::CPA::QueryExpr>>(submodule,"QueryExpr");

    py::class_<MiniMC::CPA::QueryBuilder>(submodule, "QueryBuilder")
            .def("buildValue", &MiniMC::CPA::QueryBuilder::buildValue, "Given a process id and value a QueryExpr is returned");

    py::class_<MiniMC::CPA::QueryEvaluator>(submodule, "QueryEvaluator")
            .def("evaluate", &MiniMC::CPA::QueryEvaluator::evaluate, "Given a QueryExpr evaluate it");
}