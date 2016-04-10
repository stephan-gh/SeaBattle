#pragma once

namespace SeaBattle {
namespace Network {
class Client;
}

class Player
{
public:
    Player();
    Network::Client* client;

};

}
