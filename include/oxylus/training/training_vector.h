#ifndef TRAINING_VECTOR_H
#define TRAINING_VECTOR_H

#include <vector>

namespace rdf{
  namespace bpc{

    template <class T1>
    template <class T2>
    class TrainingVector{
      public:
        virtual std::vector<T1> GetVector() = 0;
        virtual void GenerateVector() = 0;
        virtual void PrintVector();
      protected:
        size_t size;
        T2 maxValue;
        T2 minValue;

        virtual void SetMaximumValue(T2 value){
          this->maxValue = value;
        };

        virtual void SetMinimumValue(T2 value){
          this->minValue = value;
        }

        virtual T2 GetMinimumValue(){
          return this->minValue;
        }

        virtual T2 GetMaximumValue(){
          return this->maxValue;
        }

        virtual void SetSize(size_t value){
          this->size = value;
        }

        virtual size_t GetSize(){
          return this->size;
        }

    }

  }
}

#endif /* TRAINING_VECTOR_H */
