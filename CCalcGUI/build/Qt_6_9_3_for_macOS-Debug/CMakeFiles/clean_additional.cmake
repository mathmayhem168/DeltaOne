# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CCalcGUI_autogen"
  "CMakeFiles/CCalcGUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/CCalcGUI_autogen.dir/ParseCache.txt"
  )
endif()
