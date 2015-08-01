#ifndef ERTOOL_ERALGOCOSMICANALYZER_CXX
#define ERTOOL_ERALGOCOSMICANALYZER_CXX

#include "ERAlgoCosmicAnalyzer.h"
#include <TH1.h>
#include <TCanvas.h>

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
  {
    _EnterFaces = {0, 0, 0, 0, 0, 0};
  }

  bool ERAlgoCosmicAnalyzer::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
		int showers_inspill = 0;
		int tracks_inspill = 0;

		int tracks_touchtop = 0;
		
		auto DetGeometry = ::larutil::Geometry::GetME();
		geoalgo::AABox DetectorBox(0.0,-DetGeometry->DetHalfHeight(),0.0,2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());


		// loop over showers
		for (auto const& p : graph.GetParticleNodes(RecoType_t::kShower)) {
			auto const& shower = data.Shower(graph.GetParticle(p).RecoID());
			InSpill(shower._time, showers_inspill);


		}
		
// 		std::cout << "New event!" << std::endl;

		// loop over tracks
		for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)) {
			auto const& track = data.Track(graph.GetParticle(t).RecoID());
			InSpill(track._time, tracks_inspill);
			 
// 			 if(ThroughTop(track)) 
// 				tracks_touchtop++;
			 std::vector<unsigned int> FaceVector = ThroughFaces(track);
			 
// 			 for(const auto& iter : FaceVector)
// 			 {
// 			   std::cout << iter << " ";
// 			 }
// 			 if(FaceVector.size()) std::cout << std::endl;
			 
			 if(FaceVector.size())
			 {
			   switch(FaceVector.front())
			   {
			       case top:
				 _EnterFaces.at(top)++;
				 graph.SetPrimary(kCosmicDaughter);
				 break;
			       case bottom:
				 _EnterFaces.at(bottom)++;
				 graph.SetPrimary(kCosmicDaughter);
				 break;
			       case anode:
				 _EnterFaces.at(anode)++;
				 graph.SetPrimary(kCosmicDaughter);
				 break;
			       case cathode:
				 _EnterFaces.at(cathode)++;
				 graph.SetPrimary(kCosmicDaughter);
				 break;
			       case upstream:
				 _EnterFaces.at(upstream)++;
				 graph.SetPrimary(kCosmicDaughter);
				 break;
			       case downstream:
				 _EnterFaces.at(downstream)++;
				 graph.SetPrimary(kCosmicDaughter);
				 break;
			       case inside:
				 _EnterFaces.at(inside)++;
			   }
			   
			   // Look for problematic tacks
			   if( FaceVector.front() == inside && !(FaceVector.back() == anode || FaceVector.back() == cathode) )
			   {			     
// 			     std::cout << FaceVector.size() << " " << FaceVector.front() << " " << FaceVector.back() << std::endl;
			     graph.SetPrimary(kCosmicDaughter);
			     _inside_frame++;
			   }
			     
			 }
			 
			 
// 			 std::cout << "DEBUG: start " << track.front() << std::endl;
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
    TH1I* FaceDiagram = new TH1I("Entry_Point","Entry_Point",_EnterFaces.size()+1,0,_EnterFaces.size()+1);
	  
    std::cout << "DATASET INFO" << std::endl;
    std::cout << "total showers in spill:    " << _showers_inspill << std::endl;
    std::cout << "total tracks in spill:     " << _tracks_inspill << std::endl;
    std::cout << "total tracks touching top: " << _tracks_touchtop << std::endl;
    std::cout << "Enter faces: ";
	  
    for(const auto& enterface : _EnterFaces)
    {
      std::cout << enterface << " ";
    }
    
    std::cout << std::endl;
    std::cout << "inside frame: " << _inside_frame << std::endl;
    
    for(unsigned int bin = 0; bin < _EnterFaces.size(); bin++)
    {
      FaceDiagram->SetBinContent(bin+1,_EnterFaces.at(bin));
    }
    
    FaceDiagram->SetBinContent(_EnterFaces.size()+1,_inside_frame);
    
    FaceDiagram->SetStats(0);
    FaceDiagram->GetXaxis()->SetBinLabel(1,"Top");
    FaceDiagram->GetXaxis()->SetBinLabel(2,"Bottom");
    FaceDiagram->GetXaxis()->SetBinLabel(3,"Anode");
    FaceDiagram->GetXaxis()->SetBinLabel(4,"Cathode");
    FaceDiagram->GetXaxis()->SetBinLabel(5,"Upstream");
    FaceDiagram->GetXaxis()->SetBinLabel(6,"Downstream");
    FaceDiagram->GetXaxis()->SetBinLabel(7,"Inside");
    FaceDiagram->GetXaxis()->SetBinLabel(8,"Inside w/ cut");
    FaceDiagram->GetYaxis()->SetTitle("Number of Events");
    
    
    TCanvas* C0 = new TCanvas("Entry Points","Entry Points",1000,500);
    FaceDiagram->Draw();
    C0 -> Print("FaceDiag.png","png");
    
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
  
  // Function to get TPC penetration faces array=(top, bottom, anode, kathode, upstream, downstream)
std::vector<unsigned int> ERAlgoCosmicAnalyzer::ThroughFaces(const Track& InputTrack)
{
  // Initialize output array
  std::vector<unsigned int> FacesVector;
  
  // Get detector geometry variables  
  auto DetGeometry = ::larutil::Geometry::GetME();
  
  // Construct detector box and Geometry algorithms
  geoalgo::AABox DetectorBox(0.0,-DetGeometry->DetHalfHeight(),0.0,2*DetGeometry->DetHalfWidth(),DetGeometry->DetHalfHeight(),DetGeometry->DetLength());
  geoalgo::GeoAlgo GeometryAlgo;
    
  // Find intersection points of the track with the box
  std::vector<geoalgo::Vector> IntersectionPoints = GeometryAlgo.Intersection(DetectorBox,InputTrack);
  
  float epsilon = std::numeric_limits<double>::round_error();
  
  if(DetectorBox.Contain(InputTrack.front()))
  {
    FacesVector.push_back(inside);
  }
  
  for(const auto& point : IntersectionPoints)
  {
    if(point.at(1) <= DetGeometry->DetHalfHeight() + epsilon && point.at(1) >= DetGeometry->DetHalfHeight() - epsilon) 
      FacesVector.push_back(top);
    else if(point.at(1) <= -DetGeometry->DetHalfHeight() + epsilon && point.at(1) >= -DetGeometry->DetHalfHeight() - epsilon)
      FacesVector.push_back(bottom);
    else if(point.at(0) <= 0.0 + epsilon && point.at(0) >= 0.0 - epsilon)
      FacesVector.push_back(anode);
    else if(point.at(0) <= 2*DetGeometry->DetHalfWidth() + epsilon && point.at(0) >= 2*DetGeometry->DetHalfWidth() - epsilon)
      FacesVector.push_back(cathode);
    else if(point.at(2) <= 0.0 + epsilon && point.at(2) >= 0.0 - epsilon)
      FacesVector.push_back(upstream);
    else if(point.at(2) <= DetGeometry->DetLength() + epsilon && point.at(2) >= DetGeometry->DetLength() - epsilon)
      FacesVector.push_back(downstream); 
  }
  
  if(DetectorBox.Contain(InputTrack.back()))
  {
    FacesVector.push_back(inside);
  }
  
//   if(FacesVector.size()>0 && FacesVector.front() == bottom) 
//   {
//     std::cout << InputTrack.front() << " " << InputTrack.back() << std::endl;
//     for(const auto& scheiss : IntersectionPoints)
//     {
//       std::cout << scheiss << std::endl;
//     }
//   }
  
//   if(IntersectionPoints.size() > 2) 
//   {
//     for(const auto& Faces : FacesVector)
//       std::cout << Faces << " ";
//   
//     std::cout <<  std::endl;
//   }
  
  return FacesVector;
}


  void ERAlgoCosmicAnalyzer::TouchTop(const EventData &data, ParticleGraph& graph)
  {

  }
}



#endif
