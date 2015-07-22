#include "xAODPerfTools/MinixAOD.h"

#include "EventLoop/Job.h"
#include "EventLoop/StatusCode.h"
#include "EventLoop/Worker.h"

#include "EventLoop/OutputStream.h"

// xAH includes
#include "xAODAnaHelpers/HelperFunctions.h"
#include "xAODAnaHelpers/tools/ReturnCheck.h"

// safeDeepCopy
#include "xAODPerfTools/tools/SafeDeepCopy.h"

// c++ include(s):
#include <iostream>
#include <sstream>

// EDM includes:
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"

namespace HF = HelperFunctions;

// this is needed to distribute the algorithm to the workers
ClassImp(MinixAOD)

MinixAOD :: MinixAOD () :
  m_debug(false),
  m_copyNames(""),
  m_deepCopyNames(""),
  m_copyNames_vec(),
  m_deepCopyNames_vec(),
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
EL::StatusCode MinixAOD :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode MinixAOD :: initialize ()
{
  // assign m_event and m_store
  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  Info("initialize()", "Number of events = %lli", m_event->getEntries() ); // print long long int

  // output files
  //TFile *file_tree = wk()->getOutputFile ("output_tree");
  //m_tree_maker = new TreeMaker(file_tree, m_doTruth, m_doRcJets, m_doAK10Jets, m_doTRF);

  TFile *file_xAOD = wk()->getOutputFile ("output_xAOD");
  RETURN_CHECK("initialize()", m_event->writeTo(file_xAOD), "Could not set output to file");

  // tools to store the meta data in the output mini-xAOD
  m_metadata_tool = new xAODMaker::FileMetaDataTool();
  RETURN_CHECK("initialize()", m_metadata_tool->initialize(), "Could not initialize FileMetaDataTool");
  Info("initialize()","FileMetaDataTool initialized... " );

  m_trigger_metadata_tool = new xAODMaker::TriggerMenuMetaDataTool();
  RETURN_CHECK("initialize()", m_trigger_metadata_tool->initialize(), "Could not initialize TriggerMenuMetaDataTool");
  Info("initialize()","TriggerMenuMetaDataTool initialized... " );

  if(m_debug)
    RETURN_CHECK("initialize()", m_trigger_metadata_tool->setProperty( "OutputLevel", MSG::VERBOSE ), "" );

  // parse and split by space
  std::string token;
  std::istringstream ss(m_copyNames);
  while ( std::getline(ss, token, ' ') ) {
    m_copyNames_vec.push_back(token);
  }
  ss.clear();
  ss.str(m_deepCopyNames);
  while( std::getline(ss, token, ' ') ) {
    m_deepCopyNames_vec.push_back(token);
  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: execute ()
{
  // code will go.
  m_event_count++;

  if(m_event_count % 20000 == 0)
    std::cout << "On event " << m_event_count << std::endl;

  for(auto contName: m_copyNames_vec){
    if(contName.empty()) continue;
    RETURN_CHECK("execute()", m_event->copy(contName), std::string("Could not copy "+contName+" over").c_str());
  }


  const xAOD::JetContainer* in_jets(nullptr);
  for(auto contName: m_deepCopyNames_vec){
    if(contName.empty()) continue;
    RETURN_CHECK("execute()", HF::retrieve(in_jets, contName, m_event, m_store, m_debug), std::string("Could not retrieve "+contName+". Enable m_debug to find out why.").c_str());

    // make the new containers to store deep copies
    xAOD::JetContainer* jets_copy(new xAOD::JetContainer);
    xAOD::JetAuxContainer* jets_copy_aux(new xAOD::JetAuxContainer);
    // connect the two
    jets_copy->setStore(jets_copy_aux);
    // deep copy time
    for(const auto jet: *in_jets){
      xAOD::Jet* jet_copy(new xAOD::Jet);
      jets_copy->push_back(jet_copy);
      xAOD::safeDeepCopy(*jet, *jet_copy);
    }
    // record to output xaod
    RETURN_CHECK("execute()", m_event->record(jets_copy, contName), std::string("Could not record "+contName+" to output xAOD.").c_str());
    RETURN_CHECK("execute()", m_event->record(jets_copy_aux, contName+"Aux."), std::string("Could not record "+contName+"Aux. to output xAOD.").c_str());

    // reset in_jets
    in_jets = nullptr;
  }

  m_event->fill();
  if(m_debug) std::cout << "end of event!" << std::endl;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: postExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode MinixAOD :: finalize ()
{
  // output xAOD
  TFile *file_xAOD = wk()->getOutputFile ("output_xAOD");
  RETURN_CHECK("finalize()", m_event->finishWritingTo( file_xAOD ), "Could not finish writing to the output xAOD");

  if(m_metadata_tool) delete m_metadata_tool;
  if(m_trigger_metadata_tool) delete m_trigger_metadata_tool;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode MinixAOD :: histFinalize () { return EL::StatusCode::SUCCESS; }
