// COPYRIGHT Dassault Systemes 2022
//===================================================================
//
// TRACommand5.cpp
// The command: TRACommand5
//
//===================================================================
//
// Usage notes:
//
//===================================================================
//
//  Aug 2022  Creation: Code generated by the CAA wizard  Naga
//===================================================================
#include <iostream>
using namespace std;
#include "TRACommand5.h"

#include "CATIStringList.h" 
#include "CATUnicodeString.h"
#include "CATListOfCATUnicodeString.h"
#include "CATBaseUnknown.h" 
#include "IUnknown.h" 

// ObjectModelerBase 
#include "CATSession.h" 
#include "CATSessionServices.h" 
#include "CATInit.h"
#include "CATDocument.h"
#include "CATDocumentServices.h"
#include "CATIContainer.h"
#include "CATISpecObject.h"

// MechModInterfaces
#include "CATIPrtPart.h"
#include "CATIPrtContainer.h"

// Drafting2DLInterfaces 
#include "CAT2DLayoutServices.h"
#include "CATI2DLayoutLayout.h"
#include "CATI2DLayoutSheet.h"
#include "CATI2DLayoutView.h"

// CATTPSInterfaces
#include "CATITPSDocument.h"
#include "CATITPSList.h"
#include "CATITPSComponent.h"
#include "CATITPSSet.h"
#include "CATITPSView.h"
#include "CATITPSCapture.h"
#include "CATITPSCaptureList.h"

#include "CATCreateExternalObject.h"
CATCreateClass( TRACommand5);


//-------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------
TRACommand5::TRACommand5() :
  CATCommand (NULL, "TRACommand5")
{
  RequestStatusChange (CATCommandMsgRequestExclusiveMode);
}

//-------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------
TRACommand5::~TRACommand5()
{
}


