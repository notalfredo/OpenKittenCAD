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

#include "ApplicationCommon.h"

#include <Standard_WarningsDisable.hxx>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QFrame>
#include <QGroupBox>
#include <QMap>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QPair>
#include <QSplitter>
#include <QStatusBar>
#include <QtGlobal>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QDomDocument>
#include <QDomAttr>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <Standard_WarningsRestore.hxx>

#include <OpenGl_GraphicDriver.hxx>
#include <OSD_Environment.hxx>

#include <stdlib.h>
#include <memory>


#include "../frontend/frontEndEntryPoint.hxx"
#include "AIS_DisplayMode.hxx"
#include "TopTools_HSequenceOfShape.hxx"


ApplicationCommonWindow::ApplicationCommonWindow (ApplicationType theCategory)
: QMainWindow (nullptr),
  myAppType(theCategory),
  myStdToolBar (nullptr),
  myViewBar (nullptr),
  myCasCadeBar (nullptr),
  myFilePopup (nullptr),
  myCategoryPopup (nullptr)
{
    ALL_CATEGORIES[AppType_DataExchange] = "DataExchange";
    ALL_CATEGORIES[AppType_Viewer3d] = "3D viewer settings";

    myViewer3dMapper = new QSignalMapper(this);
    myExchangeMapper = new QSignalMapper(this);
    myCategoryMapper = new QSignalMapper(this);

    connect(myViewer3dMapper, SIGNAL(mapped(const QString &)), this, SLOT(onProcessViewer3d(const QString &)));
    connect(myExchangeMapper, SIGNAL(mapped(const QString &)), this, SLOT(onProcessExchange(const QString &)));
    connect(myCategoryMapper, SIGNAL(mapped(const QString &)), this, SLOT(onChangeCategory(const QString &)));

    setFocusPolicy(Qt::StrongFocus);

    QFont aCodeViewFont;
    aCodeViewFont.setFamily("Courier");
    aCodeViewFont.setFixedPitch(true);
    aCodeViewFont.setPointSize(16);

    QGroupBox* aCodeFrame = new QGroupBox(tr("Sample code"));
    QVBoxLayout* aCodeLayout = new QVBoxLayout(aCodeFrame);
    aCodeLayout->setContentsMargins(3, 3, 3, 3);

    QHBoxLayout* aCodeHeaderLayout = new QHBoxLayout;
    aCodeHeaderLayout->addWidget(new QLabel(""));
    aCodeHeaderLayout->addStretch(1);



    //QPushButton* aCodeButton = new QPushButton("Click me");
    QPushButton* aCodeButton = new QPushButton;
    aCodeButton->setIcon(QIcon("./res/button-triangle-symbol-media-player-font-awesome.jpg"));
    aCodeButton->setIconSize(QSize(24, 24));  // Adjust the size as needed

    

    aCodeHeaderLayout->addWidget(aCodeButton);
    aCodeLayout->addLayout(aCodeHeaderLayout);
    myCodeView = new QTextEdit(aCodeFrame);

    aCodeLayout->addWidget(myCodeView);
    myCodeView->setDocumentTitle("Code");
    myCodeView->setLineWrapMode(QTextEdit::NoWrap);
    myCodeView->setReadOnly(false);
    myCodeView->setFont(aCodeViewFont);
    myCodeViewHighlighter = new OcctHighlighter(myCodeView->document());

     
    connect(aCodeButton, &QPushButton::clicked, this, [this]{
        QString codeText = myCodeView->toPlainText();
        std::string codeText_ = codeText.toStdString();
        const char* p_ = codeText_.c_str();

        qDebug() << "Code Text:" << codeText;
        std::cout << codeText_ << std::endl;
        std::cout << p_ << std::endl;
    });


    connect(aCodeButton, &QPushButton::clicked, this, [this](){
        QString codeEditorTxt = myCodeView->toPlainText();
        std::string strCodeEditorTxt = codeEditorTxt.toStdString();
        const char* p = strCodeEditorTxt.c_str();
        GenCode(strdup(p));
    });



    QGroupBox* aResultFrame = new QGroupBox(tr("Output"));
    QVBoxLayout* aResultLayout = new QVBoxLayout(aResultFrame);
    aResultLayout->setContentsMargins(3, 3, 3, 3);
    myResultView = new QTextEdit(aResultFrame);
    aResultLayout->addWidget(myResultView);
    myResultView->setDocumentTitle("Output");
    myResultView->setReadOnly(true);
    myResultView->setFont(aCodeViewFont);

    QSplitter* aCodeResultSplitter = new QSplitter(Qt::Vertical);
    aCodeResultSplitter->addWidget(aCodeFrame);
    aCodeResultSplitter->addWidget(aResultFrame);

    myDocument3d = createNewDocument();
    myDocument2d = createNewDocument();

    QFrame* aViewFrame = new QFrame;
    aViewFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    aViewFrame->setLineWidth(3);
    QVBoxLayout* aViewLayout = new QVBoxLayout(aViewFrame);
    aViewLayout->setContentsMargins(0, 0, 0, 0);
    myGeomWidget = new GeomWidget(myDocument3d, myDocument2d, aViewFrame);
    aViewLayout->addWidget(myGeomWidget);

    myGeomWidget->setContentsMargins(0, 0, 0, 0);
    QSplitter* aGeomTextSplitter = new QSplitter(Qt::Horizontal);

    aGeomTextSplitter->addWidget(aViewFrame);
    aGeomTextSplitter->addWidget(aCodeResultSplitter);
    aGeomTextSplitter->setStretchFactor(0, 1);
    aGeomTextSplitter->setStretchFactor(1, 1);
    QList<int> aSizeList;
    aSizeList.append(640);
    aSizeList.append(640);
    aGeomTextSplitter->setSizes(aSizeList);
    setCentralWidget(aGeomTextSplitter);

    #include <Standard_WarningsDisable.hxx>
        Q_INIT_RESOURCE(Samples);
    #include <Standard_WarningsRestore.hxx>

    TCollection_AsciiString aSampleSourcePach = getSampleSourceDir();



    myDataExchangeSamples  = new DataExchangeSamples(aSampleSourcePach,
                                                     myGeomWidget->Get3dView(),
                                                     myDocument3d->getContext());

    myViewer3dSamples      = new Viewer3dSamples(aSampleSourcePach,
                                                 myGeomWidget->Get3dView(),
                                                 myDocument3d->getContext());


    MenuFormXml(":/menus/DataExchange.xml",  myExchangeMapper, myDataExchangeMenus);
    MenuFormXml(":/menus/Viewer3d.xml",      myViewer3dMapper, myViewer3dMenus);

    onChangeCategory(ALL_CATEGORIES[myAppType]);

    resize(1280, 560);
}

