#ifndef ERTOOL_ERALGOCOSMICANALYZER_CXX
#define ERTOOL_ERALGOCOSMICANALYZER_CXX

#include "ERAlgoCosmicAnalyzer.h"

namespace ertool {

  ERAlgoCosmicAnalyzer::ERAlgoCosmicAnalyzer(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoCosmicAnalyzer::Reset()
  {}

  void ERAlgoCosmicAnalyzer::AcceptPSet(const ::fcllite::PSet& cfg)
  {
	std::cout << this->Name().c_str() << " called... " << std::endl;

	// Look for a configuration
	if (cfg.contains_pset(this->Name())) {
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

  void ERAlgoCosmicAnalyzer::ProcessBegin()
  {}

  bool ERAlgoCosmicAnalyzer::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    auto geom = ::larutil::Geometry::GetME();
    
//     	_DetWidth = 2 * geom->DetHalfWidth();
		int showers_inspill = 0;
		int tracks_inspill = 0;

		int tracks_touchtop = 0;

		// loop over showers
		for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)) {
			auto const& shower = data.Shower(graph.GetParticle(p).RecoID());
			InSpill(shower._time, showers_inspill);


		}

		// loop over tracks
		for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)) {
			auto const& track = data.Track(graph.GetParticle(t).RecoID());
			InSpill(track._time, tracks_inspill);
			 
			 if(ThroughTop(track)) 
				tracks_touchtop++;
			 
// 			 std::cout << "DEBUG: start " << trackStart[1] << " " << geom->DetHalfHeight() << std::endl;
// 			 std::cout << "DEBUG:   end " << trackEnd[1] << std::endl;

		}



		if (_verbosity){
			std::cout << "EVENT INFO" << std::endl;
			std::cout << " particles in event: "<< graph.GetNumParticles() << std::endl;
			std::cout << " showers in spill:   " << showers_inspill << std::endl;
			std::cout << " tracks in spill:    " << tracks_inspill << std::endl;
			std::cout << " tracks touch top:   " << tracks_touchtop << std::endl;
		}
		_showers_inspill += showers_inspill;
		_tracks_inspill += tracks_inspill;

		_tracks_touchtop += tracks_touchtop;

	return true;
  }

  void ERAlgoCosmicAnalyzer::ProcessEnd(TFile* fout)
  {
	  std::cout << "DATASET INFO" << std::endl;
	  std::cout << "total showers in spill:    " << _showers_inspill << std::endl;
	  std::cout << "total tracks in spill:     " << _tracks_inspill << std::endl;
	  std::cout << "total tracks touching top: " << _tracks_touchtop << std::endl;
  }


  bool ERAlgoCosmicAnalyzer::InSpill(const double& time, int& event_counter)
  {
	  if  ((time > _spill_start) && (time < _spill_end))
	  {
		  event_counter++;
		  return true;
	  }
	  else{
		  return false;
	  }
  }
  
  bool ERAlgoCosmicAnalyzer::ThroughTop(const ertool::Track& InputTrack)
  {
	  // Get detector geometry variables
	  auto DetGeometry = ::larutil::Geometry::GetME();
	  
	  // Construct detector box and Geometry algorithms
	  geoalgo::AABox DetectorBox(0.0,-DetGeometry->DetHalfHeight(),0.0,2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());
	  geoalgo::GeoAlgo GeometryAlgo;
	  
	  // Find intersection points of the track with the box
	  std::vector<geoalgo::Vector> IntersectionPoints = GeometryAlgo.Intersection(DetectorBox,InputTrack);
	  
	  // Return true if the track entered the top
	  if(IntersectionPoints.size() && IntersectionPoints.front().at(1) == DetGeometry->DetHalfHeight())
		  return true;
	  else 
		  return false;
  }

  void ERAlgoCosmicAnalyzer::TouchTop(const EventData &data, ParticleGraph& graph)
  {

  }
}



#endif
