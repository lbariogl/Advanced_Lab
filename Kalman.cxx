#include "Kalman.h"
#include <iostream>

Kalman::Kalman(float x_seed, float y_seed) : H0(2, 1), A(2, 2), B(2, 1), Z(2, 1), V(2, 2), State(2, 1), Prev_State(2, 1), StateCov(2, 2), Prev_StateCov(2, 2)
{
    Prev_State(0, 0) = x_seed;
    Prev_State(1, 0) = y_seed;
    Prev_StateCov(0, 0) = 10000;
    Prev_StateCov(0, 1) = 0;
    Prev_StateCov(1, 0) = 0;
    Prev_StateCov(1, 1) = 10000;
}

void Kalman::setMatrixA(float m)
{
    A(0, 0) = -1 * m;
    A(0, 1) = 1;
    A(1, 0) = 0;
    A(1, 1) = 0;
}

void Kalman::setMatrixB(float x)
{
    B(0, 0) = -1 * x;
    B(1, 0) = 1;
}

void Kalman::setMatrixH0(float x, float y, float m)
{
    H0(0, 0) = 0;
    H0(1, 0) = 0;
}

void Kalman::setRefPoint(float x_ref, float y_ref, float m_ref)
{
    setMatrixA(0);
    setMatrixB(0);
}

void Kalman::addMeasure(float m_meas, float q_meas, float m_err, float q_err)
{
    Z(0, 0) = q_meas;
    Z(1, 0) = m_meas;
    V(0, 0) = q_err * q_err;
    V(0, 1) = 0;
    V(1, 0) = 0;
    V(1, 1) = m_err * m_err;

    TMatrixF G(V);
    G.Invert();

    TMatrixF Btr(1, 2);
    Btr.Transpose(B);

    TMatrixF W(1, 1);
    W = (Btr * G * B);
    W.Invert();

    TMatrixF Gb(2, 2);
    Gb = G - G * B * W * Btr * G;

    TMatrixF Atr(2, 2);
    Atr.Transpose(A);

    TMatrixF Prev_StateCov_Inv(Prev_StateCov);
    Prev_StateCov_Inv.Invert();

    StateCov = Prev_StateCov_Inv + Atr * Gb * A;
    StateCov.Invert();

    printf("Before measure\n");

    State = StateCov * ((Prev_StateCov_Inv * Prev_State) + (Atr * Gb * (Z - H0)));

    printf("After measure\n");

    Prev_State = State;
    Prev_StateCov = StateCov;
}

void Kalman::printState()
{
    std::cout << "Vertex position" << std::endl;
    printf("x: %f y: %f\n\n", State(0, 0), State(1, 0));
    std::cout << "Covariance matrix" << std::endl;
    std::cout << StateCov(0, 0) << " " << StateCov(0, 1) << std::endl;
    std::cout << StateCov(1, 0) << " " << StateCov(1, 1) << std::endl;
    std::cout << std::endl
              << "************************************************" << std::endl;
}