//  Overload this method: when your command gains focus
//
// Activates a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand5::Activate( CATCommand * iFromClient, CATNotification * iEvtDat)
{

	CATDocument* pDocPart = _pTRAUtilities->GetCurrentDocument();

	CATInit_var spInitOnDoc(pDocPart);
	if(NULL_var != spInitOnDoc)
	{
		// Retrieves the TPS Component: The TPS Set
		//=========================================
		CATITPSDocument *piTPSDoc= NULL;
		CATITPSSet *piTPSSet = NULL;
		if (SUCCEEDED(spInitOnDoc->QueryInterface(IID_CATITPSDocument,(void **)&piTPSDoc)) && piTPSDoc)
		{
			CATITPSList *piTPSList = NULL;
			if (SUCCEEDED(piTPSDoc->GetSets(&piTPSList)) && piTPSList)
			{
				unsigned int nbSet;
				piTPSList->Count(&nbSet);
				// Only one set in the Part Document
				if (nbSet > 0)
				{
					CATITPSComponent *piTPSCmp = NULL;
					if (SUCCEEDED(piTPSList->Item(0,&piTPSCmp)) && piTPSCmp)
					{
						piTPSCmp->QueryInterface(IID_CATITPSSet,(void **)&piTPSSet);
						if (piTPSSet)
							cout << " The active TPS Set is correctly retrieved" << endl;
						piTPSCmp->Release();piTPSCmp=NULL;
					}
				}
				piTPSList->Release();piTPSList=NULL;
			}
			piTPSDoc->Release();piTPSDoc=NULL;
		}

		// Retrieves the Part Root Container
		// =================================
		CATIPrtContainer * piPrtCont =  (CATIPrtContainer*) spInitOnDoc->GetRootContainer("CATIPrtContainer");	
		if (piPrtCont)
		{
			// Get the part feature of the container.
			CATIPrtPart_var spPart = piPrtCont->GetPart();

			// Creates the 2DLayout Root
			// =========================
			if (NULL_var != spPart)
			{
				CATI2DLayoutLayout *pi2DLayout = NULL;
				cout << "call CAT2DLayoutServices::GetLayout2DL" << endl;
				CAT2DLayoutServices::GetLayout2DL(spPart,&pi2DLayout);

				if (!pi2DLayout)
				{
					cout << "No 2DL. A 2DL must be created "<< endl;
					CATListOfCATUnicodeString lstStd;
					if (SUCCEEDED(CAT2DLayoutServices::GetAvailable2DLayoutStandards(lstStd)))
					{
						cout << "nb std in the environment = " << lstStd.Size() << endl;
						for ( int indice = 1 ; indice  <= lstStd.Size()  ; indice ++ )
							cout << "lstStd ("<< indice << ") = " << lstStd[indice].ConvertToChar() << endl;
						// The fifth standard defined in the list is choosen (ISO_3D)
						// The 2DL standard and FTA standard must be identical
						CATUnicodeString myStd = lstStd[5];
						cout << " myStd = " << myStd.ConvertToChar() << endl;

						CATListOfCATUnicodeString lstSheetStyle;
						if (SUCCEEDED(CAT2DLayoutServices::GetListOfSheetStyles(myStd,lstSheetStyle)))
						{
							for (int indice = 1 ; indice  <= lstSheetStyle.Size()  ; indice ++ )
								cout << "lstSheetStyle ("<< indice << ") = " << lstSheetStyle[indice].ConvertToChar() << endl;
						}
						CATUnicodeString mySheetStyle = lstSheetStyle[2];
						cout << " mySheetStyle = " << mySheetStyle.ConvertToChar() << endl;

						// Creates the 2DLayuout Root
						if (SUCCEEDED(CAT2DLayoutServices::CreateLayout2DL(spPart,myStd, mySheetStyle,&pi2DLayout)) && pi2DLayout)
							cout << " The 2DL is correctly created" << endl;
					}
				}
				// Retrieves the 2DL Sheet
				// =======================
				if (pi2DLayout)
				{
					CATI2DLayoutSheet *pi2DLSheet = NULL;
					pi2DLayout->GetActiveSheet(&pi2DLSheet);
					if (pi2DLSheet)
						cout << " The active 2DL Sheet is correctly retrieved" << endl;

					// Retrieves the FTA Active View and the Capture
					// =============================================
					CATBaseUnknown *piTPSViewBU = NULL;
					CATBaseUnknown *piTPSCaptureBU1 = NULL;
					CATBaseUnknown *piTPSCaptureBU2 = NULL;

					if (piTPSSet)
					{
						CATITPSView *piTPSView = NULL; 
						piTPSSet->GetActiveView(&piTPSView);
						if (piTPSView)
						{
							piTPSView->QueryInterface(IID_CATBaseUnknown,(void **)&piTPSViewBU);
							piTPSView->Release();piTPSView=NULL;
							cout << " The active TPS View is correctly retrieved" << endl;
						}
						CATITPSCaptureList *piTPSCaptList = NULL;
						piTPSSet->GetCaptures (&piTPSCaptList);
						piTPSSet->Release();piTPSSet=NULL;
						if (piTPSCaptList)
						{
							unsigned int nbCapt;
							piTPSCaptList->Count(&nbCapt);
							// there is at least one capture in the Part Document
							if (nbCapt > 0)
							{
								CATITPSCapture *piTPSCapture = NULL;
								if (SUCCEEDED(piTPSCaptList->Item(0,&piTPSCapture)) && piTPSCapture)
								{
									piTPSCapture->QueryInterface(IID_CATBaseUnknown,(void **)&piTPSCaptureBU1);
									cout << " The first capture is correctly retrieved" << endl;
									piTPSCapture->Release();piTPSCapture=NULL;
								}
								if (SUCCEEDED(piTPSCaptList->Item(1,&piTPSCapture)) && piTPSCapture)
								{
									piTPSCapture->QueryInterface(IID_CATBaseUnknown,(void **)&piTPSCaptureBU2);
									cout << " The 2nd capture is correctly retrieved" << endl;
									piTPSCapture->Release();piTPSCapture=NULL;
								}
							}
							piTPSCaptList->Release();piTPSCaptList=NULL;
						}
					}

					// 2DL View Creation from FTA Active View
					// ======================================
					if (pi2DLSheet)
					{
						double pt1Coord[2] = {-400.0,600.0};
						CATI2DLayoutView *pi2DLayoutView = NULL;
						cout << "Start the 2DL view creation from 3D FTA View"  << endl;
						if (SUCCEEDED(pi2DLSheet->CreateView2DLFromFTA (piTPSViewBU, pt1Coord, &pi2DLayoutView)))
						{

							cout << "View has been correctly created" << endl;
							pi2DLayoutView->Release();pi2DLayoutView=NULL;
						}

						double pt2Coord[2] = {-400.0,10.0};
						CATI2DLayoutView *pi2DLayoutView1 = NULL;
						cout << "Start the 2DL view creation from the 1st 3D FTA Capture"  << endl;
						if (SUCCEEDED(pi2DLSheet->CreateView2DLFromFTA (piTPSCaptureBU1, pt2Coord, &pi2DLayoutView1)))
						{

							cout << "View has been correctly created" << endl;
							pi2DLayoutView1->Release();pi2DLayoutView1=NULL;
						}

						double pt3Coord[2] = {-400.0,800.0};
						CATI2DLayoutView *pi2DLayoutView2 = NULL;
						cout << "Start the 2DL view creation from the 2nd 3D FTA Capture"  << endl;
						// This view can not be created because the capture is not associated to a view annotation plane
						if (SUCCEEDED(pi2DLSheet->CreateView2DLFromFTA (piTPSCaptureBU2, pt3Coord, &pi2DLayoutView2)))
						{
							cout << "View has been correctly created" << endl;
							pi2DLayoutView2->Release();pi2DLayoutView2=NULL;
						}
						else
						{
							 
							cout << "The View can not be created" << endl;
						}
						pi2DLSheet->Release();pi2DLSheet=NULL;
					}
					// Return code management
					// ======================
					
					if (piTPSViewBU) piTPSViewBU->Release(),piTPSViewBU=NULL;
					if (piTPSCaptureBU1) piTPSCaptureBU1->Release(),piTPSCaptureBU1=NULL;
					if (piTPSCaptureBU2) piTPSCaptureBU2->Release(),piTPSCaptureBU2=NULL;

					pi2DLayout->Release();pi2DLayout=NULL;
				}
			}
			piPrtCont->Release();piPrtCont=NULL;
		}
	}

	// SAVE THE RESULT
	// =================

	CATUnicodeString pfileNameOut = "D:\\Working\\TestResult.CATDrawing";
	CATDocumentServices::SaveAs(*pDocPart, pfileNameOut);
	CATDocumentServices::Remove (*pDocPart);

	return (CATStatusChangeRCCompleted);


}


//  Overload this method: when your command loses focus
//
// Deactivates a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand5::Desactivate( CATCommand * iFromClient, CATNotification * iEvtDat)
{
  return (CATStatusChangeRCCompleted);
}

//  Overload this method: when your command is canceled
//
// Cancel a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand5::Cancel( CATCommand * iFromClient, CATNotification * iEvtDat)
{
  RequestDelayedDestruction();
  return (CATStatusChangeRCCompleted);
}
