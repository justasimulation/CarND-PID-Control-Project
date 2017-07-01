#ifndef PID_H
#define PID_H

#include <chrono>

//PID controller implementation
class PID
{
    public:
        /*
        * Errors
        */
        double p_error_;
        double i_error_;
        double d_error_;

        /*
        * Coefficients
        */
        double kp_;
        double ki_;
        double kd_;

        long num_updates_;

        std::chrono::high_resolution_clock::time_point prev_time_;
      /*
      * Constructor
      */
      PID();

      /*
      * Destructor.
      */
      virtual ~PID();

      /*
      * Initialize PID.
      */
      void Init(double Kp, double Ki, double Kd);

      /*
      * Update the PID error variables given cross track error.
      */
      void UpdateError(double cte);

      /*
      * Calculate the total PID error.
      */
      double TotalError();
};

#endif /* PID_H */
