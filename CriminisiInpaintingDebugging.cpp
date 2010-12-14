/*=========================================================================
 *
 *  Copyright David Doria 2010 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "CriminisiInpainting.h"

#include "Helpers.h"

#include <iomanip> // setfill, setw

template <class TImage>
void CriminisiInpainting<TImage>::DebugWriteAllImages(itk::Index<2> pixelToFill, itk::Index<2> bestMatchPixel, unsigned int iteration)
{
  std::cout << "Writing debug images for iteration " << iteration << std::endl;

  DebugWritePatch(bestMatchPixel, "BestMatchPatch", iteration);
  std::cout << "Wrote BestMatchPatch.";

  DebugWritePatchToFillLocation(pixelToFill, iteration);
  std::cout << "Wrote pixelToFillLocation." << std::endl;

  DebugWritePixelToFill(pixelToFill, iteration);
  std::cout << "Wrote pixelToFill." << std::endl;

  {
  std::stringstream padded;
  padded << "Isophotes" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<VectorImageType>(this->IsophoteImage, padded.str());
  std::cout << "Wrote isophotes." << std::endl;
  }

  {
  std::stringstream padded;
  padded << "Confidence_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<FloatImageType>(this->ConfidenceImage, padded.str());
  std::cout << "Wrote confidence." << std::endl;
  }

  {
  std::stringstream padded;
  padded << "Boundary_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<UnsignedCharImageType>(this->BoundaryImage, padded.str());
  std::cout << "Wrote boundary." << std::endl;
  }

  {
  std::stringstream padded;
  padded << "BoundaryNormals_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<VectorImageType>(this->BoundaryNormals, padded.str());
  std::cout << "Wrote boundaryNormals." << std::endl;
  }

  {
  std::stringstream padded;
  padded << "Priorities_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<FloatImageType>(this->PriorityImage, padded.str());
  std::cout << "Wrote priorities." << std::endl;
  }

  {
  std::stringstream padded;
  padded << "Mask_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<UnsignedCharImageType>(this->Mask, padded.str());
  std::cout << "Wrote mask." << std::endl;
  }

  {
  std::stringstream padded;
  padded << "FilledImage_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<TImage>(this->Image, padded.str());
  std::cout << "Wrote filled image." << std::endl;
  }
}

template <class TImage>
void CriminisiInpainting<TImage>::DebugWritePatch(itk::Index<2> pixel, std::string filePrefix, unsigned int iteration)
{
  std::stringstream padded;
  padded << filePrefix << "_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  DebugWritePatch(pixel, padded.str());

}

template <class TImage>
void CriminisiInpainting<TImage>::DebugWritePatch(itk::Index<2> pixel, std::string filename)
{
  typedef itk::RegionOfInterestImageFilter< TImage,
                                            TImage> ExtractFilterType;

  typename ExtractFilterType::Pointer extractFilter = ExtractFilterType::New();
  extractFilter->SetRegionOfInterest(GetRegionInRadiusAroundPixel(pixel, this->PatchRadius[0]));
  extractFilter->SetInput(this->Image);
  extractFilter->Update();

  WriteImage<TImage>(extractFilter->GetOutput(), filename);

}

template <class TImage>
void CriminisiInpainting<TImage>::DebugWritePixelToFill(itk::Index<2> pixelToFill, unsigned int iteration)
{
  // Create a blank image with the pixel to fill colored white
  UnsignedCharImageType::Pointer pixelImage = UnsignedCharImageType::New();
  pixelImage->SetRegions(this->Mask->GetLargestPossibleRegion());
  pixelImage->Allocate();

  itk::ImageRegionIterator<UnsignedCharImageType> iterator(pixelImage, pixelImage->GetLargestPossibleRegion());

  while(!iterator.IsAtEnd())
    {
    if(iterator.GetIndex()[0] == pixelToFill[0] && iterator.GetIndex()[1] == pixelToFill[1])
      {
      iterator.Set(255);
      }
    else
      {
      iterator.Set(0);
      }
    ++iterator;
    }


  std::stringstream padded;
  padded << "PixelToFill_" << std::setfill('0') << std::setw(4) << iteration << ".jpg";

  WriteImage<UnsignedCharImageType>(pixelImage, padded.str());
}

template <class TImage>
void CriminisiInpainting<TImage>::DebugWritePatchToFillLocation(itk::Index<2> pixelToFill, unsigned int iteration)
{
  // Create a blank image with the patch that has been filled colored white
  UnsignedCharImageType::Pointer patchImage = UnsignedCharImageType::New();
  patchImage->SetRegions(this->Mask->GetLargestPossibleRegion());
  patchImage->Allocate();
  // Make image black
  itk::ImageRegionIterator<UnsignedCharImageType> blackIterator(patchImage, patchImage->GetLargestPossibleRegion());

  while(!blackIterator.IsAtEnd())
    {
    blackIterator.Set(0);
    ++blackIterator;
    }

  UnsignedCharImageType::Pointer patch = UnsignedCharImageType::New();
  CreateConstantPatch<UnsignedCharImageType>(patch, 255, this->PatchRadius[0]);

  CopyPatchIntoImage<UnsignedCharImageType>(patch, patchImage, pixelToFill);

  std::stringstream padded;
  padded << "PatchToFillLocation_" << std::setfill('0') << std::setw(4) << iteration << ".mhd";
  WriteImage<UnsignedCharImageType>(patchImage, padded.str());
}

template <class TImage>
void CriminisiInpainting<TImage>::DebugTests()
{
  /*
  // Check to make sure patch doesn't have any masked pixels
  itk::ImageRegionIterator<ColorImageType> patchIterator(this->Patch,this->Patch->GetLargestPossibleRegion());

  while(!patchIterator.IsAtEnd())
    {
    ColorImageType::PixelType val = patchIterator.Get();
    if(val[0] == 0 && val[1] == 255 && val[2] == 0)
      {
      std::cerr << "Patch has a blank pixel!" << std::endl;
      exit(-1);
      }
    ++patchIterator;
    }
  */
}

template class CriminisiInpainting<ColorImageType>;
template class CriminisiInpainting<RGBDIImageType>;