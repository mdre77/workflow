
add_library( WorkflowType SHARED
        include/workflow/type/DataStream.hpp
        include/workflow/type/IDataStream.hpp
        include/workflow/type/IDataType.hpp
        include/workflow/type/IDataTypeVisitor.hpp
        include/workflow/type/IVariantMethods.hpp
        include/workflow/type/StructDataType.hpp
        include/workflow/type/Variant.hpp
        include/workflow/type/VariantDataType.hpp
        include/workflow/type/VariantMethodsManager.hpp
        include/workflow/type/VectorDataType.hpp
        src/DataStream.cpp
        src/IDataStream.cpp
        src/IDataType.cpp
        src/IDataTypeVisitor.cpp
        src/IVariantMethods.cpp
        src/StructDataType.cpp
        src/Variant.cpp
        src/VariantDataType.cpp
        src/VariantMethodsManager.cpp
        src/VectorDataType.cpp
        )

target_include_directories( WorkflowType
        PUBLIC
            include
            src/include
        )
target_link_libraries( WorkflowType
        PUBLIC
            WorkflowUtils
            Boost::boost
        )

add_subdirectory(test)