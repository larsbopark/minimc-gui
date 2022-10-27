pybind11_add_module(pyminimc
        pyminimc.cpp
        loaders/loaders.cpp
        model/model.cpp
        interpreter/interpreter.cpp
        cpa/interface.cpp
        )
set_target_properties(pyminimc PROPERTIES OUTPUT_NAME minimc)

add_subdirectory(loaders)
add_subdirectory(model)
add_subdirectory(interpreter)
add_subdirectory(cpa)

target_link_libraries(pyminimc PUBLIC minimclib interpreter)