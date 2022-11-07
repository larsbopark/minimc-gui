#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "loaders/loader.hpp"

namespace py = pybind11;

void addLoaderModule(py::module& m){
    auto submodule = m.def_submodule("Loaders");

    submodule.def("getLoader",[](const int i){
        MiniMC::Model::TypeFactory_ptr tfac =  std::shared_ptr<MiniMC::Model::TypeFactory> (new MiniMC::Model::TypeFactory64 ());

        MiniMC::Model::ConstantFactory_ptr cfac = std::shared_ptr<MiniMC::Model::ConstantFactory> (new MiniMC::Model::ConstantFactory64 (tfac));

        return MiniMC::Loaders::getLoaders ().at(i)->makeLoader(tfac,cfac);
    });

    py::class_<MiniMC::Loaders::Loader>(submodule,"Loader")
            .def("loadFromFile", &MiniMC::Loaders::Loader::loadFromFile)
            .def("loadFromString",&MiniMC::Loaders::Loader::loadFromString);

    py::class_<MiniMC::Loaders::LoadResult>(submodule, "LoadResult", "The result of running a Loader. Containing a MiniMC Program and a entrycreator")
            .def_readwrite("program", &MiniMC::Loaders::LoadResult::program)
            .def_readwrite("entrycreator",&MiniMC::Loaders::LoadResult::entrycreator);
}