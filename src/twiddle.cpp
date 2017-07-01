#include <iostream>

#include "twiddle.h"

using namespace std;

void Twiddle::Run(Driver &server, vector<double> &p,
                  vector<double> &dp, double tolerance, int max_num_iterations)
{
    cout<<endl<<"Setting baseline: "<<endl;
    double min_error = server.CalcError(p[0], p[1], p[2],
                                        numeric_limits<double>::max(), max_num_iterations);
    cout<<"Baseline error: "<<min_error<<endl;

    while(dp[0] + dp[1] + dp[2] > tolerance)
    {
        for(int i = 0; i < 3; i++)
        {
            p[i] += dp[i];

            cout<<endl<<"Plus at "<<i<<": "<<p[i] - dp[i]<< " + "<<dp[i]<<endl;
            ReportValues(p, dp);
            double error = server.CalcError(p[0], p[1], p[2], min_error, max_num_iterations);
            if(error >= min_error)
            {
                std::cout<<"No improvement. Error: "<<error<<" Min error: "<<min_error<<std::endl;

                p[i] -= 2 * dp[i];

                std::cout<<endl<<"Minus at "<<i<<": "<<p[i] + dp[i]<< " - "<<dp[i]<<std::endl;
                ReportValues(p, dp);
                error = server.CalcError(p[0], p[1], p[2], min_error, max_num_iterations);
            }

            if(error < min_error)
            {
                cout<<"Improvement!!!!!!!!!!!!!!!!!!! Error: "<<error<< " Prev error: "<<min_error<<endl;
                min_error = error;
                dp[i] *= 1.1;
            }
            else
            {
                std::cout<<"No improvement. Error: "<<error<<" Min error: "<<min_error<<std::endl;

                p[i] += dp[i];
                dp[i] *= 0.9;
            }
        }
    }

    cout<<endl<<"Final result:"<<endl;
    ReportValues(p, dp);
}

void Twiddle::ReportValues(vector<double> &p, vector<double> &dp)
{
    cout<<"Values: p: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<" dp: "<<dp[0]<<" "<<dp[1]<<" "<<dp[2]<<endl;
}