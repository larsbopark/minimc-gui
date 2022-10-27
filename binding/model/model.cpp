#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "model/cfg.hpp"
#include "model/controller.hpp"
#include "model/instructions.hpp"
#include "loaders/loader.hpp"


namespace py = pybind11;

void addModelModule(py::module& m){
    auto submodule = m.def_submodule("Model");


    py::class_<MiniMC::Model::Program>(submodule,"Program")
            .def(py::init<MiniMC::Model::TypeFactory_ptr&, MiniMC::Model::ConstantFactory_ptr&>());

    m.def("Controller", [](MiniMC::Loaders::LoadResult& loadresult){
        MiniMC::Model::Controller control(*loadresult.program,loadresult.entrycreator);
        return control;
    });

    py::class_<MiniMC::Model::Controller>(submodule,"Controller")
    .def("typecheck", &MiniMC::Model::Controller::typecheck)
    .def("structuralcheck", &MiniMC::Model::Controller::structuralcheck)
    .def("lowerPhi", &MiniMC::Model::Controller::lowerPhi)
    //.def("makeMemNonDet", &MiniMC::Model::Controller::makeMemNonDet)
    .def("createAssertViolateLocations", &MiniMC::Model::Controller::createAssertViolateLocations)
    .def("inlineFunctions", &MiniMC::Model::Controller::inlineFunctions)
    .def("unrollLoops", &MiniMC::Model::Controller::unrollLoops)
    .def("boolCasts", &MiniMC::Model::Controller::boolCasts)
    //.def("splitAtCMP", &MiniMC::Model::Controller::splitAtCMP)
    .def("expandNonDeterministic", &MiniMC::Model::Controller::expandNonDeterministic)
    //.def("onlyOneMemAccessPerEdge", &MiniMC::Model::Controller::onlyOneMemAccessPerEdge)
    .def("addEntryPoint", &MiniMC::Model::Controller::addEntryPoint)
    .def("getProgram", &MiniMC::Model::Controller::getProgram);

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


}