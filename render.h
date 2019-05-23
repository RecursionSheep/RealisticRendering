#pragma once

#include "util.h"

void PathTracing(Vector **canvas, int width, int height, int sampling);
void ProgressivePhotonMapping(Vector **canvas, int width, int height, int photonNum, int roundNum);
