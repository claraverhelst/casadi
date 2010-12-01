find_package(LAPACK)
find_package(Threads)

FIND_PATH(IPOPT_INCLUDE_DIR 
coin/IpIpoptApplication.hpp
HINTS $ENV{IPOPT}/include
)

IF (IPOPT_INCLUDE_DIR)
   SET(FOUND_IPOPT_INCLUDE TRUE)
   MESSAGE(STATUS "Found IPOPT include dir: ${IPOPT_INCLUDE_DIR}")
ELSE (IPOPT_INCLUDE_DIR)
   MESSAGE(STATUS "Could not find IPOPT include dir")
ENDIF (IPOPT_INCLUDE_DIR)

FIND_LIBRARY(IPOPT_LIBRARY 
NAMES ipopt HINTS $ENV{IPOPT}/lib)

MESSAGE(STATUS "${PTHREAD_LIBRARIES}")


IF (IPOPT_LIBRARY)
   SET(FOUND_IPOPT_LIBS TRUE)
   SET(IPOPT_LIBRARIES ${IPOPT_LIBRARY} ${IPOPT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${LAPACK_LIBRARIES} ${EXTRA_LIBRARIES}) 
   MESSAGE(STATUS "Found Ipopt libs: ${IPOPT_LIBRARIES}")
ELSE (IPOPT_LIBRARY)
   MESSAGE(STATUS "Could not find Ipopt libs")
ENDIF (IPOPT_LIBRARY)

IF(FOUND_IPOPT_INCLUDE AND FOUND_IPOPT_LIBS)
  SET(FOUND_IPOPT TRUE)
ENDIF(FOUND_IPOPT_INCLUDE AND FOUND_IPOPT_LIBS)
