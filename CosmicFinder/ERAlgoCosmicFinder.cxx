#ifndef ERTOOL_ERALGOCOSMICFINDER_CXX
#define ERTOOL_ERALGOCOSMICFINDER_CXX

#include "ERAlgoCosmicFinder.h"

namespace ertool {

  ERAlgoCosmicFinder::ERAlgoCosmicFinder(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoCosmicFinder::Reset()
  {}

  void ERAlgoCosmicFinder::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoCosmicFinder::ProcessBegin()
  {}

  bool ERAlgoCosmicFinder::Reconstruct(const EventData &data, ParticleGraph& graph) {
	  int showers = 0;
	  int tracks = 0;

	  std::cout << "Number of Tracks + Particles in event: " << graph.GetNumParticles() << std::endl;

	  // loop over particles
	  for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)){
		  auto const& shower = data.Shower(graph.GetParticle(p).RecoID());
		  std::cout << "SHOWER: time: " << shower._time << std::endl;
		  showers++;
	  }

	  // loop over tracks
	  std::cout << "Track info:" << std::endl;
	  for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
		  auto const& track = data.Track(graph.GetParticle(t).RecoID());

		  std::cout << "TRACK: length: " << track.Length() << " time: " << track._time
				  <<" start: " << track.back() << " end: " << track.front() << std::endl;
		  tracks++;
	  }
	  std::cout << "total showers in event: " << showers << std::endl;
	  std::cout << "total tracks in event: " << tracks << std::endl;
	  return true;
  }

  void ERAlgoCosmicFinder::ProcessEnd(TFile* fout)
  {}

}

#endif
