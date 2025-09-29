
#include <iostream>
#include <string>
#include <thread>

#include <jsonrpccxx/server.hpp>
#include <nlohmann/json.hpp>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "rcws/base.hpp"

using namespace jsonrpccxx;
using namespace RCWS;
//using json = nlohmann::json;

extern "C"
int main_controller(void);


class RpcConnector {
public:
	explicit RpcConnector(jsonrpccxx::JsonRpcServer& server) : server(server) {}
    std::string HandleRequest(const std::string& request)
	{
		return server.HandleRequest(request);
	}
private:
	jsonrpccxx::JsonRpcServer& server;
};



class ZmqRpcServer {
public:
    ZmqRpcServer(RpcConnector& handler) : handler(handler) {
        // Initialize ZeroMQ context and socket
        ctx = zmq::context_t();
        rpc_sock = zmq::socket_t(ctx, ZMQ_REP);
        rpc_sock.bind("tcp://*:5555");

        // Set up PUB status
    }

    ~ZmqRpcServer() {
        rpc_sock.close();
        ctx.shutdown();
    }

    int start() {
        int ret = -1;

        service_run = true;
        service_thread = new std::thread(&ZmqRpcServer::Loop, this);
        if (service_thread->joinable())
            ret = 0;
        
        return ret;
    }

    void stop() {
        service_run = false;
    }

private:
    zmq::context_t ctx;
    zmq::socket_t rpc_sock;
    RpcConnector handler;
    bool service_run;
    std::thread *service_thread;

    void Loop()
    {
        try {
            zmq::pollitem_t items[] = {
                { rpc_sock, 0, ZMQ_POLLIN, 0 },
                //{ subscriber, 0, ZMQ_POLLIN, 0 }
            };
            const std::chrono::milliseconds timeout{ 100 };

            while (service_run) {
                zmq::message_t message;
				auto res = zmq::poll(items, (sizeof(items) / sizeof(items[0])), timeout);

				if (res == 0) {
					/* Timeout */
                    /* Publish status */
					//publish_config(mgr._priv->status);
				}
				else if (res == -1) {
					break;
				}
				else {
					if (items[0].revents & ZMQ_POLLIN) {
                        // Handle RPC request
                        auto received_size= rpc_sock.recv(message);
						auto resp = handler.HandleRequest(message.to_string());
                        rpc_sock.send(zmq::message_t(resp), zmq::send_flags::none);
					}
				}
            }
        }
        catch (std::exception e) {
            std::cout << e.what() << std::endl;
        }
    }
};



static JsonRpc2Server RpcServer;
static RpcConnector RpcHandler(RpcServer);

int main()
{
    RemoteWeaponSystemServer app;

    RpcServer.Add("SetAzimuth", GetHandle(&RemoteWeaponSystemServer::SetAzimuth, app), { "setpoint" });
    RpcServer.Add("SetElevation", GetHandle(&RemoteWeaponSystemServer::SetElevation, app), { "setpoint" });
    RpcServer.Add("HaltCurrentAction", GetHandle(&RemoteWeaponSystemServer::HaltCurrentAction, app), { "force" });
    RpcServer.Add("SetManualControlRate", GetHandle(&RemoteWeaponSystemServer::SetManualControlRate, app), { "speed" });
    RpcServer.Add("DoManualControlAzimuth", GetHandle(&RemoteWeaponSystemServer::DoManualControlAzimuth, app), { "dir" });
    RpcServer.Add("DoManualControlElevation", GetHandle(&RemoteWeaponSystemServer::DoManualControlElevation, app), { "dir" });

    RpcServer.Add("GetBarrelStatus", GetHandle(&RemoteWeaponSystemServer::GetBarrelStatus, app), { "num"});
    RpcServer.Add("GetAllBarrelStatus", GetHandle(&RemoteWeaponSystemServer::GetAllBarrelStatus, app), { });
    RpcServer.Add("GetRipplePreset", GetHandle(&RemoteWeaponSystemServer::GetRipplePreset, app), { });
    RpcServer.Add("SetSelectedBarrels", GetHandle(&RemoteWeaponSystemServer::SetSelectedBarrels, app), { "barrels" });
    RpcServer.Add("ConfirmSelection", GetHandle(&RemoteWeaponSystemServer::ConfirmSelection, app), { "confirm" });
    RpcServer.Add("SetFiringMode", GetHandle(&RemoteWeaponSystemServer::SetFiringMode, app), { "mode" });
    RpcServer.Add("GetFiringMode", GetHandle(&RemoteWeaponSystemServer::GetFiringMode, app), { });
    RpcServer.Add("GetLauncherStatus", GetHandle(&RemoteWeaponSystemServer::GetLauncherStatus, app), { });
    RpcServer.Add("IsLauncherArmed", GetHandle(&RemoteWeaponSystemServer::IsLauncherArmed, app), { });

    RpcServer.Add("HeadingCorrection", GetHandle(&RemoteWeaponSystemServer::Correction, app), { "north" });


    ZmqRpcServer serv(RpcHandler);

    serv.start();
    
    main_controller();

    serv.stop();

    return 0;
}