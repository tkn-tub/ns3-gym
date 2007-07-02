/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


int main (int argc, char *argv[])
{
  Bind ("RandomWalkMinSpeed", "2");
  Bind ("RandomWalkMaxSpeed", "3");
  Bind ("RandomWalkMode", "Time");
  Bind ("RandomWalkModeDistance", "20");
  Bind ("RandomWalkModeTime", "2s");

  RandomWalkPosition pos;
  
  return 0;
}
