#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

#include "model/cfg.hpp"
#include "model/controller.hpp"
#include "model/instructions.hpp"
#include "loaders/loader.hpp"
#include "model/output.hpp"


namespace py = pybind11;

void addModelModule(py::module& m){
    auto submodule = m.def_submodule("Model");


    py::class_<MiniMC::Model::Program,std::shared_ptr<MiniMC::Model::Program>>(submodule,"Program",
            "A MiniMC Program"
            )
            .def(py::init<MiniMC::Model::TypeFactory_ptr&, MiniMC::Model::ConstantFactory_ptr&>(),
                    "Constructing an instance of program from a TypeFactory and ConstantFactory")
            .def("__str__",[](MiniMC::Model::Program& program){
                std::stringstream ss;
                MiniMC::Model::writeProgram (ss,program);
                return ss.str();
            },
                 "Get the MiniMC program as a string");

    m.def("Controller", [](MiniMC::Loaders::LoadResult& loadresult){
        MiniMC::Model::Controller control(*loadresult.program,loadresult.entrycreator);
        return control;
    },
          "Instantiating a Controller object from a LoadResult");

    py::class_<MiniMC::Model::Controller>(submodule,"Controller", "Helper class for running modification on a MiniMC Program")
    .def("typecheck", &MiniMC::Model::Controller::typecheck, "Typecheck the program")
    .def("structuralcheck", &MiniMC::Model::Controller::structuralcheck, "Run structural check of the CFG")
    .def("lowerPhi", &MiniMC::Model::Controller::lowerPhi)
    //.def("makeMemNonDet", &MiniMC::Model::Controller::makeMemNonDet)
    .def("createAssertViolateLocations", &MiniMC::Model::Controller::createAssertViolateLocations, "For each MiniMC::Model::InstructionCode::Assert\n"
                                                                                                   "instruction, a special Location with \n"
                                                                                                   "MiniMC::Location::Attributes::AssertViolated set is inserted.\n"
                                                                                                   "The edge containing the Instruction is split, such that the\n"
                                                                                                   "one edge goes to the current successor Location while one goes newly\n"
                                                                                                   "constructed Location.")
    .def("inlineFunctions", &MiniMC::Model::Controller::inlineFunctions)
    .def("unrollLoops", &MiniMC::Model::Controller::unrollLoops)
    .def("boolCasts", &MiniMC::Model::Controller::boolCasts)
    //.def("splitAtCMP", &MiniMC::Model::Controller::splitAtCMP)
    .def("expandNonDeterministic", &MiniMC::Model::Controller::expandNonDeterministic)
    //.def("onlyOneMemAccessPerEdge", &MiniMC::Model::Controller::onlyOneMemAccessPerEdge)
    .def("addEntryPoint", &MiniMC::Model::Controller::addEntryPoint)
    .def("getProgram", &MiniMC::Model::Controller::getProgram, "Get MiniMC Program");

    py::class_<MiniMC::Model::InstructionStream>(submodule,"InstructionStream")
#define X(OP) \
        .def("addInstruction", &MiniMC::Model::InstructionStream::addInstruction<MiniMC::Model::InstructionCode::OP>)
        OPERATIONS
#undef X
        .def(py::init())
        .def(py::init<bool>())
        .def(py::init<const std::vector<MiniMC::Model::Instruction>&,bool>())
        .def("assign", &MiniMC::Model::InstructionStream::operator=)
        .def("output",&MiniMC::Model::InstructionStream::output);

    /* edge */
    py::class_<MiniMC::Model::Edge,std::shared_ptr<MiniMC::Model::Edge>>(submodule, "Edge")
        .def(py::init<MiniMC::Model::Location_ptr,MiniMC::Model::Location_ptr>())
        .def("getInstructions", [](MiniMC::Model::Edge& self){
            MiniMC::Model::InstructionStream is = self.getInstructions();
            return is;
        })
        .def("getTo", &MiniMC::Model::Edge::getTo)
        .def("getFrom",&MiniMC::Model::Edge::getFrom)
        .def("setTo", &MiniMC::Model::Edge::setTo);

    /* cfg */
    py::class_<MiniMC::Model::CFA>(submodule, "CFA")
        .def(py::init())
        .def("makeLocation", &MiniMC::Model::CFA::makeLocation)
        .def("makeEdge", &MiniMC::Model::CFA::makeEdge)
        .def("getInitialLocation", &MiniMC::Model::CFA::getInitialLocation)
        .def("deleteEdge", &MiniMC::Model::CFA::deleteEdge)
        .def("deleteLocation", &MiniMC::Model::CFA::deleteLocation);

    py::class_<MiniMC::Model::Function, std::shared_ptr<MiniMC::Model::Function>>(submodule, "Function")
    .def("getSymbol", &MiniMC::Model::Function::getSymbol)
    .def("getParameters", &MiniMC::Model::Function::getParameters)
    .def("getRegisterDescr", &MiniMC::Model::Function::getRegisterDescr)
    .def("getRegisterStackDescr", &MiniMC::Model::Function::getRegisterStackDescr)
    .def("getID", &MiniMC::Model::Function::getID)
    .def("getReturnType", &MiniMC::Model::Function::getReturnType)
    .def("getPrgm", &MiniMC::Model::Function::getPrgm);

    /* location */
    py::class_<MiniMC::Model::Location, std::shared_ptr<MiniMC::Model::Location>>(submodule, "Location")
        .def("addEdge", &MiniMC::Model::Location::addEdge)
        .def("addIncomingEdge", &MiniMC::Model::Location::addIncomingEdge)
        .def("getOutGoingEdges", [](MiniMC::Model::Location& self){
            std::vector<MiniMC::Model::Edge*> edges;
            std::for_each(self.ebegin(), self.eend(), [&edges](auto& e) { edges.push_back(e); });
            return edges;
        })
        .def("getIncomingEdges", [](MiniMC::Model::Location& self){
            std::vector<MiniMC::Model::Edge*> edges;
            std::for_each(self.iebegin(), self.ieend(), [&edges](auto& e) { edges.push_back(e); });
            return edges;
        })
        .def("getCFG", &MiniMC::Model::Location::getCFG);

    /* variables */
    py::class_<MiniMC::Model::Value> value(submodule, "Value");
    value.def("type_t", &MiniMC::Model::Value::type_t)
        .def("getType", &MiniMC::Model::Value::getType)
        .def("setType", &MiniMC::Model::Value::setType);

    py::class_<MiniMC::Model::Constant>(submodule, "Constant", value);
}