void ApplicationCommonWindow::GenCode(const char* p)
{
    fprintf(stderr, ">%s<\n", p);
    
    programResponse* result = frontEndEntryPoint(
        p,
        string
    );

    switch(result->status){
        case lexical_error: {
            myResultView->setPlainText("Lexical error");
            break;
        }
        case parser_error: {
            myResultView->setPlainText("Parser error");
            break;
        }
        case semantic_error: {
            myResultView->setPlainText("Semantic error");
            break;
        }
        case other_error: {
            myResultView->setPlainText("Other error");
            break;
        }
        case success: {
            myResultView->setPlainText("Success !!!");

            for(int index = 0; index < result->vec.size(); index++){
                myViewer3dSamples->AddShape(result->vec[index]);
            }

            break;
        }
    }

}


void ApplicationCommonWindow::RebuildMenu()
{
  menuBar()->clear();

  myStdActions[StdActions_FileQuit] = CreateAction("Quit", "CTRL+Q");
  connect(myStdActions[StdActions_FileQuit], SIGNAL(triggered()), this, SLOT(onCloseAllWindows()));
  myStdActions[StdActions_HelpAbout] = CreateAction("About", "F1", ":/icons/help.png");
  connect(myStdActions[StdActions_HelpAbout], SIGNAL(triggered()), this, SLOT(onAbout()));

  // populate a menu with all actions
  myFilePopup = new QMenu(this);
  myFilePopup = menuBar()->addMenu(tr("&File"));
  myFilePopup->addAction(myStdActions[StdActions_FileQuit]);

  myCategoryPopup = new QMenu(this);
  myCategoryPopup = menuBar()->addMenu(tr("&Category"));

  foreach (ApplicationType aCategory, ALL_CATEGORIES.keys())
  {
    QString aCategoryName = ALL_CATEGORIES.value(aCategory);

    if(aCategoryName.toStdString() == "DataExchange"){
        continue;
    }

    qDebug() << aCategory;
    qDebug() << aCategoryName;

    QAction* anAction = myCategoryPopup->addAction(aCategoryName);
    anAction->setText(aCategoryName);

    myCategoryMapper->setMapping(anAction, aCategoryName);
    connect(anAction, SIGNAL(triggered()), myCategoryMapper, SLOT(map()));
    myCategoryPopup->addAction(anAction);
    myCategoryActions.insert(aCategory, anAction);
  }

  foreach (QMenu* aSampleMenu, GetCurrentMenus()) {
    menuBar()->addMenu(aSampleMenu);
  }

  
  foreach (QMenu* aSampleMenu, GetMenu(AppType_DataExchange)) {
    menuBar()->addMenu(aSampleMenu);
  }



  // add a help menu
  QMenu* aHelp = new QMenu(this);
  menuBar()->addSeparator();
  aHelp = menuBar()->addMenu(tr("&Help"));
  aHelp->addAction(myStdActions[StdActions_HelpAbout]);
}


