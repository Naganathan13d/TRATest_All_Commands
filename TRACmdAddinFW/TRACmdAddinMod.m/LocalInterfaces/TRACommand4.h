// COPYRIGHT Dassault Systemes 2022
//===================================================================
//
// TRACommand4.h
// The command: TRACommand4
//
//===================================================================
//
// Usage notes:
//
//===================================================================
//CAA2 Wizard Generation Report
//   COMMAND
//End CAA2 Wizard Generation Report
//
//  Aug 2022  Creation: Code generated by the CAA wizard  Naga
//===================================================================
#ifndef TRACommand4_H
#define TRACommand4_H

#include "CATCommand.h"

#include "TRAUtilities.h"
#include "CATIPrtContainer.h"

//----------------------------------------------------------------------

/**
 * Describe your command here.
 * <p>
 * Using this prefered syntax will enable mkdoc to document your class.
 * <p>
 * refer to programming resources of System framework.
 * (consult base class description).
 */
class TRACommand4: public CATCommand
{
  public:

  TRACommand4();
  virtual ~TRACommand4();

/**
 * Overload this method: when your command gains focus
 * <p>
 * Activates a command.
 * @param iFromClient 
 *   The command that requests to activate the current one.
 * @param iEvtDat
 *   The notification sent.
 */
   virtual CATStatusChangeRC Activate(
                            CATCommand * iFromClient,
                            CATNotification * iEvtDat);

/**
 *  Overload this method: when your command loses focus
 * <p>
 * Deactivates a command.
 * @param iFromClient 
 *   The command that takes the current active place.
 * @param iEvtDat
 *   The notification sent.
 */
   virtual CATStatusChangeRC Desactivate(
                            CATCommand * iFromClient,
                            CATNotification * iEvtDat);

/**
 *  Overload this method: when your command is canceled
 * <p>
 * Cancels a command.
 * @param iFromClient 
 *   The command that takes the current active place.
 * @param iEvtDat
 *   The notification sent.
 */
   virtual CATStatusChangeRC Cancel(
                            CATCommand * iFromClient,
                            CATNotification * iEvtDat);

   CATUnicodeString CreateGeomSetName();

   TRAUtilities* _pTRAUtilities;

   CATIPrtContainer * _piPartContainer;

};

//----------------------------------------------------------------------

#endif
