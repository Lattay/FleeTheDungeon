#include <stdlib.h>
#include <stdio.h>
#include "lsk_basics.h"

float lerp(float a, float b, float t){
  return (1 - t) * a + t * b;
}

void error(const char* msg){
  perror(msg);
  exit(1);
}

float vdist2(Vector2 v1, Vector2 v2){
  return (v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y);
}

float vdist(Vector2 v1, Vector2 v2){
  return sqrt(vdist2(v1, v2));
}


float vangle(Vector2 v1, Vector2 v2){
  float dot = vdot(v1, v2);
  float det = v1.x * v2.y - v1.y * v2.x;
  return atan2f(det, dot);
}

Vector2 vnormalized(Vector2 v){
  if(!vzero(v)){
    float n = vnorm(v);
    return (Vector2){v.x / n, v.y / n};
  } else {
    return (Vector2){1, 0};
  }
}
