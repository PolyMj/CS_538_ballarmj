cmake_minimum_required(VERSION 3.5)
project( Basic_CS_538 )

set(CMAKE_INSTALL_PREFIX .)

set(MASTER_DEPEND "~/Dependencies" CACHE PATH "Set this if necessary to your directory for dependencies.")	
set(CMAKE_PREFIX_PATH ${MASTER_DEPEND} ${CMAKE_PREFIX_PATH})

#####################################
# Require C++17
#####################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#####################################
# Find necessary libraries
#####################################

find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 3.3 REQUIRED) 
find_package(GLEW REQUIRED)	
find_package(doctest REQUIRED)

include(CTest)
include(doctest)

add_definitions(-DGLEW_STATIC)

#####################################
# Get general sources
#####################################

include_directories("src/include" ${Vulkan_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${DOCTEST_INCLUDE_DIR})

file(GLOB GENERAL_SOURCES    
    "src/lib/*.cpp"  
    "src/lib/*.hpp"  
)

#####################################
# Create executables 
# and install targets
#####################################

set(ALL_LIBRARIES ${Vulkan_LIBRARIES} ${ASSIMP_LIBRARIES} ${ASSIMP_ZLIB} glfw GLEW::glew_s doctest::doctest)
 
# RenderApp
add_executable(BasicRenderApp ${GENERAL_SOURCES} "./src/app/BasicRenderApp.cpp")
target_link_libraries(BasicRenderApp ${ALL_LIBRARIES})
install(TARGETS BasicRenderApp RUNTIME DESTINATION bin/BasicRenderApp)
install(DIRECTORY shaders/BasicRenderApp DESTINATION bin/BasicRenderApp/shaders)

# ExerciseApp
add_executable(ExerciseApp ${GENERAL_SOURCES} "./src/app/ExerciseApp.cpp")
target_link_libraries(ExerciseApp ${ALL_LIBRARIES})
install(TARGETS ExerciseApp RUNTIME DESTINATION bin/ExerciseApp)
install(DIRECTORY shaders/ExerciseApp DESTINATION bin/ExerciseApp/shaders)

# Assign01
# Uncomment these lines
add_executable(Assign01 ${GENERAL_SOURCES} "./src/app/Assign01.cpp")
target_link_libraries(Assign01 ${ALL_LIBRARIES})
install(TARGETS Assign01 RUNTIME DESTINATION bin/Assign01)
install(DIRECTORY shaders/Assign01 DESTINATION bin/Assign01/shaders)

# RenderApp
add_executable(E01RenderApp ${GENERAL_SOURCES} "./src/app/E01RenderApp.cpp")
target_link_libraries(E01RenderApp ${ALL_LIBRARIES})
install(TARGETS E01RenderApp RUNTIME DESTINATION bin/E01RenderApp)
install(DIRECTORY shaders/E01RenderApp DESTINATION bin/E01RenderApp/shaders)


# PotatoRenderApp
add_executable(PotatoRenderApp ${GENERAL_SOURCES} "./src/app/PotatoRenderApp.cpp")
target_link_libraries(PotatoRenderApp ${ALL_LIBRARIES})
install(TARGETS PotatoRenderApp RUNTIME DESTINATION bin/PotatoRenderApp)
install(DIRECTORY shaders/PotatoRenderApp DESTINATION bin/PotatoRenderApp/shaders)

# Temporary test program
add_executable(TestApp ${GENERAL_SOURCES} "./src/app/TestApp.cpp")
target_link_libraries(TestApp ${ALL_LIBRARIES})
install(TARGETS TestApp RUNTIME DESTINATION bin/TestApp)
install(DIRECTORY shaders/TestApp DESTINATION bin/TestApp/shaders)

#####################################
# TEST TARGETS
#####################################

# COMMENT THIS LINE TO DISABLE TESTING
add_definitions(-DENABLE_TESTING)
enable_testing()

add_executable(TestVec3 ${GENERAL_SOURCES} "./src/tests/TestVec3.cpp")
target_link_libraries(TestVec3 ${ALL_LIBRARIES})
set_target_properties(TestVec3 PROPERTIES COMPILE_DEFINITIONS "TEST")
doctest_discover_tests(TestVec3)

# PExercise01
# add_executable(PExercise01 ${GENERAL_SOURCES} "./src/app/PExercise01.cpp")
# target_link_libraries(PExercise01 ${ALL_LIBRARIES})
# install(TARGETS PExercise01 RUNTIME DESTINATION bin/PExercise01)
# install(DIRECTORY shaders/PExercise01 DESTINATION bin/PExercise01/shaders)


