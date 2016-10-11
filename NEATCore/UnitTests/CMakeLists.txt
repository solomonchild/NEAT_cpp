PROJECT(UnitTests)

FILE(GLOB SOURCES
    "Src/*.cpp"
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES}
    ${NEATCore_sources}/Genome.cpp
    ${NEATCore_sources}/Gene.cpp
    ${NEATCore_sources}/RandomGenerator.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest)
