cmake_minimum_required(VERSION 2.6)

set(HAS_MARSTD2004 1 PARENT_SCOPE)
message("setting HAS_MARSTD2004")

# --- dependencies ---

add_subdirectory(../marstd2 marstd2004)

add_subdirectory(../../../framework/framework framework)

#

project(marstd2004-examples)

# --- marstd2004-examples ---

set(source_csg3d CCsg3D/main.cpp)
set(source_geomcompiler CGeomCompiler/main.cpp)

add_executable(CCsg3D ${source_csg3d})
add_executable(CGeomCompiler ${source_geomcompiler})

set(libs marstd2004 framework ${FRAMEWORK_LIBRARIES})

target_link_libraries(CCsg3D ${libs})
target_link_libraries(CGeomCompiler ${libs})
