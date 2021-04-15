
add_executable(test_sequencer_type
        test_sequencer_type_Variant.cpp
        test_sequencer_type_VariantMethodsManager.cpp
        test_sequencer_type_VariantDataType.cpp
        test_sequencer_type_StructDataType.cpp
        )
target_link_libraries(test_sequencer_type
        WorkflowType
        GTest::GTest
        GTest::Main
        gmock
        )
add_test(test_sequencer_type test_sequencer_type)