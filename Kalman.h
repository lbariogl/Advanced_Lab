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
  TMatrixF A_;
  TMatrixF B_;
  TMatrixF H0_;

  TMatrixF State_;
  TMatrixF StateCov_;

  TMatrixF Z_; // measure
  TMatrixF V_; // covariance matrix for the measurement

  TMatrixF Prev_State_;
  TMatrixF Prev_StateCov_;
  uint iteration_ = 0;
  TMatrix Accumulation_;

  void setMatrixH0(float x, float y, float m);
  void setMatrixA(float x, float y, float m);
  void setMatrixB(float x, float y, float m);

};

#endif