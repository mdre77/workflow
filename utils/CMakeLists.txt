
add_library( WorkflowUtils SHARED
        include/workflow/utils/Demangle.hpp
        include/workflow/utils/Error.hpp
        include/workflow/utils/ErrorCode.hpp
        include/workflow/utils/Macros.hpp
        include/workflow/utils/OutputStreamHelpers.hpp
        src/Error.cpp
        src/ErrorCode.cpp
        src/Demangle.cpp
    )

target_include_directories( WorkflowUtils
    PUBLIC
        include
    )
target_link_libraries( WorkflowUtils
    PUBLIC
        Boost::boost
    )