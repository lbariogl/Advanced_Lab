#include "Kalman.h"
#include <iostream>

Kalman::Kalman(float x_seed, float y_seed) : H0_(2, 1), A_(2, 2), B_(2, 1), Z_(2, 1), V_(2, 2), State_(2, 1), Prev_State_(2, 1), StateCov_(2, 2), Prev_StateCov_(2, 2), Accumulation_(2, 1)
{
    //Prev_State_(0, 0) = x_seed;
    //Prev_State_(1, 0) = y_seed;
    Prev_StateCov_(0, 0) = 10000;
    Prev_StateCov_(0, 1) = 0;
    Prev_StateCov_(1, 0) = 0;
    Prev_StateCov_(1, 1) = 10000;
}

void Kalman::setMatrixA(float x, float y, float m)
{
    A_(0, 0) = -1 * m;
    A_(0, 1) = 1;
    A_(1, 0) = 0;
    A_(1, 1) = 0;
}

void Kalman::setMatrixB(float x, float y, float m)
{
    B_(0, 0) = -1 * x;
    B_(1, 0) = 1;
}

void Kalman::setMatrixH0(float x, float y, float m)
{
    //H0_(0, 0) = y - m * x;
    //H0_(1, 0) = m;
    H0_(0, 0) = 0;
    H0_(1, 0) = 0;
}

void Kalman::setRefPoint(float x_ref, float y_ref, float m_ref)
{
    //setMatrixH0(x_ref, y_ref, m_ref);
    setMatrixA(x_ref, y_ref, m_ref);
    setMatrixB(x_ref, y_ref, m_ref);
}

void Kalman::addMeasure(float m_meas, float q_meas, float m_err, float q_err)
{
    Z_(0, 0) = q_meas;
    Z_(1, 0) = m_meas;
    V_(0, 0) = q_err * q_err;
    V_(0, 1) = 0;
    V_(1, 0) = 0;
    V_(1, 1) = m_err * m_err;

    TMatrixF G(V_); 
    G.Invert(); // Inverse of the covariance matrix

    TMatrixF Btr(1, 2);
    Btr.Transpose(B_);

    TMatrixF W(1, 1);
    W = (Btr * G * B_).Invert(); 
    //std::cout << "W: " << std::endl;
    //W.Print();

    TMatrixF Gb(2, 2);
    Gb = G - G * B_ * W * Btr * G;

    
    TMatrixF Atr(2, 2);
    Atr.Transpose(A_);

    //std::cout << "A e Atr" << std::endl;
    //A_.Print();
    //Atr.Print();

    if (iteration_ == 0) {
        StateCov_ = V_;
    } else {
        Prev_StateCov_ = StateCov_;
        StateCov_ =  (Prev_StateCov_.Invert() +  Atr * Gb * A_).Invert();
        //TMatrixF Prev_StateCov_Inv(Prev_StateCov_);
        //Prev_StateCov_Inv.Invert();
        //StateCov_
    }

  



    //StateCov_ = Prev_StateCov_Inv + Atr * Gb * A_;
    //StateCov_.Invert();

    //std::cout << "Before measure" << std::endl;

    //State_ = StateCov_ * ((Prev_StateCov_Inv * Prev_State_) + (Atr * Gb * (Z_ - H0_)));
    //State_ = StateCov_ * ((Prev_StateCov_Inv * Prev_State_) + (Atr * Gb * (Z_)));

    if (iteration_ == 0) {
        Accumulation_ = StateCov_.Invert() * Z_;
        State_ = Z_;        
    } else {
        Accumulation_ += (Atr * Gb * (Z_));
        State_ = StateCov_ * (Accumulation_ );
    }

    //std::cout << "After measure" << std::endl;

    //Prev_State_ = State_;
    Prev_StateCov_ = StateCov_;
    ++iteration_;
}

void Kalman::printState()
{
    std::cout << "************************************************" << std::endl;
    std::cout << "Vertex position (x, y): (" << State_(0, 0) << ", " << State_(1, 0) << ")" << std::endl;
    std::cout << "Covariance matrix:";
    StateCov_.Print();
    //std::cout << StateCov_(0, 0) << " " << StateCov_(0, 1) << std::endl;
    //std::cout << StateCov_(1, 0) << " " << StateCov_(1, 1) << std::endl;
   
}