cmake_minimum_required(VERSION 3.13.5)

project(RefurekuTests)

###########################################
#		Configure the tests
###########################################

set(RefurekuTestsTarget RefurekuTests)
add_executable(${RefurekuTestsTarget}
					ExampleClass.cpp

					RefurekuTests.cpp
				)

# Link to RefurekuLibrary
target_link_libraries(${RefurekuTestsTarget} PRIVATE ${RefurekuLibraryTarget})

# Create the command to run RefurekuGenerator
set(RefurekuGeneratorPath ${CMAKE_BINARY_DIR}/Refureku/Generator/Bin/${CMAKE_BUILD_TYPE}/${CMAKE_CXX_COMPILER_ARCHITECTURE_ID})
set(RefurekuGeneratorExeName RefurekuGenerator_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}_${CMAKE_BUILD_TYPE})
set(RunTestGeneratorTarget RunRefurekuTestGenerator)

add_custom_target(${RunTestGeneratorTarget}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    COMMAND ${RefurekuGeneratorPath}/${RefurekuGeneratorExeName})

# Run the RefurekuGenerator BEFORE building the project to refresh generated files
add_dependencies(${RefurekuTestsTarget} ${RunTestGeneratorTarget})

# Add include directories
target_include_directories(${RefurekuTestsTarget} PRIVATE Include)

add_test(NAME ${RefurekuTestsTarget} COMMAND ${RefurekuTestsTarget})