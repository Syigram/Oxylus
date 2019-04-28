#include <sstream>
#include <fstream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <oxylus/configuration/configuration_object.h>
#include <oxylus/training/thresholds_vector.h>
#include <oxylus/training/features_vector.h>
#include <oxylus/training/NodeMatrix.h>

using namespace rdf::bpc;

std::string filename = "/home/agonzalez/tfg/oxylus_github/build/config.ini";

rdf::ConfigurationObject* config = new rdf::ConfigurationObject(filename);
typedef std::vector<int>* ThresholdsVec;
typedef std::vector<Features>* FeaturesVec;



FeaturesVec InitializeFeaturesVector(){
  rdf::bpc::FeaturesVectorGenerator featuresVectorGen(config);
  std::vector<rdf::bpc::Features>* featuresVec = featuresVectorGen.GenerateVector();
  return featuresVec;

}

ThresholdsVec InitializeThresholdsVector(){
  rdf::bpc::ThresholdsVectorGenerator thresholdsVectorGen(config);
  std::vector<int>* thresholdsVec = thresholdsVectorGen.GenerateVector();
  return thresholdsVec;

}



BOOST_AUTO_TEST_SUITE(NodeMatrix)

  void testComparison(){
    {
      ThresholdsVec thresholdsVec = InitializeThresholdsVector();
      FeaturesVec featuresVec = InitializeFeaturesVector();
      rdf::bpc::NodeMatrix nodeMatrix_1;
      rdf::bpc::NodeMatrix nodeMatrix_2;
      nodeMatrix_1.SetThresholdsVector(thresholdsVec);
      nodeMatrix_1.SetFeaturesVector(featuresVec);
      nodeMatrix_2.SetThresholdsVector(thresholdsVec);
      nodeMatrix_2.SetFeaturesVector(featuresVec);
      nodeMatrix_1.InitCellHistograms();
      nodeMatrix_2.InitCellHistograms();
      BOOST_CHECK(nodeMatrix_1 == nodeMatrix_2);
    }
    {
      std::stringstream ss;
      std::ofstream ofs("filename");
      boost::archive::text_oarchive oa(ofs);

      ThresholdsVec thresholdsVec = InitializeThresholdsVector();
      FeaturesVec featuresVec = InitializeFeaturesVector();
      rdf::bpc::NodeMatrix nodeMatrix_1;
      nodeMatrix_1.SetThresholdsVector(thresholdsVec);
      nodeMatrix_1.SetFeaturesVector(featuresVec);
      nodeMatrix_1.InitCellHistograms();

      oa << nodeMatrix_1;

      std::ifstream ifs("filename");
      boost::archive::text_iarchive ia(ifs);
      rdf::bpc::NodeMatrix nodeMatrix_2;

      ia >> nodeMatrix_2;

      BOOST_CHECK(nodeMatrix_1 == nodeMatrix_2);

    }

  }

  BOOST_AUTO_TEST_CASE(Comparison)
  {
    testComparison();
  }

BOOST_AUTO_TEST_SUITE_END()
