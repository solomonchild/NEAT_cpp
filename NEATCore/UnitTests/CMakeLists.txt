PROJECT(UnitTests)

FILE(GLOB SOURCES
    "Src/*.cpp"
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES}
    ${NEATCore_sources}/Genome.cpp
    ${NEATCore_sources}/Gene.cpp
    ${NEATCore_sources}/RandomGenerator.cpp
    ${NEATCore_sources}/Environment.cpp
    ${NEATCore_sources}/Parameters.cpp
    ${NEATCore_sources}/Species.cpp
    ${NEATCore_sources}/ConfigParser.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest)
