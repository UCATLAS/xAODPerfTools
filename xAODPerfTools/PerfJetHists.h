#ifndef xAODPerfTools_PerfJetHists_H
#define xAODPerfTools_PerfJetHists_H

#include "xAODAnaHelpers/HistogramManager.h"
#include <xAODJet/JetContainer.h>

class PerfJetHists : public HistogramManager
{
  public:
    PerfJetHists(std::string name);
    StatusCode initialize();
    StatusCode execute( const xAOD::JetContainer* jets, float eventWeight);
    StatusCode execute( const xAOD::Jet* jet, float eventWeight);
    // make other overloaded version of book() to show up in subclass
    using HistogramManager::book;
    using HistogramManager::execute; // overload

  private:
    TH1F* m_jetPt;                  //!
    TH1F* m_jetEta;                 //!
    TH1F* m_jetPhi;                 //!
    TH1F* m_jetM;                   //!
    TH1F* m_jetE;                   //!
    TH1F* m_jetRapidity;            //!
    TH1F* m_jetMultiplicity;        //!

};

#endif
