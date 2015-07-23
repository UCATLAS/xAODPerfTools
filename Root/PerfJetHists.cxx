#include <xAODPerfTools/PerfJetHists.h>
#include <sstream>

#include "xAODAnaHelpers/tools/ReturnCheck.h"


PerfJetHists :: PerfJetHists (std::string name) :
  HistogramManager(name, "")
{}

StatusCode PerfJetHists::initialize() {
  m_jetPt           = book(m_name, "jetPt",  "jet p_{T} [GeV]", 120, 0, 3000.);
  m_jetEta          = book(m_name, "jetEta", "jet #eta",         80, -4, 4);
  m_jetPhi          = book(m_name, "jetPhi", "jet Phi",120, -TMath::Pi(), TMath::Pi() );
  m_jetM            = book(m_name, "jetMass", "jet Mass [GeV]",120, 0, 400);
  m_jetE            = book(m_name, "jetEnergy", "jet Energy [GeV]",120, 0, 4000.);
  m_jetRapidity     = book(m_name, "jetRapidity", "jet Rapidity",120, -10, 10);
  m_jetMultiplicity = book(m_name, "jetMultiplicity", "jet Multiplicity", 101, -0.5, 95.5);

  return StatusCode::SUCCESS;
}

StatusCode PerfJetHists::execute( const xAOD::JetContainer* jets, float eventWeight ) {
  for( auto jet_itr : *jets )
    RETURN_CHECK("PerfJetHists::execute()", this->execute( jet_itr, eventWeight ), "");

  m_jetMultiplicity->Fill(jets->size(), eventWeight);

  return StatusCode::SUCCESS;
}

StatusCode PerfJetHists::execute( const xAOD::Jet* jet, float eventWeight) {
  m_jetPt ->        Fill( jet->pt()/1e3,    eventWeight );
  m_jetEta->        Fill( jet->eta(),       eventWeight );
  m_jetPhi->        Fill( jet->phi(),       eventWeight );
  m_jetM->          Fill( jet->m()/1e3,     eventWeight );
  m_jetE->          Fill( jet->e()/1e3,     eventWeight );
  m_jetRapidity->   Fill( jet->rapidity(),  eventWeight );

  return StatusCode::SUCCESS;
}

