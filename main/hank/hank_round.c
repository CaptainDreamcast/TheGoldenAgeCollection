#include "hank_round.h"

static int gRound;

void resetHankRound() {
  gRound = 0;
}

void increaseHankRound() {
  gRound++;
}

int getHankRound() {
  return gRound;
}

