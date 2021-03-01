#ifndef DREV_CONTROLLER_H
#define DREV_CONTROLLER_H

typedef struct  _controller             Controller;

struct _controller {
  unsigned int up;
  unsigned int down;
  unsigned int left;
  unsigned int right;
};

int Controller_Init(Controller *controller);

void Controller_Update(Controller *controller);

#endif // DREV_CONTROLLER_H