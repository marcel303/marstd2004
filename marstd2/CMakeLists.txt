cmake_minimum_required(VERSION 2.6)

set(HAS_MARSTD2004 1 PARENT_SCOPE)
message("setting HAS_MARSTD2004")

# --- dependencies ---

#

project(marstd2004)

# --- marstd2004 ---

file(GLOB_RECURSE other RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.*")
set_source_files_properties(${other} PROPERTIES HEADER_FILE_ONLY TRUE)

set(source marstd.cpp)
set_source_files_properties(${source} PROPERTIES HEADER_FILE_ONLY FALSE)

source_group("sources" FILES ${source} ${other})

add_library(marstd2004 ${source} ${other})
