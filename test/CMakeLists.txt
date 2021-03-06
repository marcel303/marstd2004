cmake_minimum_required(VERSION 2.6)

set(HAS_MARSTD2004 1 PARENT_SCOPE)
message("setting HAS_MARSTD2004")

# --- dependencies ---

add_subdirectory(../marstd2 marstd2004)

add_subdirectory(../../../framework/framework framework)

#

project(marstd2004-examples)

# --- marstd2004-examples ---

set(libs marstd2004 framework ${FRAMEWORK_LIBRARIES})

set(source_test main.cpp)
add_executable(test ${source_test})
target_link_libraries(test ${libs})

set(source_bsp CBsp/main.cpp)
set(source_csg2d CCsg2D/main.cpp)
set(source_csg3d CCsg3D/main.cpp)
set(source_csg3ds CCsg3Ds/main.cpp)
set(source_geomcompiler CGeomCompiler/main.cpp)
set(source_isosurface CIsosurface/main.cpp)

add_executable(CBsp ${source_bsp})
add_executable(CCsg2D ${source_csg2d})
add_executable(CCsg3D ${source_csg3d})
add_executable(CCsg3Ds ${source_csg3ds})
add_executable(CGeomCompiler ${source_geomcompiler})
add_executable(CIsosurface ${source_isosurface})

target_link_libraries(CBsp ${libs})
target_link_libraries(CCsg2D ${libs})
target_link_libraries(CCsg3D ${libs})
target_link_libraries(CCsg3Ds ${libs})
target_link_libraries(CGeomCompiler ${libs})
target_link_libraries(CIsosurface ${libs})
