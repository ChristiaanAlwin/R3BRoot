# Create a library called "libR3BCal" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(SYSTEM_INCLUDE_DIRECTORIES
${SYSTEM_INCLUDE_DIRECTORIES}
${BASE_INCLUDE_DIRECTORIES}
)

set(INCLUDE_DIRECTORIES
#put here all directories where header files are located
${R3BROOT_SOURCE_DIR}/r3bbase
${R3BROOT_SOURCE_DIR}/passive
${R3BROOT_SOURCE_DIR}/veto_seg
${R3BROOT_SOURCE_DIR}/los
${R3BROOT_SOURCE_DIR}/r3bdata/
${R3BROOT_SOURCE_DIR}/r3bdata/caloData
${R3BROOT_SOURCE_DIR}/r3bdata/dchData
${R3BROOT_SOURCE_DIR}/r3bdata/gfiData
${R3BROOT_SOURCE_DIR}/r3bdata/mtofData
${R3BROOT_SOURCE_DIR}/r3bdata/tofData
${R3BROOT_SOURCE_DIR}/r3bdata/veto_segData
${R3BROOT_SOURCE_DIR}/r3bdata/Nveto_segData
${R3BROOT_SOURCE_DIR}/r3bdata/losData
${R3BROOT_SOURCE_DIR}/r3bdata/trackerData
${R3BROOT_SOURCE_DIR}/r3bdb/veto_segdb
${R3BROOT_SOURCE_DIR}/tcal
${R3BROOT_SOURCE_DIR}/Nveto_seg/unpack
${R3BROOT_SOURCE_DIR}/veto_seg/calibration
)

include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
)

link_directories( ${LINK_DIRECTORIES})

set(SRCS
R3Bveto_seg.cxx
R3BGeoveto_seg.cxx
R3BGeoveto_segPar.cxx
R3Bveto_segContFact.cxx
R3Bveto_segDigitizer.cxx
R3Bveto_segDigitizerQA.cxx
R3BNeutronTrak_segr_seg.cxx
R3Bveto_segDigiPar.cxx
R3BNVeto_segCluster.cxx
R3BNVeto_segClusterFinder.cxx
R3BNeutronCalib2D_seg.cxx
R3BPrimPat_seg.cxx
R3BNeutronTrak_segr2D_seg.cxx
R3BNeutHt_seg.cxx
calibration/R3BNveto_segMappedHist.cxx
calibration/R3BNveto_segMapped2CalPar.cxx
calibration/R3Bveto_segParamRead.cxx
calibration/R3BNveto_segMapped2Cal.cxx
calibration/R3BNveto_segHitHist.cxx
#calibration/R3BNveto_segCalTest.cxx
calibration/R3BNveto_segHitPar.cxx
calibration/R3BNveto_segCal2HitPar.cxx
calibration/R3Bveto_segCosmic1LSQR.cxx
calibration/R3Bveto_segCosmic1Util.cxx
calibration/R3BNveto_segParFact.cxx
calibration/R3BNveto_segCal2Hit.cxx
calibration/R3BNveto_segHitModulePar.cxx
calibration/R3BNveto_segMapped2QCalPar.cxx
calibration/R3BNveto_segQCalPar.cxx
)

# fill list of header files from list of source files
# by exchanging the file extension
CHANGE_FILE_EXTENSION(*.cxx *.h HEADERS "${SRCS}")

Set(LINKDEF veto_segLinkDef.h)
Set(LIBRARY_NAME R3Bveto_seg)
Set(DEPENDENCIES
    R3Bbase R3BData R3BPassive R3BTCal)

GENERATE_LIBRARY()

