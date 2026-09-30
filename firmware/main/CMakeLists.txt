if(MYSTOP_RUN_STORAGE_TESTS)
    idf_component_register(
        INCLUDE_DIRS "."
        REQUIRES firmware)
else()
    idf_component_register(
        SRCS "main.c"
        INCLUDE_DIRS "."
        REQUIRES storage)
endif()
