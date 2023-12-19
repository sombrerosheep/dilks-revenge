#ifndef DREV_CONTROLLER_H
#define DREV_CONTROLLER_H

typedef struct drev_controller Controller;

struct drev_controller {
  unsigned int up;
  unsigned int down;
  unsigned int left;
  unsigned int right;
  unsigned int space;
  unsigned int mouse_left;
  float mouse_x;
  float mouse_y;
};

int Controller_Init(Controller *controller);

void Controller_Update(Controller *controller);

#endif // DREV_CONTROLLER_H
