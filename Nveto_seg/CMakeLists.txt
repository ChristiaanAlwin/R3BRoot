set(LIBRARY_NAME R3BNveto_seg)
set(LINKDEF Nveto_segLinkDef.h)

set(SRCS
	R3BNveto_seg.cxx
	R3BNveto_segNeutron2DPar.cxx
	R3BNveto_segGeoPar.cxx
	R3BNveto_segContFact.cxx
	R3BNveto_segMCMon.cxx
R3BNveto_segDigitizrOld.cxx
	R3BNveto_segDigiMon.cxx
	R3BNveto_segClusterMon.cxx
	R3BNveto_segVisualizer.cxx
	R3BNveto_segDigitizer.cxx
	R3BNveto_segClusterFinder.cxx
	R3BNveto_segNeutronReconstruction.cxx
	R3BNveto_segNeutronReconstructionEvaluation.cxx
	digitizing/Digitizin_segEngine.cxx
	clustering/NvetoClustringEngine.cxx
	clustering/Neutron2DCalibr.cxx
	unpack/R3BNveto_segTcalFill.cxx
	unpack/R3BNveto_segTcal.cxx
	unpack/R3BNveto_segPmt.cxx 
)
change_file_extension(*.cxx *.h HEADERS "${SRCS}")

set(DEPENDENCIES
  R3Bbase R3BTCal R3BData
)

# TODO: -Wno-ignored-qualifiers for R3Bveto_segDigiPar
# -Wno-overloaded-virtual is needed because of unavoidable Print() overloading in R3BStack
# -pthread for gtest
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wformat-security -pthread")

set(SYSTEM_INCLUDE_DIRECTORIES
	${SYSTEM_INCLUDE_DIRECTORIES}
	${BASE_INCLUDE_DIRECTORIES}
)
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(INCLUDE_DIRECTORIES
	${R3BROOT_SOURCE_DIR}/Nveto_seg
	${R3BROOT_SOURCE_DIR}/Nveto_seg/unpack
	${R3BROOT_SOURCE_DIR}/Nveto_seg/digitizing
	${R3BROOT_SOURCE_DIR}/Nveto_seg/clustering
	${R3BROOT_SOURCE_DIR}/r3bbase/
	${R3BROOT_SOURCE_DIR}/r3bdata/
${R3BROOT_SOURCE_DIR}/veto_seg
${R3BROOT_SOURCE_DIR}/r3bdata/veto_segData
	${R3BROOT_SOURCE_DIR}/r3bdata/Nveto_segData
	${R3BROOT_SOURCE_DIR}/r3bbase
	${R3BROOT_SOURCE_DIR}/tcal
)
include_directories(${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
	${ROOT_LIBRARY_DIR}
	${FAIRROOT_LIBRARY_DIR}
)
link_directories(${LINK_DIRECTORIES})

generate_library()


# Testing 
enable_testing()
Set(GTEST_ROOT ${SIMPATH})
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(testNvetoClustringEngine test/testNvetoClustringEngine.cxx)
target_link_libraries(testNvetoClustringEngine ${GTEST_BOTH_LIBRARIES})
add_test(Nveto_segNvetoClustringEngineUnitTest ${EXECUTABLE_OUTPUT_PATH}/testNvetoClustringEngine)

If (CMAKE_SYSTEM_NAME MATCHES Linux)
  Set(DEPENDENCIES ${DEPENDENCIES} rt)
EndIf (CMAKE_SYSTEM_NAME MATCHES Linux)
add_executable(testNveto_segNeutron2DPar test/testNveto_segNeutron2DPar.cxx)
target_link_libraries(testNveto_segNeutron2DPar ${GTEST_BOTH_LIBRARIES} ${ROOT_LIBRARIES} R3BNveto_seg ${DEPENDENCIES})
add_test(Nveto_segNeutron2DParUnitTest ${EXECUTABLE_OUTPUT_PATH}/testNveto_segNeutron2DPar)
