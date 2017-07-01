#include <iostream>
#include <chrono>

#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double kp, double kd, double ki)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;

    p_error_ = 0;
    d_error_ = 0;
    i_error_ = 0;

    num_updates_ = 0;
}

void PID::UpdateError(double cte)
{
    std::chrono::high_resolution_clock::time_point cur_time = std::chrono::high_resolution_clock::now();
    if(num_updates_++ > 0)
    {
        int ms = std::chrono::duration_cast<std::chrono::milliseconds>(cur_time - prev_time_).count();
        double sec = ms/1000.;
        d_error_ = (cte - p_error_)/sec;
    }
    else
    {
        d_error_ = 0;
    }

    p_error_ = cte;
    i_error_ += cte;

    prev_time_ = cur_time;
}

double PID::TotalError()
{
    double total_error = -kp_ * p_error_ - kd_ * d_error_ - ki_ * i_error_;
    return total_error;
}

