#ifndef TRAINER_H
#define TRAINER_H

#include <oxylus/image/image_operations.h>

class Trainer {
  public:
    Trainer();
    void TrainNode(std::shared_ptr<ImagesVector>);
    ~Trainer(){};

};

#endif /* TRAINER_H */
