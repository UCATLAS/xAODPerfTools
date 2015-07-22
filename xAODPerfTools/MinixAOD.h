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

class MinixAOD : public EL::Algorithm
{
  public:
    bool m_debug;
    std::string m_copyNames;
    std::string m_deepCopyNames;

  private:
    std::vector<std::string> m_copyNames_vec; //!
    std::vector<std::string> m_deepCopyNames_vec; //!

    xAOD::TEvent *m_event; //!
    xAOD::TStore *m_store;  //!

    xAODMaker::FileMetaDataTool          *m_metadata_tool;      //!
    xAODMaker::TriggerMenuMetaDataTool   *m_trigger_metadata_tool; //!

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
