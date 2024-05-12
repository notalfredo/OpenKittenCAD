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

#include "Viewer3dSamples.h"


#include <AIS_Shape.hxx>
#include <AIS_ViewCube.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_SpotLight.hxx>
#include <V3d_PositionalLight.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_AmbientLight.hxx>

#include <BRepPrimAPI_MakeCone.hxx>
#include <AIS_Shape.hxx>
#include <QDebug>


void Viewer3dSamples::ExecuteSample(const TCollection_AsciiString& theSampleName)
{
  Standard_Boolean anIsSamplePresent = Standard_True;

  FindSourceCode(theSampleName);
  if (theSampleName == "VerticesSelect3dSample")
    VerticesSelect3dSample();
  else if (theSampleName == "EdgesSelect3dSample")
    EdgesSelect3dSample();
  else if (theSampleName == "FacesSelect3dSample")
    FacesSelect3dSample();
  else if (theSampleName == "NeutralPointSelect3dSample")
    NeutralPointSelect3dSample();
  else if (theSampleName == "WireFramePresentation3dSample")
    WireFramePresentation3dSample();
  else {
    myResult << "No function found: " << theSampleName;
    myCode += TCollection_AsciiString("No function found: ") + theSampleName;
    anIsSamplePresent = Standard_False;
  }
  myIsProcessed = anIsSamplePresent;
}

void Viewer3dSamples::AddShape(Handle(AIS_Shape) shape)
{
    myContext->Display(shape, AIS_Shaded, 0, false);
    myObject3d.Append(shape);
    myContext->UpdateCurrentViewer();
}


void Viewer3dSamples::ClearExtra()
{
  NeutralPointSelect3dSample();
  //VboOff3dSample();
  //ClearLight3dSample();
  // Delete Lights
  V3d_ListOfLight aLights;
  for(V3d_ListOfLightIterator anIter = myView->Viewer()->DefinedLightIterator(); 
      anIter.More(); anIter.Next())
  {
    aLights.Append(anIter.Value());
  }

  for(V3d_ListOfLightIterator aLightIter (aLights);
      aLightIter.More(); aLightIter.Next())
  {
    myView->Viewer()->DelLight (aLightIter.Value());
  }
  myView->Viewer()->SetDefaultLights(); // Setting the default lights on
  myView->Update();

  myContext->RemoveAll (Standard_True);
}

void Viewer3dSamples::VerticesSelect3dSample()
{
  myContext->Deactivate();
  myContext->Activate(AIS_Shape::SelectionMode(TopAbs_VERTEX));
}

void Viewer3dSamples::EdgesSelect3dSample()
{
  myContext->Deactivate();
  myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
}

void Viewer3dSamples::FacesSelect3dSample()
{
  myContext->Deactivate();
  myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
}

void Viewer3dSamples::NeutralPointSelect3dSample()
{
  myContext->Deactivate();
  myContext->Activate(0);
}

void Viewer3dSamples::WireFramePresentation3dSample()
{
  AIS_ListOfInteractive anAisObjectsList;
  myContext->DisplayedObjects(anAisObjectsList);
  for(AIS_ListOfInteractive::Iterator anIter(anAisObjectsList); 
      anIter.More(); anIter.Next())
  {
    const Handle(AIS_InteractiveObject)& anAisObject = anIter.Value();
    myContext->SetDisplayMode(anAisObject, 0, false); // set wireframe
  }
  myContext->UpdateCurrentViewer();
}
