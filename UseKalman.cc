#include <TSystem.h>
#include "./Kalman.h"

void UseKalman()
{
    Kalman kf(0, 0);

    // For this test I am using a vertex located in (-1,2), 
    // in a box with dimensions 4x6 centered in (0,0).
    // The referent point is the intersection of the straight 
    // line with the side of the box: x = 2 , y = 2*m + q.

    // The three straight line of detection are:
    // a. y = 0 * x + 2  -> (m, q) = (0, 2)
    // b. y = -1 * x + 1 -> (m, q) = (-1, 1)
    // c. y = -2 * x + 0 -> (m, q) = (-2, 0)
    // d. y = -1 * x + 3 -> (m, q) = (1, 3)
    
    // The 3 refernce points
    // a. (x, y) = (2, 2)
    // b. (x, y) = (2, -1)
    // c. (x, y) = (2, -4)
    // d. (x, y) = (2, 5) 
   
    kf.setRefPoint(2, 2, 0);
    kf.addMeasure(0, 2, 0.1, 0.5); // m, q, m_err, q_err
    kf.printState();

    kf.setRefPoint(2, -1, -1);
    kf.addMeasure(-1, 1, 0.1, 0.5);
    kf.printState();

    kf.setRefPoint(2, -4, -2);
    kf.addMeasure(-2, 0, 0.1, 0.5);
    kf.printState();

    kf.setRefPoint(2, 5, 1);
    kf.addMeasure(1, 3, 0.1, 0.5);
    kf.printState();


}