Handle(BaseSample) ApplicationCommonWindow::GetCurrentSamples()
{
    switch (myAppType) {
        case AppType_Viewer3d: {
            return myViewer3dSamples;
        }
        case AppType_Unknown: {
            break;
        }
        case AppType_DataExchange: {
            return myDataExchangeSamples;
        }
        default: {
            break;
        }
    }
    

    qDebug() << "GetCurrentSamples()" << myAppType;
    throw QString("Unknown Application type");
}


const QList<QMenu*>&  ApplicationCommonWindow::GetMenu(ApplicationType application) {
    switch (application) {
        case AppType_Viewer3d: {
            return myViewer3dMenus;
        }   
        case AppType_Unknown: {
            break;
        }
        case AppType_DataExchange: {
            return myDataExchangeMenus;
        }
        default: {
            break; 
        }
    }
    qDebug() << "GetCurrentMenus()";
    throw QString("Unknown Application type");
}


const QList<QMenu*>& ApplicationCommonWindow::GetCurrentMenus() {
    switch (myAppType) {
        case AppType_Viewer3d: {
            return myViewer3dMenus;
        }   
        case AppType_Unknown: {
            break;
        }
        case AppType_DataExchange: {
            return myDataExchangeMenus;
        }
        default: {
            break; 
        }
    }
    qDebug() << "GetCurrentMenus()";
    throw QString("Unknown Application type");
}

DocumentCommon* ApplicationCommonWindow::createNewDocument()
{
  return new DocumentCommon(this);
}

void ApplicationCommonWindow::onChangeCategory(const QString& theCategory)
{
    myAppType = ALL_CATEGORIES.key(theCategory);
    setWindowTitle(ALL_CATEGORIES[myAppType]);

    myViewer3dSamples->ClearExtra();

    GetCurrentSamples()->Clear();
    myDocument3d->Clear();
    myDocument2d->Clear();

    myCodeView->setPlainText("");
    myResultView->setPlainText("");
    GetCurrentSamples()->AppendCube();
    myDocument3d->SetObjects(GetCurrentSamples()->Get3dObjects());
    myGeomWidget->FitAll();

    RebuildMenu();

    switch (myAppType) {
        case AppType_Viewer3d: {
            myDocument3d->SetObjects(GetCurrentSamples()->Get3dObjects());
            myGeomWidget->Show3d();
            break;
        }
        case AppType_DataExchange: {
            myDocument3d->SetObjects(GetCurrentSamples()->Get3dObjects());
            myGeomWidget->Show3d();
            break;
        }
        case AppType_Unknown:
        default: {
          break;
        }
    }
}

void ApplicationCommonWindow::onAbout()
{
  QMessageBox::information(this, tr("OpenKittenCad"),
    tr("OpenKittenCad is build on OpenCASCADE Technology\nOpenKittenCad is a free as in freedom CAD application"),
    tr("Ok"), QString::null, QString::null, 0, 0);
}

TCollection_AsciiString ApplicationCommonWindow::getSampleSourceDir()
{
  TCollection_AsciiString aSampleSourceDir = OSD_Environment("CSF_OCCTOverviewSampleCodePath").Value();
  if (aSampleSourceDir.IsEmpty())
  {
    TCollection_AsciiString aCasRoot = OSD_Environment("CASROOT").Value();
    if (!aCasRoot.IsEmpty())
    {
      aSampleSourceDir = aCasRoot + "/samples/OCCTOverview/code";
    }
  }
  return aSampleSourceDir;
}

QAction* ApplicationCommonWindow::CreateAction (const QString& theActionName,
                                                const QString& theShortcut,
                                                const QString& theIconName)
{
  QAction* aAction(NULL);
  if (theIconName.isEmpty())
  {
    aAction = new QAction(theActionName, this);
  }
  else
  {
    QPixmap aIcon = QPixmap(theIconName);
    aAction = new QAction(aIcon, theActionName, this);
  }
  aAction->setToolTip(theActionName);
  aAction->setStatusTip(theActionName);
  aAction->setShortcut(theShortcut);

  return aAction;
}

