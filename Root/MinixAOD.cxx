#include "xAODPerfTools/MinixAOD.h"

#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"

#include "EventLoop/OutputStream.h"

#include "xAODAnaHelpers/tools/ReturnCheck.h"

// EDM includes:
#include "xAODEventInfo/EventInfo.h"

// this is needed to distribute the algorithm to the workers
ClassImp(MinixAOD)

MinixAOD :: MinixAOD () :
  m_event(nullptr),
  m_store(nullptr),
  m_metadata_tool(nullptr),
  m_trigger_metadata_tool(nullptr),
  m_event_count(0)
{}

EL::StatusCode MinixAOD :: setupJob (EL::Job& job)
{
  job.useXAOD ();

  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init(); // call before opening first file

  // outputs
  EL::OutputStream out_xAOD ("output_xAOD", "xAOD");
  job.outputAdd (out_xAOD);

  EL::OutputStream out_tree ("output_tree");
  job.outputAdd (out_tree);

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: changeInput (bool firstFile) { return EL::StatusCode::SUCCESS; }

EL::StatusCode MinixAOD :: initialize ()
{
  // assign m_event and m_store
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events = %lli", m_event->getEntries() ); // print long long int

  // output files
  TFile *file_tree = wk()->getOutputFile ("output_tree");
  //m_tree_maker = new TreeMaker(file_tree, m_doTruth, m_doRcJets, m_doAK10Jets, m_doTRF);

  TFile *file_xAOD = wk()->getOutputFile ("output_xAOD");
  if(!m_event->writeTo(file_xAOD).isSuccess()) return EL::StatusCode::FAILURE;

  // tools to store the meta data in the output mini-xAOD
  m_metadata_tool = new xAODMaker::FileMetaDataTool();
  if( !m_metadata_tool->initialize().isSuccess() ){
    Error("initialize()", "Cannot intialize FileMetaDataTool..." );
    Error("initialize()", "Exiting... " );
    return EL::StatusCode::FAILURE;
  }else{
    Info("initialize()","FileMetaDataTool initialized... " );
  }

  m_trigger_metadata_tool = new xAODMaker::TriggerMenuMetaDataTool();
  if( !m_trigger_metadata_tool->initialize().isSuccess() ){
    Error("initialize()", "Cannot intialize TriggerMenuMetaDataTool..." );
    Error("initialize()", "Exiting... " );
    return EL::StatusCode::FAILURE;
  }else{
    Info("initialize()","TriggerMenuMetaDataTool initialized... " );
  }

  if(m_debug) {
    RETURN_CHECK("initialize()", m_trigger_metadata_tool->setProperty( "OutputLevel", MSG::VERBOSE ), "" );
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: execute ()
{
  // code will go.
  m_event_count++;

  if(m_event_count % 1000 == 0)
    std::cout << "On event " << m_event_count << std::endl;

  m_event->copy("EventInfo");
  // we want trigger decisions in the output
  //  make sure metadata is also copied using xAODTrigCnv by initializing tool
  m_event->copy("xTrigDecision");
  m_event->copy("TrigConfKeys");

  m_event->fill();
  if(m_debug) std::cout << "end of event!" << std::endl;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: finalize ()
{
  // output xAOD
  TFile *file_xAOD = wk()->getOutputFile ("output_xAOD");
  if(!m_event->finishWritingTo( file_xAOD ).isSuccess()) return EL::StatusCode::FAILURE;

  if(m_metadata_tool) delete m_metadata_tool;
  if(m_trigger_metadata_tool) delete m_trigger_metadata_tool;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: histFinalize () { return EL::StatusCode::SUCCESS; }
