// Copyright (c) 2020 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

#ifndef VIEWER3DSAMPLES_H
#define VIEWER3DSAMPLES_H

#include "BaseSample.h"

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

#include <AIS_Shape.hxx>

//! Implements viewer 3D samples.
class Viewer3dSamples : public BaseSample
{
  DEFINE_STANDARD_RTTI_INLINE(Viewer3dSamples, BaseSample)
public:

  Viewer3dSamples (const TCollection_AsciiString& theSampleSourcePath,
                   const Handle(V3d_View)& theView,
                   const Handle(AIS_InteractiveContext)& theContext)
  : BaseSample (theSampleSourcePath, theContext),
    myView (theView)
  {}

  void AddShape(Handle(AIS_Shape) shape);
  void ClearExtra();

protected:
  virtual void ExecuteSample (const TCollection_AsciiString& theSampleName) Standard_OVERRIDE;

private:
  void VerticesSelect3dSample();
  void EdgesSelect3dSample();
  void FacesSelect3dSample();
  void NeutralPointSelect3dSample();
  void WireFramePresentation3dSample();

private:

  Handle(V3d_View) myView;

};

#endif  //VIEWER3DSAMPLES_H
