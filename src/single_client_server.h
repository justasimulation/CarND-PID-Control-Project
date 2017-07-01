#ifndef PID_SINGLE_CLIENT_SERVER_H
#define PID_SINGLE_CLIENT_SERVER_H

#include <uWS/uWS.h>

//General class for talking with one simulator
//Establishes connection with client and call OnReset/OnDataReceived methods
//so they could be derived in a higher level class
class SingleClientServer
{
    //
    // Disconnected --OnConnection--> Connected
    //

    private:
        // SingleClientServer will be listening on that port
        static const int kPort = 4567;

        // Minimum number of seconds for the first telemetry message to be delivered by client.
        // Sometimes connection with client is established but telemetry is not sent for some reasons.
        // To detect this:
        // 1. OnMessage method is required to set is_client_responded_ flat when the first telemetry arrives
        // 2. CheckHangingAsync method is started in a separate thread and waits KHangingCheckSec seconds
        // 3. In case is_client_reset_in_progress_ flag is not set after waiting, reset message is sent to client
        static const int kHangingCheckSec = 1;

        enum ClientState
        {
            Connected           , // Client goes to this state when OnConnection is called
            SendingTelemetry    , //
            Disconnected        , //
        } client_state_;

        bool is_reset_requested_;


        // Lower lever server class
        uWS::Hub hub_;

    public:
        SingleClientServer();
        ~SingleClientServer() {};

        void Start();

    protected:
        void RequestClientReset();
        void virtual OnReset();
        void virtual OnDataRecieved(double &cte, double &speed, double &steering_angle);

    private:
        void OnMessage(uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode);
        void OnHttpRequest(uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t);
        void OnConnection(uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req);
        void OnDisconnection(uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length);

        void CheckHangingAsync(uWS::WebSocket<uWS::SERVER> *ws);
        void SendMessage(uWS::WebSocket<uWS::SERVER> &ws, std::string command, std::string params);
        std::string GetData(std::string s);
        void SetClientState(ClientState state);
};

#endif //PID_SINGLE_CLIENT_SERVER_H
