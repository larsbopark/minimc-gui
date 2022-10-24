pybind11_add_module(pyminimc pyminimc.cpp loaders/loaders.cpp model/cfg.cpp)
set_target_properties(pyminimc PROPERTIES OUTPUT_NAME minimc)

add_subdirectory(loaders)
add_subdirectory(model)
add_subdirectory(interpreter)

target_link_libraries(pyminimc PUBLIC minimclib)
