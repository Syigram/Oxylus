/* #include <sstream> */
/* #include <fstream> */
/* #include <string> */

/* #include <boost/test/unit_test.hpp> */

/* #include <oxylus/configuration/configuration_object.h> */
/* #include <oxylus/training/thresholds_vector.h> */
/* #include <oxylus/training/features_vector.h> */
/* #include <oxylus/training/NodeMatrix.h> */

/* using namespace rdf::bpc; */

/* std::string filename = "/home/agonzalez/tfg/oxylus_github/build/config.ini"; */

/* rdf::ConfigurationObject* config = new rdf::ConfigurationObject(filename); */
/* typedef std::vector<int> ThresholdsVec; */
/* typedef std::vector<Features> FeaturesVec; */



/* FeaturesVec* InitializeFeaturesVector(){ */
/*   rdf::bpc::FeaturesVectorGenerator featuresVectorGen(config); */
/*   std::vector<rdf::bpc::Features>* featuresVec = featuresVectorGen.GenerateVector(); */
/*   return featuresVec; */

/* } */

/* ThresholdsVec* InitializeThresholdsVector(){ */
/*   rdf::bpc::ThresholdsVectorGenerator thresholdsVectorGen(config); */
/*   std::vector<int>* thresholdsVec = thresholdsVectorGen.GenerateVector(); */
/*   return thresholdsVec; */
/* } */



/* BOOST_AUTO_TEST_SUITE(NodeMatrix) */

/*   void testComparison(){ */
/*     { */

/*       /1* rdf::Matrix<rdf::bpc::Cell> histograms(10, 10); *1/ */
/*       /1* Cell newCell; *1/ */
/*       /1* rdf::Matrix<rdf::bpc::Cell> histograms = { {newCell, newCell}, {newCell, newCell} }; *1/ */
/*       /1* /2* rdf::Matrix<rdf::bpc::Cell> histograms(10, 10, rdf::bpc::Cell()); *2/ *1/ */
/*       /1* ThresholdsVec* thresholdsVec = InitializeThresholdsVector(); *1/ */
/*       /1* FeaturesVec* featuresVec = InitializeFeaturesVector(); *1/ */
/*       /1* rdf::bpc::NodeMatrix nodeMatrix_1; *1/ */
/*       /1* rdf::bpc::NodeMatrix nodeMatrix_2; *1/ */
/*       /1* nodeMatrix_1.SetThresholdsVector(thresholdsVec); *1/ */
/*       /1* nodeMatrix_1.SetFeaturesVector(featuresVec); *1/ */
/*       /1* nodeMatrix_2.SetThresholdsVector(thresholdsVec); *1/ */
/*       /1* nodeMatrix_2.SetFeaturesVector(featuresVec); *1/ */
/*       /1* nodeMatrix_1.cellHistograms = histograms; *1/ */
/*       /1* nodeMatrix_2.cellHistograms = histograms; *1/ */

/*       /1* /2* nodeMatrix_1.SetCellHistogram(histograms); *2/ *1/ */
/*       /1* /2* nodeMatrix_2.SetCellHistogram(histograms); *2/ *1/ */

/*       /1* BOOST_CHECK(nodeMatrix_1 == nodeMatrix_2); *1/ */
/*       /1* delete[] thresholdsVec; *1/ */
/*       /1* delete[] featuresVec; *1/ */
/*     } */

/*   } */

/*   BOOST_AUTO_TEST_CASE(Comparison) */
/*   { */
/*     testComparison(); */
/*   } */

/* void testSerialization(){ */

/*     { */
/*       /1* std::stringstream ss; *1/ */
/*       std::ofstream ofs("filename.txt"); */
/*       Cell newCell; */
/*       rdf::bpc::NodeMatrix nodeMatrix_1; */

/*       ThresholdsVec* thresholdsVec = InitializeThresholdsVector(); */
/*       FeaturesVec* featuresVec = InitializeFeaturesVector(); */
/*       nodeMatrix_1.SetThresholdsVector(thresholdsVec); */
/*       nodeMatrix_1.SetFeaturesVector(featuresVec); */
/*       rdf::Matrix<rdf::bpc::Cell> a = { {newCell, newCell}, {newCell, newCell} }; */
/*       nodeMatrix_1.cellHistograms = a; */
/*       BOOST_CHECK(nodeMatrix_1 == nodeMatrix_1); */
/*       { */
/*         boost::archive::text_oarchive oa(ofs); */
/*         oa << nodeMatrix_1; */
/*       } */

/*       rdf::bpc::NodeMatrix nodeMatrix_2; */
/*       { */
/*         std::ifstream ifs("filename.txt"); */
/*         boost::archive::text_iarchive ia(ifs); */
/*         ia >> nodeMatrix_2; */
/*       } */

/*       BOOST_CHECK(nodeMatrix_1 == nodeMatrix_2); */
/*       /1* delete[] thresholdsVec; *1/ */
/*       /1* delete[] featuresVec; *1/ */
/*     } */
/* } */



/*   BOOST_AUTO_TEST_CASE(Serialization) */
/*   { */
/*     testSerialization(); */
/*   } */

/* BOOST_AUTO_TEST_SUITE_END() */
