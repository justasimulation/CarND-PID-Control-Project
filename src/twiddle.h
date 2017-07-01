#ifndef PID_TWIDDLE_H
#define PID_TWIDDLE_H

#include "driver.h"

//Implemnetation of twiddle
class Twiddle
{
    public:
        //Runs optimization as it was described in the lectures
        void Run(Driver &server, std::vector<double> &p,
                 std::vector<double> &dp, double tolerance,
                 int max_num_iterations);
    private:
        void ReportValues(std::vector<double> &p, std::vector<double> &dp);
};

#endif //PID_TWIDDLE_H