template <typename PointerToMemberFunction>
QAction* ApplicationCommonWindow::CreateSample (PointerToMemberFunction theHandlerMethod,
                                                const char* theActionName)
{
  QAction* anAction = new QAction(QObject::tr(theActionName), this);
  connect(anAction, SIGNAL(triggered()), this, SLOT(theHandlerMethod()));
  return anAction;
}

void ApplicationCommonWindow::resizeEvent(QResizeEvent* e)
{
  QMainWindow::resizeEvent(e);
  statusBar()->setSizeGripEnabled(!isMaximized());
}

void ApplicationCommonWindow::onProcessSample(const QString& theSampleName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setWindowTitle(ALL_CATEGORIES[myAppType] + " - " + theSampleName);
    GetCurrentSamples()->Process(theSampleName.toUtf8().data());
    myDocument3d->SetObjects(GetCurrentSamples()->Get3dObjects());
    myDocument2d->SetObjects(GetCurrentSamples()->Get2dObjects());
    myCodeView->setPlainText(GetCurrentSamples()->GetCode().ToCString());
    myResultView->setPlainText(GetCurrentSamples()->GetResult().ToCString());

    myGeomWidget->FitAll();

    QApplication::restoreOverrideCursor();
}

void ApplicationCommonWindow::onProcessExchange(const QString& theSampleName)
{
    setWindowTitle(ALL_CATEGORIES[myAppType] + " - " + theSampleName);
    int aMode = 0;
    QString aFileName = selectFileName(theSampleName, getDataExchangeDialog(theSampleName), aMode);
    if (aFileName.isEmpty())
    {
      return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    myDataExchangeSamples->SetFileName(aFileName.toUtf8().data());
    myDataExchangeSamples->SetStepType(static_cast<STEPControl_StepModelType>(aMode));

    myDataExchangeSamples->Process(theSampleName.toUtf8().data(), this->myViewer3dSamples->myObject3d);

    myDocument3d->SetObjects(myDataExchangeSamples->Get3dObjects());
    myDocument2d->SetObjects(myDataExchangeSamples->Get2dObjects());
    myCodeView->setPlainText(myDataExchangeSamples->GetCode().ToCString());
    myResultView->setPlainText(myDataExchangeSamples->GetResult().ToCString());


    myGeomWidget->FitAll();

    QApplication::restoreOverrideCursor();
}


void ApplicationCommonWindow::onProcessViewer3d(const QString& theSampleName)
{
    setWindowTitle(ALL_CATEGORIES[myAppType] + " - " + theSampleName);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    myViewer3dSamples->Process(theSampleName.toUtf8().data());

    QApplication::restoreOverrideCursor();
}


QString ApplicationCommonWindow::selectFileName(const QString& theSampleName,
                                                TranslateDialog* theDialog, int& theMode)
{
  Q_UNUSED(theSampleName)

  std::shared_ptr<TranslateDialog> aDialog(theDialog);

  int ret = aDialog->exec();
  theMode = aDialog->getMode();

  qApp->processEvents();

  QString aFilename;
  QStringList aFileNameList;
  if (ret != QDialog::Accepted)
  {
    return aFilename;
  }
  aFileNameList = aDialog->selectedFiles();
  if (!aFileNameList.isEmpty())
  {
    aFilename = aFileNameList[0];
  }

  if (!QFileInfo(aFilename).completeSuffix().length())
  {
    QString selFilter = aDialog->selectedNameFilter();
    int idx = selFilter.indexOf("(*.");
    if (idx != -1)
    {
      QString tail = selFilter.mid(idx + 3);
      idx = tail.indexOf(" ");
      if (idx == -1)
      {
        idx = tail.indexOf(")");
      }
      QString ext = tail.left(idx);
      if (ext.length())
      {
        aFilename += QString(".") + ext;
      }
    }
  }

  return aFilename;
}

TranslateDialog* ApplicationCommonWindow::getDataExchangeDialog(const QString& theSampleName)
{
  TranslateDialog* aTranslateDialog = new TranslateDialog(this, 0, true);
  TCollection_AsciiString aSampleName(theSampleName.toUtf8().data());

  if (DataExchangeSamples::IsExportSample(aSampleName))
  {
    aTranslateDialog->setWindowTitle("Export file");
    aTranslateDialog->setFileMode(QFileDialog::AnyFile);
    aTranslateDialog->setAcceptMode(QFileDialog::AcceptSave);
  }
  else if (DataExchangeSamples::IsImportSample(aSampleName))
  {
    aTranslateDialog->setWindowTitle("Import file");
    aTranslateDialog->setFileMode(QFileDialog::ExistingFile);
    aTranslateDialog->setAcceptMode(QFileDialog::AcceptOpen);
  }
  QString aFormatFilter;
  if (DataExchangeSamples::IsBrepSample(aSampleName))
  {
    aFormatFilter = "BREP Files(*.brep *.rle)";
  }
  else if (DataExchangeSamples::IsStepSample(aSampleName))
  {
    aFormatFilter = "STEP Files (*.stp *.step)";
    aTranslateDialog->addMode(STEPControl_ManifoldSolidBrep, "Manifold Solid Brep");
    aTranslateDialog->addMode(STEPControl_FacetedBrep, "Faceted Brep");
    aTranslateDialog->addMode(STEPControl_ShellBasedSurfaceModel, "Shell Based Surface Model");
    aTranslateDialog->addMode(STEPControl_GeometricCurveSet, "Geometric Curve Set");
  }
  else if (DataExchangeSamples::IsIgesSample(aSampleName))
  {
    aFormatFilter = "IGES Files (*.igs *.iges)";
  }
  else if (DataExchangeSamples::IsStlSample(aSampleName))
  {
    aFormatFilter = "STL Files (*.stl)";
  }
  else if (DataExchangeSamples::IsVrmlSample(aSampleName))
  {
    aFormatFilter = "VRML Files (*.vrml)";
  }
  else if (DataExchangeSamples::IsImageSample(aSampleName))
  {
    aFormatFilter = "All Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.tga)";
  }
  QStringList aFilters;
  aFilters.append(aFormatFilter);
  aFilters.append("All Files(*.*)");

  aTranslateDialog->setNameFilters(aFilters);
  aTranslateDialog->clear();
  return aTranslateDialog;
}


QMenu* ApplicationCommonWindow::MenuFromDomNode(QDomElement& theItemElement,
                                                QWidget* theParent,
                                                QSignalMapper* theMapper)
{
  QString anItemName        = theItemElement.attribute("name");
  QMenu* aMenu = new QMenu(anItemName, theParent);
  QDomElement anChildItemElement = theItemElement.firstChildElement("MenuItem");
  QDomElement anSampleElement    = theItemElement.firstChildElement("Sample");

  while(anChildItemElement.isElement())
  {
    aMenu->addMenu(MenuFromDomNode(anChildItemElement, aMenu, theMapper));
    anChildItemElement = anChildItemElement.nextSibling().toElement();
  }

  while(anSampleElement.isElement())
  {
    QString aSampleName     = anSampleElement.attribute("name");
    QString aSampleFunction = anSampleElement.attribute("function");
    QAction* anAction = aMenu->addAction(aSampleFunction);
    anAction->setText(aSampleName);
    theMapper->setMapping(anAction, aSampleFunction);
    connect(anAction, SIGNAL(triggered()), theMapper, SLOT(map()));
    anSampleElement = anSampleElement.nextSibling().toElement();
  }
  return aMenu;
}

void ApplicationCommonWindow::MenuFormXml(const QString& thePath,
                                          QSignalMapper* theMapper,
                                          QList<QMenu*>& theMunusList)
{
    QDomDocument aDomDocument;
    theMunusList.clear();
    QFile aXmlFile(thePath);
    QString anErrorMessage;
    if (aXmlFile.error() != QFile::NoError) {
        anErrorMessage = aXmlFile.errorString();
        Message::SendFail() << "QFile creating error: " << anErrorMessage.toUtf8().constData();
        aXmlFile.close();
        return;
    }
    if (!aXmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Message::SendFail() << "File " << thePath.toUtf8().constData() << " could not open";
        if (aXmlFile.error() != QFile::NoError) {
          anErrorMessage = aXmlFile.errorString();
          Message::SendFail() << "QFile opening error: " << anErrorMessage.toUtf8().constData();
        }
        aXmlFile.close();
        return;
    }
    bool aNamespaceProcessing(false);
    QString anErrorMsg;
    int anErrorLine(0);
    int anErrorColumn(0);
    if (!aDomDocument.setContent(&aXmlFile, aNamespaceProcessing, &anErrorMsg, &anErrorLine, &anErrorColumn))
    {
        Message::SendFail() << "XML file parsing error: " <<  anErrorMsg.toStdString()
                            << " at line: " << anErrorLine << " column: " << anErrorColumn;
        aXmlFile.close();
        return;
    }
    aXmlFile.close();

    QDomElement aRootElement = aDomDocument.documentElement();
    QDomElement anItemElement = aRootElement.firstChildElement("MenuItem");
    while(!anItemElement.isNull())
    {
        theMunusList.push_back(MenuFromDomNode(anItemElement, this, theMapper));
        anItemElement = anItemElement.nextSiblingElement("MenuItem");
    }
}
