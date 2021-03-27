#ifndef LSK_BASICS_H
#define LSK_BASICS_H
#include <raylib.h>
#include <math.h>

/* Error routine */
void error(const char* msg);

/* Square of the cartesian distance between two points */
float vdist2(Vector2 v1, Vector2 v2);

/* Cartesian distance between two points */
float vdist(Vector2 v1, Vector2 v2);

/* Dot product */
static inline float vdot(Vector2 v1, Vector2 v2){
  return v1.x * v2.x + v1.y * v2.y;
}

/* Angle between two vectors */
float vangle(Vector2 v1, Vector2 v2);

static inline Vector2 vadd(Vector2 v1, Vector2 v2){
  return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

static inline Vector2 vsub(Vector2 v1, Vector2 v2){
  return (Vector2){v1.x - v2.x, v1.y - v2.y};
}

/* Return a rotated vector of angle (radians) */
static inline Vector2 vrot(Vector2 v, float angle){
  return (Vector2){cosf(angle) * v.x - sinf(angle) * v.y, sinf(angle) * v.x + cosf(angle) * v.y};
}

/* Return the square of the magnitude of the vector */
static inline float vnorm2(Vector2 v){
  return v.x * v.x + v.y * v.y;
}

/* Return the magnitude of the vector */
static inline float vnorm(Vector2 v){
  return sqrt(vnorm2(v));
}

/* Return true if the vector is stricly zero */
static inline bool vzero(Vector2 v){
  return v.x == 0.0 && v.y == 0.0;
}

/* Return true if the vector is almost zero (magnitude less than tol) */
static inline bool vzero_tol(Vector2 v, float tol){
  return vnorm2(v) < tol * tol;
}

static inline int max(int a, int b){
  return a > b ? a : b;
}

/* Return a vector with the direction of v and a magnitude of 1 */
Vector2 vnormalized(Vector2 v);

float lerp(float a, float b, float t);
#endif
