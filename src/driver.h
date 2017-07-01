#ifndef PID_SERVER_H
#define PID_SERVER_H

#include <condition_variable>
#include <list>

#include "single_client_server.h"
#include "PID.h"

//Implements driving using PID
//The main method is CalcError. It resets the scene and runs the simulation with given PID parameters
//for given number of iterations and while the total error is below given maximum error
class Driver : public SingleClientServer
{
    private:
        //When speed is higher we try to check some parameters to stay on track
        //when lower it is ok to drive without any checkings
        const double kDangerousSpeed = 25;
        //When speed is dangerous andabsolute value of cte is higher that this, should lower the speed
        const double kDangerousCte = 1.4;
        //When we change side of the target trajectory more often than this number in a certain
        // period the speed should be reduced because we're swinging
        const int kMaxNumSwings = 3;
        //When car crosses the distance [target - tolerance; target + tolerance] in one iteration
        //this is considered a change of side of the line and used to detect swinging.
        //Note that as we do not calc change per time, but per iteration this behaviour can change
        //on different hardware
        const double kSwingTolerance = 0.04;
        //Regular throttle
        const double kSpeed = 0.5;
        //Throttle value that is use for breaking
        const double kBreakSpeed = -1;
        //Number of last cte values to store to check swings
        const double kLastCteNum = 80;
        //When cte is higher than this value we should stop the simulation, because it is pointless
        const double kMaxCte = 4.5;

        PID pid_;

        //total cte for the run, we need this to stop simulation when the error is to high
        double total_error_;
        //number of iterations so far, when we twiddle we use a fixed number of iterations
        //so comparison between different PID parameters performance would be fair
        double num_iterations_;
        //List of last N cte values, we use this for detecting swings, when negative and positive ctes
        //differ for more than kDangerousCteAmplitude we should lower the speed
        std::list<double> last_cte_values;

        //maximum number iterations passed as an argument
        int max_num_iterations_;
        //maximum allowed total error
        double max_error_;

        //State of the server
        enum State
        {
            CollectingData,
            Idle
        } state_;

        //variables to syncronize with with client-server thread
        //when CalcError is called it initalizes PID params and waints on this variables
        //after that clien-server thread calls OnDataRecieved which may decide that the interaction
        //is over and unlock the variables.
        std::mutex mutex_;
        std::condition_variable condition_variable_;

    public:
        Driver();
        ~Driver() {}
        //Runs the simulation with given params and return the total cte of the run
        double CalcError(double p, double d, double i, double max_error, int max_num_iterations);

    protected:
        //Called when the connection with simulator is reset
        void OnReset() override;
        //Called when new telemetry data arrived
        void OnDataRecieved(double &cte, double &speed, double &steering_angle) override;


    private:
        //This method updates the list of last ctes,
        //finds how often car crossed the target trajectory with some tolerance.
        //This is done to detect swings.
        int UpdateLastCteAndGetSwingsNum(double cte);
};

#endif //PID_SERVER_H
