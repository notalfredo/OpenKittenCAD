// Copyright (c) 2019 OPEN CASCADE SAS
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

#include "GlfwOcctView.h"

#include <AIS_Shape.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <AIS_ViewCube.hxx>

#include <iostream>

#include <GLFW/glfw3.h>

#include "./imgui/imgui.h"
#include "./imgui/imgui_stdlib.h"
#include "./imgui/imgui_impl_opengl3.h"
#include "./imgui/imgui_impl_glfw.h"

int mode = 0;


namespace
{






  //! Convert GLFW mouse button into Aspect_VKeyMouse.
  static Aspect_VKeyMouse mouseButtonFromGlfw (int theButton)
  {
    switch (theButton)
    {
      case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
      case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
      case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
    }
    return Aspect_VKeyMouse_NONE;
  }

  //! Convert GLFW key modifiers into Aspect_VKeyFlags.
  static Aspect_VKeyFlags keyFlagsFromGlfw (int theFlags)
  {
    Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
    if ((theFlags & GLFW_MOD_SHIFT) != 0)
    {
      aFlags |= Aspect_VKeyFlags_SHIFT;
    }
    if ((theFlags & GLFW_MOD_CONTROL) != 0)
    {
      aFlags |= Aspect_VKeyFlags_CTRL;
    }
    if ((theFlags & GLFW_MOD_ALT) != 0)
    {
      aFlags |= Aspect_VKeyFlags_ALT;
    }
    if ((theFlags & GLFW_MOD_SUPER) != 0)
    {
      aFlags |= Aspect_VKeyFlags_META;
    }
    return aFlags;
  }
}

// ================================================================
// Function : GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::GlfwOcctView()
{
}

// ================================================================
// Function : ~GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::~GlfwOcctView()
{
}

// ================================================================
// Function : toView
// Purpose  :
// ================================================================
GlfwOcctView* GlfwOcctView::toView (GLFWwindow* theWin)
{
  return static_cast<GlfwOcctView*>(glfwGetWindowUserPointer (theWin));
}

// ================================================================
// Function : errorCallback
// Purpose  :
// ================================================================
void GlfwOcctView::errorCallback (int theError, const char* theDescription)
{
  Message::DefaultMessenger()->Send (TCollection_AsciiString ("Error") + theError + ": " + theDescription, Message_Fail);
}

// ================================================================
// Function : run
// Purpose  :
// ================================================================
void GlfwOcctView::run()
{
  initWindow (800, 600, "glfw occt");
  initViewer();
  initDemoScene();
  if (myView.IsNull())
  {
    return;
  }

  myView->MustBeResized();
  myOcctWindow->Map();
  initUI();
  mainloop();
  cleanupUI();
  cleanup();
}

