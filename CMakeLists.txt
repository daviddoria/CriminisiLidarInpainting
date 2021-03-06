cmake_minimum_required(VERSION 2.6)
PROJECT(CriminisiInpainting)

# Where to copy executables when 'make install' is run
SET(INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) #fix the dynamic linking error (RPATH) that would occur without this

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_UI(UISrcs Form.ui)
QT4_WRAP_CPP(MOCSrcs Form.h ProgressThread.h CriminisiInpainting.h)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

# Interactive
ADD_EXECUTABLE(CriminisiInpaintingInteractive
CriminisiInpainting.cpp 
CriminisiInpaintingDebugging.cpp 
Form.cxx 
Helpers.cpp 
Interactive.cpp 
InteractorStyleImageNoLevel.cxx
ProgressThread.cpp
SelfPatchCompare.cpp
${UISrcs} ${MOCSrcs})
set_target_properties(CriminisiInpaintingInteractive PROPERTIES COMPILE_FLAGS "-DINTERACTIVE")

TARGET_LINK_LIBRARIES(CriminisiInpaintingInteractive ${VTK_LIBRARIES} QVTK ${ITK_LIBRARIES} ${QT_LIBRARIES})
INSTALL( TARGETS CriminisiInpaintingInteractive RUNTIME DESTINATION ${INSTALL_DIR} )

# Non-interactive
# Should look something like this:
# ADD_EXECUTABLE(CriminisiInpainting 
# CriminisiInpaintingNonInteractive.cpp
# CriminisiInpainting.cpp 
# CriminisiInpaintingDebugging.cpp 
# SelfPatchCompare.cpp 
# Helpers.cpp)
# TARGET_LINK_LIBRARIES(CriminisiInpainting ${VTK_LIBRARIES} ${ITK_LIBRARIES})
# INSTALL( TARGETS CriminisiInpainting RUNTIME DESTINATION ${INSTALL_DIR} )

# Until we get this question answered, we must build everything
# ADD_EXECUTABLE(CriminisiInpainting 
# CriminisiInpaintingNonInteractive.cpp
# CriminisiInpainting.cpp 
# CriminisiInpaintingDebugging.cpp 
# SelfPatchCompare.cpp 
# Helpers.cpp
# 
# ProgressThread.cpp
# InteractorStyleImageNoLevel.cxx
# Form.cxx 
# ${UISrcs} ${MOCSrcs})
# TARGET_LINK_LIBRARIES(CriminisiInpainting ${VTK_LIBRARIES} ${ITK_LIBRARIES} QVTK)
# INSTALL( TARGETS CriminisiInpainting RUNTIME DESTINATION ${INSTALL_DIR} )




# ADD_EXECUTABLE(TestPatchDifference TestPatchDifference.cpp Helpers.cpp)
# TARGET_LINK_LIBRARIES(TestPatchDifference ${VTK_LIBRARIES} ${ITK_LIBRARIES})

# ADD_EXECUTABLE(DemoErroneousGradient DemoErroneousGradient.cpp Helpers.cpp )
# TARGET_LINK_LIBRARIES(DemoErroneousGradient ${ITK_LIBRARIES})

# ADD_EXECUTABLE(DemoBoundaryNormals DemoBoundaryNormals.cpp Helpers.cpp )
# TARGET_LINK_LIBRARIES(DemoBoundaryNormals ${ITK_LIBRARIES})

# ADD_EXECUTABLE(DemoInnerOuterBoundary DemoInnerOuterBoundary.cpp Helpers.cpp )
# TARGET_LINK_LIBRARIES(DemoInnerOuterBoundary ${ITK_LIBRARIES})

# ADD_EXECUTABLE(PatchImageDifferenceExample PatchImageDifferenceExample.cpp SelfPatchMatch.h Helpers.h  Helpers.cpp)
# TARGET_LINK_LIBRARIES(PatchImageDifferenceExample ${VTK_LIBRARIES} ${ITK_LIBRARIES})
#
# ADD_EXECUTABLE(SelfPatchMatchExample SelfPatchMatchExample.cpp SelfPatchMatch.h Helpers.h  Helpers.cpp)
# TARGET_LINK_LIBRARIES(SelfPatchMatchExample ${VTK_LIBRARIES} ${ITK_LIBRARIES})

# ADD_EXECUTABLE(CriminisiInpaintingRGBDI CriminisiInpaintingExampleRGBDI.cxx CriminisiInpainting.cpp CriminisiInpaintingDebugging.cpp CustomStyle.cpp SelfPatchMatch.h Helpers.h Helpers.cpp RotateVectors.h CriminisiInpainting.h)
# TARGET_LINK_LIBRARIES(CriminisiInpaintingRGBDI ${VTK_LIBRARIES} ${ITK_LIBRARIES})

##################################################################
# Testing

# SET( BUILD_TESTING OFF CACHE BOOL "Perform some tests." )
# IF( ITKTOOLS_BUILD_TESTING )
#   ENABLE_TESTING()
#   set(BaselineDir ${CriminisiInpainting_SOURCE_DIR}/../Testing/Baselines/)
#   set(DataDir ${CriminisiInpainting_SOURCE_DIR}/../Testing/Data/)
#   set(ExeDir ${CriminisiInpainting_BINARY_DIR})
#   set(OutDir ${CriminisiInpainting_BINARY_DIR}/Testing/)
# ENDIF()
# 
# ADD_EXECUTABLE(TestSelfPatchCompare TestSelfPatchCompare.cpp SelfPatchCompare.cpp)
# TARGET_LINK_LIBRARIES(TestSelfPatchCompare ${ITK_LIBRARIES})
