#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <xAODJet/JetContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODEventInfo/EventInfo.h>
#include <AthContainers/ConstDataVector.h>

#include <xAODPerfTools/PerfJetHistsAlgo.h>
#include <xAODAnaHelpers/HelperFunctions.h>
#include <xAODAnaHelpers/HelperClasses.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include "TEnv.h"
#include "TSystem.h"

// this is needed to distribute the algorithm to the workers
ClassImp(PerfJetHistsAlgo)

PerfJetHistsAlgo :: PerfJetHistsAlgo ():
  m_inContainerName(""),
  m_inputAlgo("")
{}

EL::StatusCode PerfJetHistsAlgo :: setupJob (EL::Job& job)
{
  job.useXAOD();
  xAOD::Init("PerfJetHistsAlgo").ignore();
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PerfJetHistsAlgo :: histInitialize () { return EL::StatusCode::SUCCESS; }
EL::StatusCode PerfJetHistsAlgo :: fileExecute () { return EL::StatusCode::SUCCESS; }
EL::StatusCode PerfJetHistsAlgo :: changeInput (bool /*firstFile*/) { return EL::StatusCode::SUCCESS; }

EL::StatusCode PerfJetHistsAlgo::AddHists( std::string name ) {
  std::string fullname(m_name);
  fullname += name; // add systematic
  PerfJetHists* perfJetHists = new PerfJetHists( fullname ); // add systematic
  RETURN_CHECK("PerfJetHistsAlgo::AddHists", perfJetHists->initialize(), "");
  perfJetHists->record( wk() );
  m_plots[name] = perfJetHists;

  return EL::StatusCode::SUCCESS;
}


EL::StatusCode PerfJetHistsAlgo :: initialize ()
{
  Info("initialize()", m_name.c_str());

  m_event = wk()->xaodEvent();
  m_store = wk()->xaodStore();

  // only running 1 collection
  if(m_inputAlgo.empty()) { AddHists( "" ); }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PerfJetHistsAlgo :: execute ()
{
  const xAOD::EventInfo* eventInfo(nullptr);
  RETURN_CHECK("PerfJetHistsAlgo::execute()", HelperFunctions::retrieve(eventInfo, m_eventInfoContainerName, m_event, m_store, m_verbose) ,"");

  float eventWeight(1);
  if( eventInfo->isAvailable< float >( "mcEventWeight" ) ) {
    eventWeight = eventInfo->auxdecor< float >( "mcEventWeight" );
  }

  // this will hold the collection processed
  const xAOD::JetContainer* inJets = 0;

  // if input comes from xAOD, or just running one collection,
  // then get the one collection and be done with it
  if( m_inputAlgo.empty() ) {
    RETURN_CHECK("PerfJetHistsAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName, m_event, m_store, m_verbose) ,("Failed to get "+m_inContainerName).c_str());

    // if you loop over the jets and fill each jet individually, you will not get the jet multiplicity
    RETURN_CHECK("PerfJetHistsAlgo::execute()", m_plots[""]->execute( inJets, eventWeight ), "");

  } else { // get the list of systematics to run over

    // get vector of string giving the names
    std::vector<std::string>* systNames(nullptr);
    RETURN_CHECK("PerfJetHistsAlgo::execute()", HelperFunctions::retrieve(systNames, m_inputAlgo, 0, m_store, m_verbose) ,"");

    // loop over systematics
    for( auto systName : *systNames ) {
      RETURN_CHECK("PerfJetHistsAlgo::execute()", HelperFunctions::retrieve(inJets, m_inContainerName+systName, m_event, m_store, m_verbose) ,"");
      if( m_plots.find( systName ) == m_plots.end() ) { this->AddHists( systName ); }
      RETURN_CHECK("PerfJetHistsAlgo::execute()", m_plots[systName]->execute( inJets, eventWeight ), "");
    }

  }

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PerfJetHistsAlgo :: postExecute () { return EL::StatusCode::SUCCESS; }

EL::StatusCode PerfJetHistsAlgo :: finalize () {
  Info("finalize()", m_name.c_str());
  if(!m_plots.empty()){
    for( auto plots : m_plots ){
      if(plots.second) delete plots.second;
    }
  }
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode PerfJetHistsAlgo :: histFinalize () { return EL::StatusCode::SUCCESS; }
