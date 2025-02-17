#pragma once

class Dn_Websocket
{
private:
    char *ssid;
    char *password;
public:
    void init(char *ssid, char *password);
    void send(char *message);
    void receive();
    void close();
};