// COPYRIGHT Dassault Systemes 2022
//===================================================================
//
// TRACommand2.cpp
// The command: TRACommand2
//
//===================================================================
//
// Usage notes:
//
//===================================================================
//
//  Aug 2022  Creation: Code generated by the CAA wizard  Naga
//===================================================================
#include "TRACommand2.h"

// System
#include "CATUnicodeString.h"
#include "CATIUnknownList.h"

// ObjectModelerBase
#include "CATDocument.h"
#include "CATDocumentServices.h"
#include "CATSessionServices.h" 
#include "CATIContainer.h"

// ObjectSpecsModeler
#include "CATISpecObject.h"

// DraftingInterfaces
#include "CATIDftDocumentServices.h"
#include "CATIDftDrawing.h"
#include "CATIDftSheet.h"
#include "CATIDftViewMakeUp.h"
#include "CATIDftView.h"
#include "CATIView.h"
#include "CATDrwViewType.h"
#include "CATIDrwFactory.h"

#include "CATIDftStandardManager.h"
#include "CATIDftFormat.h"
#include "CATIDftSheetFormat.h"
#include "CATIDftDrawingFormats.h"

// SketcherInterfaces
#include "CATI2DWFFactory.h"
#include "CATISketch.h"

#include "CATCreateExternalObject.h"
#include "CATFrmEditor.h"

CATCreateClass( TRACommand2);


//-------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------
TRACommand2::TRACommand2() :
  CATCommand (NULL, "TRACommand2")
{
  RequestStatusChange (CATCommandMsgRequestExclusiveMode);
}

//-------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------
TRACommand2::~TRACommand2()
{
}


