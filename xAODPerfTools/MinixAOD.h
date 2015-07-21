#ifndef xAODPerfTools_MinixAOD_H
#define xAODPerfTools_MinixAOD_H

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

//Metadata
#include "xAODMetaDataCnv/FileMetaDataTool.h"
#include "xAODTriggerCnv/TriggerMenuMetaDataTool.h"

// trigger
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

class MinixAOD : public EL::Algorithm
{

  private:
    xAOD::TEvent *m_event; //!
    xAOD::TStore *m_store;  //!

    TrigDecisionTool          *m_trig_decision_tool; //!
    xAODConfigTool            *m_config_tool;        //!
    FileMetaDataTool          *m_metadata_tool;      //!
    TriggerMenuMetaDataTool   *m_trigger_metadata_tool; //!

    int m_event_count; //!

 public:
  MinixAOD ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(MinixAOD, 1);
};

#endif
