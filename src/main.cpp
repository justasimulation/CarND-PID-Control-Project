#include <iostream>

#include "driver.h"
#include "twiddle.h"

// Main method starts server comunicating with simulator and run the simulation with given coefficients

// There is a thread synchronization between server thread and twiddle thread so it was better
// to move all the sever logic into a separate class to incapsulate this synchronization and
// make the code more readable.

// Server is implemented in two classes:
// SingleClientServer - a "general" class that talks to the client
// Driver (derived from SingleClient) - here all the driving logic is located

int main()
{
    //Start the server
    Driver server;
    server.Start();

    //Synchronization is not implemented perfectly so need to wait on start
    sleep(2);

    //Run the simulation
    std::vector<double> p = {0.110385, 0.0751249, 0.000802641};
    std::cout<<"Calculated error: "<<server.CalcError(p[0], p[1], p[2], 1e+30, 1e+9)<<std::endl;

    // The commented code below is twiddle running and intermediate results

    /*
    //100 iterations throttle 1.0 speed 50
    //Values: p: 0.21 0.1 0 dp: 0.00373928 0.00305941 0.00250316
    std::vector<double> p = {0.21, 0.1, 0};
    std::vector<double> dp = {0.1, 0.1, 0.1};

    //200 iterations throttle 1.0 speed 50
    //Values: p: 0.11 0.0414771 0 dp: 0.00275347 0.00411321 0.00225284
    p = {0.11, 0.0414771, 0};

    //400 iterations throttle 1.0 speed 50
    //Values: p: 0.11742 0.0725602 0.00309032 dp: 0.00299853 0.00447929 0.00200728
    p = {0.11742, 0.0725602, 0.00309032};

    //600 iterations throttle 1.0 speed 50
    //p: 0.109403 0.0725602 0.00309032 dp: 0.00415476 0.00278128 0.00278128
    p = {0.109403, 0.0725602, 0.00309032};

    //Plus at 2: 0.00604277 + 0.000394844
    //Values: p: 0.109506 0.0860827 0.00643761 dp: 0.000648811 0.000530846 0.000394844
    p = {0.109506, 0.0860827, 0.00604277};


    p = {0.109403, 0.0725602, 0.00309032};


    //200 iterations throttle 1.0 speed 100
    //Values: p: 0.111127 0.0721102 0.00115322 dp: 3.81093e-05 3.11803e-05 2.55112e-05
    p = {0.111127, 0.0721102, 0.00115322};

    p = {0.03, 0.09, 0.0};
    dp = {0.1, 0.1, 0.1};


    //300 iterations throttle 1.0 speed 100
    //Values: p: 0.0671379 0.0716542 0 dp: 0.00336535 0.00411321 0.00225284

    p = {0.0671379, 0.0716542, 0};
    dp = {0.01, 0.01, 0.01};

    p = {0.109403, 0.0725602, 0.00309032};


    //new simulator 400 iterations fin tuning
    //Values: p: 0.110385 0.0751249 0.000802641
    p = {0.110385, 0.0751249, 0.000802641};

    std::cout<<"Calculated error: "<<server.CalcError(p[0], p[1], p[2], 1e+30, 1e+30)<<std::endl;
    //std::cout<<"Calculated error: "<<server.CalcError(p[0], p[1], p[2], 1000., 800)<<std::endl;
    //std::cout<<"Calculated error: "<<server.CalcError(p[0], p[1], p[2], 100000., 20050)<<std::endl;

    //Twiddle twiddle;
    //twiddle.Run(server, p, dp, 1e-3, 400);

     */

    return 0;
}
