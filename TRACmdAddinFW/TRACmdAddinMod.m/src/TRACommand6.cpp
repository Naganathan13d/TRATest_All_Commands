// COPYRIGHT Dassault Systemes 2022
//===================================================================
//
// TRACommand6.cpp
// The command: TRACommand6
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
#include "TRACommand6.h"

#include "CATCreateExternalObject.h"
#include "CATIPrtContainer.h"
#include "CATISpecObject.h"
#include "CATLISTV_CATBaseUnknown.h"
#include "CATIPartRequest.h"
#include "CATIDescendants.h"
#include "CATIVisProperties.h"
#include "CATVisPropertiesValues.h"
#include "CATVisPropertyType.h"
#include "CATVisGeomType.h"
#include "CATIModelEvents.h"
#include "CATCreate.h"
#include "CATDocumentServices.h"
#include "CATIRedrawEvent.h"
#include "CATPathElement.h"
#include "CATModifyVisProperties.h"















CATCreateClass( TRACommand6);


//-------------------------------------------------------------------------
// Constructor
//-------------------------------------------------------------------------
TRACommand6::TRACommand6() :
  CATCommand (NULL, "TRACommand6")
{
  RequestStatusChange (CATCommandMsgRequestExclusiveMode);
}

//-------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------
TRACommand6::~TRACommand6()
{
}


