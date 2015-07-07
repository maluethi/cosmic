/**
 * \file ERAlgoInSpill.h
 *
 * \ingroup CosmicFinder
 * 
 * \brief Class def header for a class ERAlgoInSpill
 *
 * @author matthias
 */

/** \addtogroup CosmicFinder

    @{*/

#ifndef ERTOOL_ERALGOINSPILL_H
#define ERTOOL_ERALGOINSPILL_H

#include "ERTool/Base/AlgoBase.h"

namespace ertool {

  /**
     \class ERAlgoInSpill
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoInSpill : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoInSpill(const std::string& name="ERAlgoInSpill");

    /// Default destructor
    virtual ~ERAlgoInSpill(){};

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

    void SetSpill(double spilll_start, double spill_end){
    	_spill_start = spilll_start;
    	_spill_end = spill_end;
    }
  private:
    int _tracks_inspill;
    int _showers_inspill;

    double _spill_start;
    double _spill_end;

    bool _verbosity = false;
  };

}
#endif

/** @} */ // end of doxygen group 
