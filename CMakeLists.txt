cmake_minimum_required(VERSION 2.6)
PROJECT(Inpainting)
 
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
 
ADD_EXECUTABLE(Inpainting vtkFastDigitalInpaintingExample.cxx vtkInpainting.cxx vtkBertalmioInpainting.cxx vtkFastDigitalInpainting.cxx)
TARGET_LINK_LIBRARIES(Inpainting vtkHybrid)

ADD_EXECUTABLE(vtkImageVectorRotateExample vtkImageVectorRotateExample.cxx vtkImageVectorRotate.cxx)
TARGET_LINK_LIBRARIES(vtkImageVectorRotateExample vtkHybrid)