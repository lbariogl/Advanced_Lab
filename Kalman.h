#ifndef _KALMAN_
#define _KALMAN_

#include <TMatrixF.h>

class Kalman
{
public:
  Kalman(float x_seed = 0, float y_seed = 0);
  void setRefPoint(float x_ref, float y_ref, float m_ref);
  void addMeasure(float m_meas, float q_meas, float m_err, float q_err);

  void printState();

private:
  TMatrixF A;
  TMatrixF B;
  TMatrixF H0;

  TMatrixF State;
  TMatrixF StateCov;

  TMatrixF Z; // measure
  TMatrixF V; // covariance matrix for the measurement

  TMatrixF Prev_State;
  TMatrixF Prev_StateCov;

  void setMatrixA(float m);
  void setMatrixB(float x);
  void setMatrixH0(float x, float y, float m);
};

#endif