#pragma once

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// Forward declaration of the task function
class Dn_Websocket;
void webSocketTaskFunction(void* parameter);

// Structure for message queue - using fixed-length char arrays to prevent memory issues
struct WSMessage {
    char to[32];          // Fixed-size buffer for recipient
    char content[256];    // Fixed-size buffer for message content
    bool valid;
};

class Dn_Websocket {
private:
    WebSocketsClient webSocket;
    char username[32];    // Fixed-size buffer for username
    bool isConnected = false;
    bool isRunning = false;
    
    // Task handle for WebSocket thread
    TaskHandle_t webSocketTaskHandle = NULL;
    
    // Queue for sending messages between threads
    QueueHandle_t messageQueue;
    
    void handleMessage(char* payload);
    void registerUsername();
    
    // Watchdog timer handling
    unsigned long lastHeartbeat = 0;
    void sendHeartbeat();

public:
    Dn_Websocket();
    ~Dn_Websocket();
    
    void begin(const char* serverAddress, uint16_t port, const char* url, const char* username);
    void stop();
    bool sendMessage(const char* to, const char* message);
    bool isWebSocketConnected();
    void loop(); // Optional loop function for main thread to monitor status
    
    // Made public so the static callback can access it
    static void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);
    
    // Friend function declaration to allow the task function to access private members
    friend void webSocketTaskFunction(void* parameter);
};