#ifndef FEATURES_H
#define FEATURES_H

template <class T>

class Features{
  private:
    std::pair<T, T> feature1;
    std::pair<T, T> feature2;

  public:
    std::pair<T, T> GetFeature1(){
      return this->feature1;
    }

    std::pair<T, T> GetFeature2(){
      return this->feature2;
    }

    T GetFeature1First(){
      return this->feature1.first;
    }

    T GetFeature1Second(){
      return this->feature1.second;
    }

    T GetFeature2First(){
      return this->feature2.first;
    }

    T GetFeature2Second(){
      return this->feature2.second;
    }

    void SetFeature1(T param1, T param2){
      this->feature1.first = param1;
      this->feature1.second = param2;
    }

    void SetFeature2(T param1, T param2){
      this->feature2.first = param1;
      this->feature2.second = param2;
    }

    void SetFeature1First(T param){
      this->feature1.first = param;
    }

    void SetFeature1Second(T param){
      this->feature1.second = param;
    }

    void SetFeature2First(T param){
      this->feature2.first = param;
    }

    void SetFeature2Second(T param){
      this->feature2.second = param;
    }
};
#endif /* FEATURES_H */