//  Overload this method: when your command gains focus
//
// Activates a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand2::Activate( CATCommand * iFromClient, CATNotification * iEvtDat)
{
	CATFrmEditor *pEditor = NULL;
	pEditor = CATFrmEditor::GetCurrentEditor();
	
	CATDocument* pDoc = NULL;
    pDoc = pEditor->GetDocument();

   // DRAWING STANDARD CREATION
   // ===============================
   
   // Gets the drawing feature and drawing container
   CATIDftDrawing *piDftDrawing = NULL;
   CATIDftDocumentServices *piDftDocServices = NULL;
   CATIContainer_var spDrwCont;
   if (SUCCEEDED(pDoc->QueryInterface(IID_CATIDftDocumentServices, (void **)&piDftDocServices)))
   {
      if (SUCCEEDED(piDftDocServices->GetDrawing(IID_CATIDftDrawing, (void **)&piDftDrawing)))
      {
         if (piDftDrawing)
         {
            // Gets the drawing container
            CATISpecObject *piSpecObj=NULL;
            if (SUCCEEDED(piDftDrawing->QueryInterface(IID_CATISpecObject,(void **)&piSpecObj)))
            {
               spDrwCont = piSpecObj->GetFeatContainer();
               piSpecObj->Release();
               piSpecObj=NULL;
            }
         }
      }
      piDftDocServices->Release();
      piDftDocServices=NULL;
   }

    // Gets standard manager from the drawing container.
   CATIDftStandardManager *piStdmgr = NULL;
   
   if (SUCCEEDED(spDrwCont->QueryInterface(IID_CATIDftStandardManager,(void**)&piStdmgr)))
   {
      //  Finds a standard in the list of allowed standards (ie. the list of xml files in the resources/standard/drafting directory)
      CATIStringList *piListstd = NULL;
      if ( SUCCEEDED(piStdmgr->GetAvailableStandards(&piListstd)) && piListstd )
      {
         unsigned int  nbrstd = 0;
         piListstd->Count(&nbrstd);
         for (unsigned int indice = 0; indice < nbrstd; indice ++)
         {
            wchar_t  *wstd = NULL;
            if ( SUCCEEDED ( piListstd->Item ( indice, &wstd ) )  && wstd )
            {
               const CATUnicodeString ANSI_UncS = "ANSI";
               CATUnicodeString stdname;
               stdname.BuildFromWChar(wstd);
               if ( stdname == ANSI_UncS )  
               {
                  // Import the ANSI standard in the document
                  piStdmgr->ImportStandard (wstd);
                  break;
               }
            }
            if (wstd) {delete[] wstd; wstd = NULL;}
         }
         piListstd->Release(); piListstd=NULL;            
      }
      piStdmgr->Release (); piStdmgr=NULL;
   }

   // ADDING FORMAT TO THE CURRENT SHEET
   // ========================================
   
   // Gets available formats from drawing
   CATIDftDrawingFormats *piDftFormats = NULL;
   CATUnicodeString myFormatName;
   if (SUCCEEDED(piDftDrawing->QueryInterface(IID_CATIDftDrawingFormats,(void **)&piDftFormats)))
   {
      CATLISTV(CATISpecObject_var) spListFormat;
      if (SUCCEEDED(piDftFormats->GetAvailableFormats(spListFormat)))
      {
         int nbformats= spListFormat.Size();
         // Gets the first format in the list.
         if (nbformats >= 1)
         {
            CATIDftFormat_var spFormat = spListFormat[1];
            if (FAILED(spFormat->GetFormatName(myFormatName)))
            {
               // Memory clean.
               piDftFormats->Release();
               piDftFormats=NULL;
               piDftDrawing->Release();
               piDftDrawing=NULL;
               CATDocumentServices::Remove (*pDoc);
               ::Delete_Session("SampleSession");
               
            }
         }
      }
      piDftFormats->Release();
      piDftFormats=NULL;
   }

    // Adds the format to the current sheet.
   CATIDftSheet *piDftSheet = NULL;
   CATIDftSheetFormat *piDftSheetFormat = NULL;

   // Gets active sheet.
   if (SUCCEEDED(piDftDrawing->GetActiveSheet(&piDftSheet)))
   {
      CATIDftSheetFormat *piDftSheetFormat = NULL;
      if (piDftSheet && SUCCEEDED(piDftSheet->QueryInterface (IID_CATIDftSheetFormat,(void **)&piDftSheetFormat)))
      {
         if (FAILED(piDftSheetFormat->SetSheetFormat(myFormatName)))
         {
            // Memory clean.
            piDftSheetFormat->Release();
            piDftSheetFormat=NULL;
            piDftSheet->Release();
            piDftSheet=NULL;
            piDftDrawing->Release();
            piDftDrawing=NULL;
            CATDocumentServices::Remove (*pDoc);
            ::Delete_Session("SampleSession");
            
         }
         piDftSheetFormat->Release();
         piDftSheetFormat=NULL;
      }
      piDftSheet->Release();
      piDftSheet=NULL;
   }

    // ADDING A NEW SHEET TO THE DRAWING
   // =======================================
   CATIDftSheet *piDftNewSheet = NULL;
   wchar_t *pSheetName= L"MyNewSheet";
   if (SUCCEEDED(piDftDrawing->AddSheet(&piDftNewSheet,pSheetName)))
   {
      // The drawing factory is implemented on the drawing container
      CATIDrwFactory_var spDrwFact = spDrwCont;
      
      // ADDING A NEW VIEW TO THE NEW SHEET
      // ========================================
      
      // Create a view with Make Up
      CATIDftViewMakeUp *piNewViewMU = NULL;
      if (NULL_var != spDrwFact && SUCCEEDED(spDrwFact -> CreateViewWithMakeUp(IID_CATIDftViewMakeUp, (void **)&piNewViewMU)))
      {
         if (piNewViewMU)
         {
            // Gets the view from the MakeUp
            CATIView  *piNewView = NULL; 
            if (SUCCEEDED(piNewViewMU->GetView(&piNewView)))
            {
               if (piNewView)
               {
                  // The view has to be typed: FrontView for Interactive view.  
                  piNewView->SetViewType(FrontView);
                  piNewViewMU->SetAxisData(100.0,50.0);
                  
                  // At last, add the view to the sheet
                  if (piDftNewSheet) piDftNewSheet->AddView(piNewViewMU);
                  
                  
                  // CREATING A GEOMETRIC ELEMENT IN THIS VIEW
                  // =======================================
                  
                  // Activates this new view in the new sheet
                  CATIDftView *piDftNewView=NULL;
                  if (SUCCEEDED(piNewView->QueryInterface(IID_CATIDftView,(void **)&piDftNewView)))
                  {
                     piDftNewSheet->SetDefaultActiveView(piDftNewView);
                     piDftNewView->Release();
                     piDftNewView=NULL;
                  }
                  
                  // Gets the Wire frame factory to create geometry.
                  CATI2DWFFactory_var spGeomFactory(piNewView);
                  
                  // Creation of a circle:
                  double center[2];
                  center[0]=50.0;
                  center[1]=60.0;
                  double radius = 50.0;
                  CATISpecObject_var spCercle;
                  if (NULL_var != spGeomFactory) 
                  {
                     spCercle = spGeomFactory->CreateCircle(center,radius);
                     // Circle has been created
                
                  }
                  
                  // Memory cleaning
                  piNewView->Release(); 
                  piNewView = NULL;
               }
            }
            piNewViewMU->Release(); 
            piNewViewMU = NULL;
         }
      }
      piDftNewSheet->Release();
      piDftNewSheet=NULL;
   }
            
   piDftDrawing->Release();
   piDftDrawing=NULL;

  
   

  return (CATStatusChangeRCCompleted);
}

//  Overload this method: when your command loses focus
//
// Deactivates a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand2::Desactivate( CATCommand * iFromClient, CATNotification * iEvtDat)
{
  return (CATStatusChangeRCCompleted);
}

//  Overload this method: when your command is canceled
//
// Cancel a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand2::Cancel( CATCommand * iFromClient, CATNotification * iEvtDat)
{
  RequestDelayedDestruction();
  return (CATStatusChangeRCCompleted);
}
