add_executable(injector main.cc)
set_target_properties(injector PROPERTIES
    DEPENDS dll)