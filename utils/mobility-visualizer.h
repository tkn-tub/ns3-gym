/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include <vector>

int model_init (int argc, char *argv[], double *x1, double *y1, double *x2, double *y2);

struct NodeUpdate
{
    void *node;
    double x;
    double y;
    double vx;
    double vy;
};

struct ViewUpdateData
{
  std::vector<NodeUpdate> updateList;
  double time;
};

void view_update (ViewUpdateData *updateData);

#define SAMPLE_INTERVAL (1.0/100) // due to some race condition, this is the only value that works
