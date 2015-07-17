/**
 * \file ERAlgoCosmicAnalyzer.h
 *
 * \ingroup CosmicFinder
 * 
 * \brief Class def header for a class ERAlgoCosmicAnalyzer
 *
 * @author matthias
 */

/** \addtogroup CosmicFinder

    @{*/

#ifndef ERTOOL_ERALGOCOSMICANALYZER_H
#define ERTOOL_ERALGOCOSMICANALYZER_H

#include "ERTool/Base/AlgoBase.h"
#include "LArUtil/Geometry.h"
#include "GeoAlgo/GeoAlgo.h"

namespace ertool {

  /**
     \class ERAlgoCosmicAnalyzer
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoCosmicAnalyzer : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoCosmicAnalyzer(const std::string& name="ERAlgoCosmicAnalyzer");

    /// Default destructor
    virtual ~ERAlgoCosmicAnalyzer(){};

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

    void TouchTop(const EventData &data, ParticleGraph& graph);

    bool InSpill(const double& time, int& counter);
    
    bool ThroughTop(const ertool::Track&);
    
    // Function to get TPC penetration faces array=(top, bottom, anode, kathode, upstream, downstream)
    std::array<unsigned int, 6> ThroughFaces(const ertool::Track&);

    void SetSpill(double spilll_start, double spill_end){
    	_spill_start = spilll_start;
    	_spill_end = spill_end;
    }
  private:
    int _tracks_inspill = 0;
    int _showers_inspill = 0;

    int _tracks_touchtop = 0;

    double _spill_start;
    double _spill_end;

    bool _verbosity = false;
  };
}
#endif

/** @} */ // end of doxygen group 
