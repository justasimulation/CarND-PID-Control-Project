#include "single_client_server.h"

#include <thread>

#include "json.hpp"


using namespace std;
using namespace std::placeholders;

using json = nlohmann::json;

SingleClientServer::SingleClientServer()
{
    hub_.onMessage(bind(&SingleClientServer::OnMessage, this, _1, _2, _3, _4));
    hub_.onHttpRequest(bind(&SingleClientServer::OnHttpRequest, this, _1, _2, _3, _4, _5));
    hub_.onConnection(bind(&SingleClientServer::OnConnection, this, _1, _2));
    hub_.onDisconnection(bind(&SingleClientServer::OnDisconnection, this, _1, _2, _3, _4));
}

void SingleClientServer::Start()
{
    SetClientState(ClientState::Disconnected);

    if (hub_.listen(kPort))
    {
        cout << "Listening to port " << kPort << endl;
    }
    else
    {
        cerr << "Failed to listen to port" << endl;
        exit(-1);
    }

    std::thread t([this]()
                  {
                      hub_.run();
                  });
    t.detach();
}

void SingleClientServer::OnMessage(uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode)
{
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event

    if(is_reset_requested_)
    {
        cout<<"Resetting"<<endl;
        SendMessage(ws, "reset", "");
        return;
    }
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
        auto string_data = GetData(std::string(data));
        if (string_data != "")
        {
            auto json_data = json::parse(string_data);
            std::string event = json_data[0].get<std::string>();
            if (event == "telemetry")
            {
                SetClientState(ClientState::SendingTelemetry);

                double cte = std::stod(json_data[1]["cte"].get<std::string>());
                double speed = std::stod(json_data[1]["speed"].get<std::string>());
                double angle = std::stod(json_data[1]["steering_angle"].get<std::string>());

                OnDataRecieved(cte, speed, angle);

                json json_message;
                json_message["steering_angle"] = angle;
                json_message["throttle"] = speed;
                SendMessage(ws, "steer", json_message.dump());
            }
        }
        else
        {
            SendMessage(ws, "manual", "");
        }
    }
}

void SingleClientServer::OnHttpRequest(uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t)
{
    assert(false); // this should be never called

    if (req.getUrl().valueLength == 1)
    {
        const string s = "<h1>Hello world!</h1>";
        res->end(s.data(), s.length());
    }
    else
    {
        // i guess this should be done more gracefully?
        res->end(nullptr, 0);
    }
}

void SingleClientServer::OnConnection(uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req)
{
    cout << "Connected!!!" << endl;

    SetClientState(ClientState::Connected);
    is_reset_requested_ = false;

    thread t(&SingleClientServer::CheckHangingAsync, this, &ws);
    t.detach();
}

void SingleClientServer::OnDisconnection(uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length)
{
    ws.close();
    cout << "Disconnected" << endl;

    SetClientState(ClientState::Disconnected);
}

void SingleClientServer::CheckHangingAsync(uWS::WebSocket<uWS::SERVER> *ws)
{
    sleep(kHangingCheckSec);
    if(client_state_ != ClientState::SendingTelemetry)
    {
        cout<< "Client is hanging. Reseting connection"<<endl;

        SendMessage(*ws, "reset", "");

        RequestClientReset();
    }
}

void SingleClientServer::SendMessage(uWS::WebSocket<uWS::SERVER> &ws, std::string command, std::string params)
{
    string message = "42[\"" + command + "\",";
    message += params.empty() ? "{}" : params;
    message +="]";
    ws.send(message.data(), message.length(), uWS::OpCode::TEXT);
}

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string SingleClientServer::GetData(string s)
{
    auto found_null = s.find("null");
    auto b1 = s.find_first_of("[");
    auto b2 = s.find_last_of("]");
    if (found_null != std::string::npos)
    {
        return "";
    }
    else if (b1 != std::string::npos && b2 != std::string::npos)
    {
        return s.substr(b1, b2 - b1 + 1);
    }
    return "";
}

void SingleClientServer::RequestClientReset()
{
    is_reset_requested_ = true;

    OnReset();
}

void SingleClientServer::OnReset() {}
void SingleClientServer::OnDataRecieved(double &cte, double &speed, double &steering_angle) {}

void SingleClientServer::SetClientState(ClientState state)
{
    client_state_ = state;
}








