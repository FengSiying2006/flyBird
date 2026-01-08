# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\flybird_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\flybird_autogen.dir\\ParseCache.txt"
  "flybird_autogen"
  )
endif()
