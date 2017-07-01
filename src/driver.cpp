#include <chrono>

#include "driver.h"

Driver::Driver()
{
    OnReset();
}

//When simulator is reset need to reinitialize the variables
void Driver::OnReset()
{
    total_error_ = 0;
    num_iterations_ = 0;
    last_cte_values.clear();
}

//When telemetry is recived need to process them
void Driver::OnDataRecieved(double &cte, double &speed, double &steering_angle)
{
    //In case were in idle state do nothing
    if(state_ == State::Idle)
    {
        speed = 0;
        steering_angle = 0;
        last_cte_values.clear();

        return;
    }

    double abs_cte = fabs(cte);
    total_error_ += abs_cte;
    num_iterations_++;

    //In case cte is too large or max error is too large or number or we've done enough iterations
    //stop the simulation
    //Otherwise drive the car
    if(abs_cte > kMaxCte || total_error_ > max_error_ || num_iterations_ > max_num_iterations_)
    {
        state_ = State::Idle;

        std::cout<<"Stop reason: ";
        if(abs_cte > kMaxCte)
        {
            std::cout<<"CTE is too large: "<<cte<<" Iteration: "<<num_iterations_<<std::endl;
            total_error_ = std::numeric_limits<double>::max();
        }
        else if(total_error_ > max_error_)
        {
            std::cout<<"Total error is too large: "<<total_error_<<" Max allowed: "<<max_error_
                     <<" Iteration:"<<num_iterations_<<std::endl;
        }
        else if(num_iterations_ > max_num_iterations_)
        {
            std::cout<<"Max number of iterations exceeded: "<<num_iterations_<<std::endl;
        }


        //We're done unlick the mutex so CalcError could return the value
        condition_variable_.notify_one();

        speed = 0;
        steering_angle = 0;
        last_cte_values.clear();
    }
    else
    {
        //Drive the car

        //Update PID
        pid_.UpdateError(cte);
        //Get steering angle in rad/deg2rad(25) units
        steering_angle = std::max(std::min(pid_.TotalError(), 1.), -1.);

        //Add current cte to the list of last ctes and get number of crossings
        int cross_num = UpdateLastCteAndGetSwingsNum(cte);

        //In case speed is higher than some value and cte is higher than some value
        //or cte is swinging slow down or use a regular speed otherwise
        speed = speed > kDangerousSpeed &&
                (abs_cte > kDangerousCte || cross_num > kMaxNumSwings) ? kBreakSpeed : kSpeed;
    }
}

//Runs a simulation with given params and returns the total cte of the run
double Driver::CalcError(double p, double d, double i, double max_error, int max_num_iterations)
{
    state_ = State::Idle;

    total_error_ = 0;
    num_iterations_ = 0;
    max_num_iterations_ = max_num_iterations;
    max_error_ = max_error;

    pid_.Init(p, d, i);

    state_ = State::CollectingData;

    //Request simulator reset
    RequestClientReset();

    //Wait until simulation is over,
    //OnDataRecieved will be called in client-server thread and will decide when to unlock the mutex
    std::unique_lock<std::mutex> lock(mutex_);
    while(state_ == State::CollectingData)
    {
        condition_variable_.wait_for(lock, std::chrono::seconds(1));
    }

    return total_error_;
}

//This method updates the list of last ctes,
//finds the number of times the car crossed the target trajectory with some tolerance
//This is done to detect swings.
int Driver::UpdateLastCteAndGetSwingsNum(double cte)
{
    last_cte_values.push_back(cte);
    if(last_cte_values.size() > kLastCteNum)
    {
        last_cte_values.pop_front();
    }

    int change_count = 0;
    double prev_cte = 0;
    for(auto i = last_cte_values.begin(); i != last_cte_values.end(); ++i)
    {
        double tol = kSwingTolerance;
        if((*i < -tol && prev_cte > tol) || (prev_cte < -tol && *i > tol))
        {
            change_count++;
        }

        prev_cte = *i;
    }

    return change_count;
}