//  Overload this method: when your command gains focus
//
// Activates a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand6::Activate( CATCommand * iFromClient, CATNotification * iEvtDat)
{
	CATString DocumentName = "Current Document";

	CATIPrtContainer* pPrtContainer = pTRAUtilities->GetPartContainer();

	// 3-1 Retrieves the MechanicalPart
	//
	CATISpecObject_var spPart = pPrtContainer->GetPart();

	if ( NULL_var == spPart )
	{
		cout << "The part is NULL"<< endl;

	}

	// No more need of this interface
	pPrtContainer->Release();
	pPrtContainer = NULL ;

	// 3-2 Retrieves the OpenBody1
	//
	CATLISTV(CATBaseUnknown_var) surfBodies;  
	CATIPartRequest *pPartAsRequest = NULL;
	HRESULT rc = spPart->QueryInterface(IID_CATIPartRequest, (void**)&pPartAsRequest) ;

	if ( SUCCEEDED(rc) )
	{
		const CATUnicodeString stdContext(" "); // Sets the context for topo bodies lookup 

		// Extracts the lists of its surfacic mechanical bodies from the part ...
		pPartAsRequest->GetSurfBodies(stdContext, surfBodies);		

		if ( (1 != surfBodies.Size()) && ( NULL_var != surfBodies[1]) )
		{
			cout << "The document "<< DocumentName.CastToCharPtr() ;
			cout << " is not the original model"<< endl;

		}

		pPartAsRequest->Release();
		pPartAsRequest = NULL ;
	}else
	{
		cout << "Error in CATIPartRequest"<< endl;

	}

	CATIDescendants * pIDescendantOnOpenBody1 = NULL ;

	CATBaseUnknown_var spSurfBody = surfBodies[1];
	if(NULL_var == spSurfBody)
	{
		rc = E_FAIL;
	}
	else
	{  
		rc = spSurfBody->QueryInterface(IID_CATIDescendants, (void**)&pIDescendantOnOpenBody1) ;
	}

	if ( FAILED(rc) )
	{
		cout << "The document "<< DocumentName.CastToCharPtr() << " is not the original model"<< endl;

	}

	int nbelt = pIDescendantOnOpenBody1->GetNumberOfChildren();
	if ( 20 != nbelt )
	{
		cout << "The document "<< DocumentName.CastToCharPtr() << " is not the original model"<< endl;

	}

	// The point - Point.1
	CATISpecObject_var ThePoint = pIDescendantOnOpenBody1->GetChildAtPosition(1);

	// The line  - Line.1
	CATISpecObject_var TheLine = pIDescendantOnOpenBody1->GetChildAtPosition(8);

	// The yellow surface - Extrude.1
	CATISpecObject_var TheExtrude1Surface = pIDescendantOnOpenBody1->GetChildAtPosition(15);

	// The violet surface - Sweep.2
	CATISpecObject_var TheSweep2Surface = pIDescendantOnOpenBody1->GetChildAtPosition(10);

	// The hidden surface - Extrude.2 
	CATISpecObject_var TheExtrude2Surface = pIDescendantOnOpenBody1->GetChildAtPosition(20);

	if ( (NULL_var == ThePoint) || (NULL_var == TheLine) || (NULL_var == TheSweep2Surface) ||
		(NULL_var == TheExtrude1Surface) || (NULL_var == TheExtrude2Surface)  )
	{
		cout << "The document "<< DocumentName.CastToCharPtr() ; 
		cout << " is not the original model"<< endl;

	}

	// No more need of this interface pointer
	pIDescendantOnOpenBody1->Release();
	pIDescendantOnOpenBody1 = NULL ;

	// 4-Modifies the point
	//---------------------------------------------------

	cout << endl << "Case of Point.1 ----------"<< endl << endl ;

	CATIVisProperties * pIPropertiesOnPoint = NULL ;
	rc = ThePoint->QueryInterface(IID_CATIVisProperties, (void**)&pIPropertiesOnPoint) ;
	if ( FAILED(rc) )
	{
		cout << "Error with the point"<< endl;

	}

	// Is the point green ?
	CATVisPropertiesValues               MyPropertyOnPoint ;
	CATVisPropertyType                   PropTypeOnPoint = CATVPColor ;
	CATVisGeomType                       GeomTypeOnPoint     = CATVPPoint ;

	rc = pIPropertiesOnPoint->GetPropertiesAtt(MyPropertyOnPoint,
		PropTypeOnPoint,
		GeomTypeOnPoint);

	if ( rc == S_OK )
	{
		// The color of the point is not the standard
		unsigned int r,g,b ;
		MyPropertyOnPoint.GetColor(r,g,b);
		cout << "The color of Point.1 is not the standard color"<< endl ;
		cout << "Its color is  red= "<< r << " green="<< g << " blue=" << b << endl;

	}else
	{
		cout << "Error with the GetPropertiesAtt with the color point"<< endl;

	}

	// Is it a full square point ? 
	PropTypeOnPoint = CATVPSymbol;
	rc = pIPropertiesOnPoint->GetPropertiesAtt(MyPropertyOnPoint,
		PropTypeOnPoint,
		GeomTypeOnPoint);

	if ( rc == S_OK )
	{
		// The symbol of the point is not the standard
		CATSymbolType symbol ;
		MyPropertyOnPoint.GetSymbolType(symbol);
		cout << "The symbol of Point.1 is not the standard symbol"<< endl ;
		if ( symbol == FULLSQUARE )
		{
			cout << "Its symbol is a FullSquare "<< endl;
		}else 
		{
			cout <<" The point should be a full square point" << endl;
			cout <<" The value is " << symbol << endl;

		}
	}else
	{
		cout << "Error with GetPropertiesAtt with the symbol point"<< endl;

	}

	// The point becomes red 
	PropTypeOnPoint = CATVPColor ;
	MyPropertyOnPoint.SetColor(255,0,0);
	rc = pIPropertiesOnPoint->SetPropertiesAtt(MyPropertyOnPoint,
		PropTypeOnPoint,
		GeomTypeOnPoint);
	if ( E_FAIL == rc )
	{

	}

	// The point becomes a cross 
	PropTypeOnPoint = CATVPSymbol ;
	MyPropertyOnPoint.SetSymbolType(CROSS);
	rc = pIPropertiesOnPoint->SetPropertiesAtt(MyPropertyOnPoint,
		PropTypeOnPoint,
		GeomTypeOnPoint);

	if ( E_FAIL == rc )
	{
		cout <<" Error with the SetPropertiesAtt on the point" << endl;

	}

	cout << "The Point.1 is now red and its symbol is a cross"<< endl;

	
	cout << endl << "End of the case of Point.1 ----------"<< endl << endl ;

	pTRAUtilities->ObjectUpdate(ThePoint);

	 // Updates Display 
      CATIModelEvents * piModelEventPoint = NULL;                
      rc = ThePoint->QueryInterface(IID_CATIModelEvents, (void**)&piModelEventPoint);
      if (SUCCEEDED(rc))
      {
		  CATPathElement pPathElement;
		  CATModifyVisProperties notifPoint(ThePoint, pPathElement, GeomTypeOnPoint, PropTypeOnPoint, MyPropertyOnPoint); 
		 
         piModelEventPoint->Dispatch(notifPoint);

         piModelEventPoint->Release();
         piModelEventPoint=NULL;
      }

	 //  // Updates the specification tree
  //    CATIRedrawEvent * piRedrawEvent= NULL;                
  //    rc =spPart->QueryInterface(IID_CATIRedrawEvent, (void**)&piRedrawEvent);
  //    if (SUCCEEDED(rc))
  //    {
  //      piRedrawEvent->Redraw();
  //      
  //      piRedrawEvent->Release();
  //      piRedrawEvent= NULL ;
  //    }

	  // No more Need of this interface pointer
	pIPropertiesOnPoint->Release();
	pIPropertiesOnPoint = NULL;

	 // 9- Saves the document 
  //---------------------------------------------------------

  /*CATString DocumentSavecName = "D:\\Working\\CAAPropertyWithNewProperties.CATPart" ;

  CATDocument* pPartDocument = pTRAUtilities->GetCurrentDocument();
  

  rc = CATDocumentServices::SaveAs (*pPartDocument,DocumentSavecName.CastToCharPtr());
  if (S_OK == rc) 
  {
     cout << endl << "The Document "<< DocumentSavecName.CastToCharPtr() << " is saved OK" << endl ;
  }
  else
  {
     cout << "ERROR in saving document "<< DocumentSavecName.CastToCharPtr() << endl ;
     
  }*/

	 //---------------------------------------------------
  // 5-Modifies the line
  //---------------------------------------------------

  cout << endl << "Case of Line.1 ----------"<< endl << endl ;

   CATIVisProperties * pIPropertiesOnLine = NULL ;
  rc = TheLine->QueryInterface(IID_CATIVisProperties, (void**)&pIPropertiesOnLine) ;

  // Verify that the line is not an edge
  CATVisGeomType TypeLine = CATVPLine;
  CATVisGeomType TypeEdge = CATVPEdge;
  HRESULT rc1 = pIPropertiesOnLine->IsGeomTypeDefined(TypeLine);
  HRESULT rc2 = pIPropertiesOnLine->IsGeomTypeDefined(TypeEdge);
  if ( SUCCEEDED(rc1) && FAILED(rc2) )
  {
     cout << "Line.1 is a line not an edge"<< endl;
  }else
  {
     cout << "Line.1 should be a line not an edge" << endl;
     
  }

  // Is the type of the line a standard value for this feature ?
  CATVisPropertiesValues MyPropertyOnLine ;
  CATVisPropertyType PropTypeOnLine = CATVPLineType ;
  CATVisGeomType GeomTypeOnLine = CATVPLine ;

  rc = pIPropertiesOnLine->GetPropertiesAtt(MyPropertyOnLine,
                                            PropTypeOnLine,
                                            GeomTypeOnLine);
  if ( S_AUTOMATIC  == rc )
  {
     cout << "The line type of Line.1 is the standard "<< endl ;
     rc = pIPropertiesOnLine->GetStandardProperties(MyPropertyOnLine,
                                                PropTypeOnLine,
                                                GeomTypeOnLine);
     if ( SUCCEEDED(rc) )
     {
        unsigned int linetype ;
        MyPropertyOnLine.GetLineType(linetype);
        if ( 1 == linetype )
        {
           cout << "For a GSM feature the standard line type is SOLID"<<endl ; 
        }else
        {
           cout << "For a GSM feature the standard line type is "<< linetype << endl ;
        }
     }
     
  }else
  {
     cout << "Error with GetPropertiesAtt with the color line"<< endl;
    
  }

   // Sets the dotted line type for the line .
  MyPropertyOnLine.SetLineType(2);
  rc = pIPropertiesOnLine->SetPropertiesAtt(MyPropertyOnLine,
                                            PropTypeOnLine,
                                            GeomTypeOnLine);
 
  if ( FAILED(rc) )
  {
     cout << "Error with the SetPropertiesAtt method on the line" << endl;
     
  }

  // Sets the line no pickable
  // If you open the output model in an interactive session,test that the 
  // line is not highlighted when you pass over it
  MyPropertyOnLine.SetPickAttr(CATNoPickAttr);
  
  // With this type of geometry you can define : show/pick and layer property
  GeomTypeOnLine = CATVPGlobalType ;
  PropTypeOnLine = CATVPPick ;
  rc = pIPropertiesOnLine->SetPropertiesAtt(MyPropertyOnLine,
                                            PropTypeOnLine,
                                            GeomTypeOnLine);
 
  if ( FAILED(rc) )
  {
     cout <<" Error with the SetPropertiesAtt on the line" << endl;
    
  }

  cout << "The  Line.1 is now not pickable and dashed"<<endl ;

  // No more Need of this interface pointer
  pIPropertiesOnLine->Release();
  pIPropertiesOnLine = NULL;

 

  // Updates Display 
      CATIModelEvents * piModelEventLine = NULL;                
      rc = TheLine->QueryInterface(IID_CATIModelEvents, (void**)&piModelEventLine);
      if (SUCCEEDED(rc))
      {
		 CATPathElement pPathElement; 
		 CATModifyVisProperties notifLine(TheLine, pPathElement, GeomTypeOnLine, PropTypeOnLine, MyPropertyOnLine); 
		 
         piModelEventLine->Dispatch(notifLine);

         piModelEventLine->Release();
         piModelEventLine=NULL;
      }

	   cout << endl << "End of the case of Line.1 ----------"<< endl << endl ;



	   // 6-Modifies the sweep
  //-----------------------------------------------------------

  cout << endl << "Case of Sweep.2 ----------"<< endl << endl ;

  CATIVisProperties * pIPropertiesOnSurface2 = NULL ;
  rc = TheSweep2Surface->QueryInterface(IID_CATIVisProperties, (void**)&pIPropertiesOnSurface2) ;
  if ( FAILED(rc) )
  {
     cout << "Error with the surface TheSweep2Surface"<< endl;
     
  }
  
  // Verify the type of properties for a surface 
  CATVisGeomType TypePoint = CATVPPoint;
  CATVisGeomType TypeMesh = CATVPMesh;
  CATVisGeomType TypeGlobal = CATVPGlobalType;

  HRESULT rc3 = pIPropertiesOnSurface2->IsGeomTypeDefined(TypePoint);
  HRESULT rc4 = pIPropertiesOnSurface2->IsGeomTypeDefined(TypeEdge);
  HRESULT rc5 = pIPropertiesOnSurface2->IsGeomTypeDefined(TypeMesh);
  HRESULT rc6 = pIPropertiesOnSurface2->IsGeomTypeDefined(TypeGlobal);

  cout << "In this application the surface has the following geometry types="  ;
  if ( SUCCEEDED(rc3) )
  {
     cout << " CATVPPoint, " ;
  }
  if ( SUCCEEDED(rc4) )
  {
     cout << " CATVPEdge, " ;
  }
  if ( SUCCEEDED(rc5) )
  {
     cout << " CATVPMesh, " ;
  }
  if ( SUCCEEDED(rc6) )
  {
     cout << " CATVPGlobalType, " ;
  }
  cout << endl;

  // Modifications for the edge geometry
  CATVisPropertiesValues MyPropertyOnSurface2 ;
  CATVisPropertyType PropTypeOnSurface2 = CATVPAllPropertyType ;
  CATVisGeomType GeomTypeOnSurface2     = CATVPEdge ;

  // Initializes the MyPropertyOnSurface2 value
  rc = pIPropertiesOnSurface2->GetPropertiesAtt(MyPropertyOnSurface2,
                                                PropTypeOnSurface2,
                                                GeomTypeOnSurface2);
  if ( FAILED(rc) )
  {
     cout << "Error with the surface Sweep.2"<< endl;
     
  }

  // we can change the color and the width of the edge of the surface
  MyPropertyOnSurface2.SetColor(230,0,0);
  MyPropertyOnSurface2.SetWidth(4);

  rc = pIPropertiesOnSurface2->SetPropertiesAtt(MyPropertyOnSurface2,
                                                PropTypeOnSurface2,
                                                GeomTypeOnSurface2);

  if ( FAILED(rc) )
  {
     cout << "Error with the surface Sweep.2"<< endl;
     
  }

  // Modifications for the mesh geometry
  PropTypeOnSurface2 = CATVPAllPropertyType ;
  GeomTypeOnSurface2 = CATVPMesh ;

  // Initializes the MyPropertyOnSurface2 value
  rc = pIPropertiesOnSurface2->GetPropertiesAtt(MyPropertyOnSurface2,
                                                PropTypeOnSurface2,
                                                GeomTypeOnSurface2);
  if ( FAILED(rc) )
  {
     cout << "Error with the surface Sweep.2"<< endl;
     
  }

  // we can change the color and the opacity of the surface
  MyPropertyOnSurface2.SetColor(0,0,20);
  MyPropertyOnSurface2.SetOpacity(50);

  rc = pIPropertiesOnSurface2->SetPropertiesAtt(MyPropertyOnSurface2,
                                                PropTypeOnSurface2,
                                                GeomTypeOnSurface2);

  if ( FAILED(rc) )
  {
     cout << "Error with the surface Sweep.2 "<< endl;
     
  }

  cout << "The surface Sweep.2 is now transparency blue, with large edge in red "<< endl ;
  
  // No more Need of this interface pointer
  pIPropertiesOnSurface2->Release();
  pIPropertiesOnSurface2 = NULL;

  // Updates Display 
      CATIModelEvents * piModelEventSweep = NULL;                
      rc = TheLine->QueryInterface(IID_CATIModelEvents, (void**)&piModelEventSweep);
      if (SUCCEEDED(rc))
      {
		 CATPathElement pPathElement; 
		 CATModifyVisProperties notifLine(TheSweep2Surface, pPathElement, GeomTypeOnSurface2, PropTypeOnSurface2, MyPropertyOnSurface2); 
		 
         piModelEventSweep->Dispatch(notifLine);

         piModelEventSweep->Release();
         piModelEventSweep=NULL;
      }

  cout << endl << "End of the case of Sweep.2 ----------"<< endl << endl ;





	return (CATStatusChangeRCCompleted);
}

//  Overload this method: when your command loses focus
//
// Deactivates a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand6::Desactivate( CATCommand * iFromClient, CATNotification * iEvtDat)
{
  return (CATStatusChangeRCCompleted);
}

//  Overload this method: when your command is canceled
//
// Cancel a command.
//   iFromClient :The command that requests to activate the current one.
//   iEvtDat :The notification sent.
// ----------------------------------------------------
CATStatusChangeRC TRACommand6::Cancel( CATCommand * iFromClient, CATNotification * iEvtDat)
{
  RequestDelayedDestruction();
  return (CATStatusChangeRCCompleted);
}
