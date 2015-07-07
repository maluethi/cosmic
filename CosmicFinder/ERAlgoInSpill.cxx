#ifndef ERTOOL_ERALGOINSPILL_CXX
#define ERTOOL_ERALGOINSPILL_CXX

#include "ERAlgoInSpill.h"

namespace ertool {

  ERAlgoInSpill::ERAlgoInSpill(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoInSpill::Reset()
  {}

  void ERAlgoInSpill::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    std::cout << this->Name().c_str() << " called... " << std::endl;

    // Look for a configuration
    if(cfg.contains_pset(this->Name())) {
		auto const& pset = cfg.get_pset(this->Name());
		std::cout << "Reading config from: " << this->Name() << std::endl;
		_spill_start = pset.get<double>("spill_start");
		_spill_end = pset.get<double>("spill_end");

		_verbosity = pset.get<bool>("verbosity");

	} else {
		std::cout << "cannot find configuration file (" << this->Name() << ")"
				<< std::endl;
		throw ERException();
    }
  }

  void ERAlgoInSpill::ProcessBegin()
  {
	  _tracks_inspill = 0;
	  _showers_inspill = 0;
  }

  bool ERAlgoInSpill::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
	int showers = 0;
	int tracks = 0;



	// loop over showers
	for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)) {
		auto const& shower = data.Shower(graph.GetParticle(p).RecoID());

		if  ((shower._time > _spill_start) && (shower._time < _spill_end)) {
			showers++;
		}
	}

	// loop over tracks
	for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)) {
		auto const& track = data.Track(graph.GetParticle(t).RecoID());
		if  ((track._time > _spill_start) && (track._time < _spill_end)) {
			tracks++;
		}
	}
	if (_verbosity){
		std::cout << "EVENT INFO" << std::endl;
		std::cout << "total particles in event: "<< graph.GetNumParticles() << std::endl;
		std::cout << "total showers in spill: " << showers << std::endl;
		std::cout << "total tracks in spill: " << tracks << std::endl;
	}
	_tracks_inspill += tracks;
	_showers_inspill += showers;

	return true;
  }

  void ERAlgoInSpill::ProcessEnd(TFile* fout)
  {
	  std::cout << "DATASET INFO" << std::endl;
	  std::cout << "total showers in spill: " << _showers_inspill << std::endl;
	  std::cout << "total tracks in spill: " << _tracks_inspill << std::endl;
  }

}

#endif