// ================================================================
// function : initWindow
// purpose  :
// ================================================================
void GlfwOcctView::initWindow (int theWidth, int theHeight, const char* theTitle)
{
  glfwSetErrorCallback (GlfwOcctView::errorCallback);
  glfwInit();
  const bool toAskCoreProfile = true;
  if (toAskCoreProfile)
  {
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
#if defined (__APPLE__)
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  myOcctWindow = new GlfwOcctWindow (theWidth, theHeight, theTitle);
  glfwSetWindowUserPointer       (myOcctWindow->getGlfwWindow(), this);
  // window callback
  glfwSetWindowSizeCallback      (myOcctWindow->getGlfwWindow(), GlfwOcctView::onResizeCallback);
  glfwSetFramebufferSizeCallback (myOcctWindow->getGlfwWindow(), GlfwOcctView::onFBResizeCallback);
  // mouse callback
  glfwSetScrollCallback          (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseScrollCallback);
  glfwSetMouseButtonCallback     (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseButtonCallback);
  glfwSetCursorPosCallback       (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseMoveCallback);


  myImGuiWindow = glfwCreateWindow(350, 550, "ImGui Window", nullptr, nullptr);
  glfwSetWindowUserPointer(myImGuiWindow, this);



}

// ================================================================
// Function : initViewer
// Purpose  :
// ================================================================
void GlfwOcctView::initViewer()
{
  if (myOcctWindow.IsNull()
   || myOcctWindow->getGlfwWindow() == nullptr)
  {
    return;
  }

  Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver (myOcctWindow->GetDisplay(), false);

  Handle(V3d_Viewer) aViewer = new V3d_Viewer (aGraphicDriver);
  //aGraphicDriver->SetBuffersNoSwap(true);

  aViewer->SetDefaultLights();
  aViewer->SetLightOn();
  aViewer->SetDefaultTypeOfView (V3d_PERSPECTIVE);
  aViewer->ActivateGrid (Aspect_GT_Rectangular, Aspect_GDM_Lines);
  myView = aViewer->CreateView();
  myView->SetImmediateUpdate (false);
  myView->SetWindow (myOcctWindow, myOcctWindow->NativeGlContext());
  myView->ChangeRenderingParams().ToShowStats = true;
  myContext = new AIS_InteractiveContext (aViewer);
}

// ================================================================
// Function : initDemoScene
// Purpose  :
// ================================================================
void GlfwOcctView::initDemoScene()
{
  if (myContext.IsNull())
  {
    return;
  }

  myView->TriedronDisplay (Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);

  gp_Ax2 anAxis;
  anAxis.SetLocation (gp_Pnt (1.0, 0.0, 0.0));
  Handle(AIS_Shape) aBox = new AIS_Shape (BRepPrimAPI_MakeBox (anAxis, 50, 50, 50).Shape());


  myContext->Display (aBox, AIS_Shaded, mode, false);


  anAxis.SetLocation (gp_Pnt (25.0, 125.0, 0.0));
  Handle(AIS_Shape) aCone = new AIS_Shape (BRepPrimAPI_MakeCone (anAxis, 25, 0, 50).Shape());
  myContext->Display (aCone, AIS_Shaded, mode, false);

  Handle(AIS_ViewCube) aViewCube = new AIS_ViewCube();
  myContext->Display(aViewCube, true);




  TCollection_AsciiString aGlInfo;
  {
    TColStd_IndexedDataMapOfStringString aRendInfo;
    myView->DiagnosticInformation (aRendInfo, Graphic3d_DiagnosticInfo_Basic);
    for (TColStd_IndexedDataMapOfStringString::Iterator aValueIter (aRendInfo); aValueIter.More(); aValueIter.Next())
    {
      if (!aGlInfo.IsEmpty()) { aGlInfo += "\n"; }
      aGlInfo += TCollection_AsciiString("  ") + aValueIter.Key() + ": " + aValueIter.Value();
    }
  }
  Message::DefaultMessenger()->Send (TCollection_AsciiString("OpenGL info:\n") + aGlInfo, Message_Info);
}

// ================================================================
// Function : mainloop
// Purpose  :
// ================================================================
void GlfwOcctView::mainloop()
{
    while (
        !glfwWindowShouldClose(myOcctWindow->getGlfwWindow())
            &&
        !glfwWindowShouldClose(myImGuiWindow))
    {
        glfwPollEvents();

        // Render OpenCASCADE window
        glfwMakeContextCurrent(myOcctWindow->getGlfwWindow());
        if (!myView.IsNull())
        {
            myView->Invalidate();
            FlushViewEvents(myContext, myView, true);
            myView->Redraw();
        }

        // Render ImGui window
        glfwMakeContextCurrent(myImGuiWindow);
        processUI();
    }
}

// ================================================================
// Function : cleanup
// Purpose  :
// ================================================================
void GlfwOcctView::cleanup()
{
  if (!myView.IsNull())
  {
    myView->Remove();
  }
  if (!myOcctWindow.IsNull())
  {
    myOcctWindow->Close();
  }
  if (myImGuiWindow != nullptr)
  {
    glfwDestroyWindow(myImGuiWindow);
  }

  glfwTerminate();
}

// ================================================================
// Function : onResize
// Purpose  :
// ================================================================
void GlfwOcctView::onResize (int theWidth, int theHeight)
{
  if (theWidth  != 0
   && theHeight != 0
   && !myView.IsNull())
  {
    myView->Window()->DoResize();
    myView->MustBeResized();
    myView->Invalidate();
    myView->Redraw();
  }
}

// ================================================================
// Function : onMouseScroll
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseScroll (double theOffsetX, double theOffsetY)
{
  if (!myView.IsNull())
  {
    UpdateZoom (Aspect_ScrollDelta (myOcctWindow->CursorPosition(), int(theOffsetY * 8.0)));
  }
}

// ================================================================
// Function : onMouseButton
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseButton (int theButton, int theAction, int theMods)
{

  if (myView.IsNull()) { return; }

  const Graphic3d_Vec2i aPos = myOcctWindow->CursorPosition();
  if (theAction == GLFW_PRESS)
  {
    PressMouseButton (aPos, mouseButtonFromGlfw (theButton), keyFlagsFromGlfw (theMods), false);
  }
  else
  {
    ReleaseMouseButton (aPos, mouseButtonFromGlfw (theButton), keyFlagsFromGlfw (theMods), false);
  }
}

// ================================================================
// Function : onMouseMove
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseMove (int thePosX, int thePosY)
{
  const Graphic3d_Vec2i aNewPos (thePosX, thePosY);
  if (!myView.IsNull())
  {
    UpdateMousePosition (aNewPos, PressedMouseButtons(), LastMouseFlags(), false);
  }
}

void GlfwOcctView::initUI()
{
    // Setup Dear ImGui context
    //const char* glsl_version = "#version 330";
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();    
    //// Setup Platform/Renderer bindings
    //ImGui_ImplGlfw_InitForOpenGL(myOcctWindow->getGlfwWindow(), true);
    //ImGui_ImplOpenGL3_Init(glsl_version);
    //// Setup Dear ImGui style
    //ImGui::StyleColorsDark();


    //ImGui_ImplGlfw_InitForOpenGL(myImGuiWindow, true);
    

    const char* glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(myImGuiWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

}


std::string myArray = "";


void GlfwOcctView::processUI()
{
    // feed inputs to dear imgui, start new frame 
    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplGlfw_NewFrame();
    //ImGui::NewFrame();
    //
    //ImGui::Begin("STEP files");   
    //
    //ImGui::Button("Add File");
    //ImGui::SameLine();
    //ImGui::Button("Clear List");
    ////ImGui::ListBox("Files",&currentItem,listboxItems,currentItemsCount,10);
    //
    //ImGui::End();

    //// Rendering
    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    
    // feed inputs to dear imgui, start new frame 
    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplGlfw_NewFrame();
    //ImGui::NewFrame();
    //
    //ImGui::Begin("STEP files");   
    //
    //ImGui::Button("Add File");
    //ImGui::SameLine();
    //ImGui::Button("Clear List");
    ////ImGui::ListBox("Files",&currentItem,listboxItems,currentItemsCount,10);
    //
    //ImGui::End();

    //// Rendering
    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT);

    // Create the ImGui window
    

    ImVec2 windowPosition(0, 0);
    ImGui::SetNextWindowPos(windowPosition, ImGuiCond_FirstUseEver);

    
    ImGui::Begin("Utils", nullptr, ImGuiWindowFlags_NoMove);
    


    if(ImGui::Button("Gen Random cone")){
        int random = rand() % 700;
        int random2 = rand() % 700;


        if(rand() % 2 == 0){
            random *= -1;
        }
        if(rand() % 2 == 0){
            random2 *= -1;
        }


        gp_Ax2 anAxis;
        anAxis.SetLocation (gp_Pnt (random, random2, 0.0));
        Handle(AIS_Shape) aCone = new AIS_Shape (BRepPrimAPI_MakeCone (anAxis, 25, 0, 50).Shape());
        myContext->Display (aCone, AIS_Shaded, 0, false);
    }

    ImGui::SameLine();

    if(ImGui::Button("Compile")){
        std::cout << myArray << std::endl;
    }



    ImGui::Text("Selection Mode");
    if(ImGui::Button("Face")){
        myContext->Deactivate();
        myContext->Activate(AIS_Shape::SelectionMode(TopAbs_FACE));
    }
    ImGui::SameLine();
    if(ImGui::Button("Solid")){
        myContext->Deactivate();
        myContext->Activate(AIS_Shape::SelectionMode(TopAbs_SOLID));
    }
    ImGui::SameLine();
    if(ImGui::Button("Edge")){
        myContext->Deactivate();
        myContext->Activate(AIS_Shape::SelectionMode(TopAbs_EDGE));
    }


    ImGui::Text("Perform some op");
    if(ImGui::Button("Count Selected")){
        std::cout << myContext->NbSelected() << std::endl;
    }
    ImGui::SameLine();
    if(ImGui::Button("Selected Objects")){
        if(myContext->NbSelected()){
            for(myContext->InitSelected(); myContext->MoreSelected(); myContext->NextSelected()){
                TopoDS_Shape object = myContext->SelectedShape();


                switch(object.ShapeType()){
                    case TopAbs_COMPOUND: {
                        std::cout << "TopAbs_COMPOUND" << std::endl; 
                        break;
                    }
                    case TopAbs_COMPSOLID: {
                        std::cout << "TopAbs_COMPSOLID" << std::endl; 
                        break;
                    }
                    case TopAbs_SOLID: {
                        std::cout << "TopAbs_SOLID" << std::endl; 
                        break;
                    }
                    case TopAbs_SHELL: {
                        std::cout << "TopAbs_SHELL" << std::endl; 
                        break;
                    }
                    case TopAbs_FACE: {
                        std::cout << "TopAbs_FACE" << std::endl; 
                        break;
                    }
                    case TopAbs_WIRE: {
                        std::cout << "TopAbs_WIRE" << std::endl; 
                        break;
                    }
                    case TopAbs_EDGE: {
                        std::cout << "TopAbs_EDGE" << std::endl; 
                        break;
                    }
                    case TopAbs_VERTEX: {
                        std::cout << "TopAbs_VERTEX" << std::endl; 
                        break;
                    }
                    case TopAbs_SHAPE: {
                        std::cout << "TopAbs_SHAPE" << std::endl; 
                        break;
                    }
                }
            }            
        }
    }
    ImGui::End();


    //======================================
    //======================================
    //======================================
    ImGui::Begin("Text Editor");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                                   
    if(ImGui::Button("Compile")){
        std::cout << myArray << std::endl;
    }

    ImGui::PushItemWidth(350);
    

   ImGui::InputTextMultiline("##", &myArray,
                          ImVec2(350, 200),
                          ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_EnterReturnsTrue); 


    ImGui::End();

    //======================================
    //======================================
    //======================================



    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(myImGuiWindow);
    
}

void GlfwOcctView::cleanupUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
