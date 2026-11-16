# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles\\RobusUI_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RobusUI_autogen.dir\\ParseCache.txt"
  "RobusUI_autogen"
  )
endif()
