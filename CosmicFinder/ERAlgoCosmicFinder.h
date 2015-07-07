/**
 * \file ERAlgoCosmicFinder.h
 *
 * \ingroup CosmicFinder
 * 
 * \brief Class def header for a class ERAlgoCosmicFinder
 *
 * @author matthias
 */

/** \addtogroup CosmicFinder

    @{*/

#ifndef ERTOOL_ERALGOCOSMICFINDER_H
#define ERTOOL_ERALGOCOSMICFINDER_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAlgoCosmicFinder
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoCosmicFinder : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoCosmicFinder(const std::string& name="ERAlgoCosmicFinder");

    /// Default destructor
    virtual ~ERAlgoCosmicFinder(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
