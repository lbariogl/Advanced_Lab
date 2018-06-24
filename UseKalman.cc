#include <TSystem.h>
#include "./Kalman.h"

void UseKalman()
{
    gSystem->CompileMacro("Kalman.cxx", "kgf");
    Kalman kf(0, 0);

    /// For this test I am using a vertex located in (-1,2), in a box with dimensions 4x6 centered in (0,0). The referent point is the intersection of the straight line with the side of the box: x = 2 , y = 2*m + q.

    // The three straight line of detection are:
    // a. y = 0 * x + 2
    // b. y = -1 * x + 1
    // c. y = -2 * x + 0
    // May the forse be with us

    // Measure a

    kf.setRefPoint(2, 2, 0);
    kf.addMeasure(0, 2, 0.1, 0.5);
    kf.printState();
    kf.setRefPoint(2, -1, -1);
    kf.addMeasure(-1, 1, 0.1, 0.5);
    kf.printState();
    kf.setRefPoint(2, -4, -2);
    kf.addMeasure(-2, 0, 0.1, 0.5);
    kf.printState();
}