pybind11_add_module(pyminimc
        pyminimc.cpp
        pyloader.cpp
        pymodel.cpp
        pyinterpreter.cpp
        pycpa.cpp
        pyalgorithms.cpp
        )
set_target_properties(pyminimc PROPERTIES OUTPUT_NAME minimc)

target_link_libraries(pyminimc PUBLIC minimclib interpreter)