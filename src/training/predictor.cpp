#include <oxylus/training/predictor.cpp>



Predictor::Predictor() {

}

void Predictor::DeserializeTrees(ConfigurationObject* configObject) {
  std::string path = configObject->GetFolderOutput();
  tree0.Deserialize(path + "/tree0.dat");
  tree1.Deserialize(path + "/tree1.dat");
  tree2.Deserialize(path + "/tree2.dat");
  
}

void Predictor::DeserializeTree(std::string path) {

}
