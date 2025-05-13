#include <Websocket.h>

static Dn_Websocket *clientInstance = nullptr;

// Task function that runs WebSocket loop in a separate thread
void webSocketTaskFunction(void *parameter)
{
    Dn_Websocket *client = (Dn_Websocket *)parameter;

    while (client->isRunning)
    {
        client->webSocket.loop();

        WSMessage msg;
        if (xQueueReceive(client->messageQueue, &msg, 0) == pdTRUE)
        {
            if (msg.valid && client->isConnected)
            {
                {
                    JsonDocument doc;
                    doc["type"] = "message";
                    doc["from"] = client->username;
                    doc["to"] = msg.to;
                    doc["message"] = msg.content;

                    char jsonBuffer[384];
                    size_t len = serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

                    client->webSocket.sendTXT(jsonBuffer, len);

                    vTaskDelay(5 / portTICK_PERIOD_MS);
                }

                ESP.getFreeHeap();
            }
        }

        vTaskDelay(5 / portTICK_PERIOD_MS);
    }

    client->webSocketTaskHandle = NULL;
    vTaskDelete(NULL);
}

void Dn_Websocket::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    if (clientInstance == nullptr)
    {
        return;
    }

    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("[WSc] Disconnected!");
        clientInstance->isConnected = false;
        break;
    case WStype_CONNECTED:
        Serial.println("[WSc] Connected!");
        clientInstance->isConnected = true;
        clientInstance->lastHeartbeat = millis();

        clientInstance->registerUsername();
        break;
    case WStype_TEXT:
        if (length > 0)
        {
            char *payloadCopy = (char *)malloc(length + 1);
            if (payloadCopy)
            {
                memcpy(payloadCopy, payload, length);
                payloadCopy[length] = '\0';

                clientInstance->handleMessage(payloadCopy);

                free(payloadCopy);
            }
        }
        break;
    case WStype_ERROR:
        Serial.println("[WSc] Connection error!");
        break;
    }
}

Dn_Websocket::Dn_Websocket()
{
    clientInstance = this;

    username[0] = '\0';

    // Create message queue
    messageQueue = xQueueCreate(5, sizeof(WSMessage));
    if (messageQueue == NULL)
    {
        Serial.println("[WSc] Error creating message queue");
    }
}

Dn_Websocket::~Dn_Websocket()
{
    stop();

    if (messageQueue != NULL)
    {
        vQueueDelete(messageQueue);
        messageQueue = NULL;
    }
}

void Dn_Websocket::begin(const char *serverAddress, uint16_t port, const char *url, const char *usernameStr)
{
    strncpy(username, usernameStr, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0'; // Ensure null termination

    webSocket.begin(serverAddress, port, url);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000); // Attempt to reconnect every 5 seconds

    webSocket.enableHeartbeat(20000, 3000, 2);

    Serial.println("[WSc] WebSocket client initialized");

    // Start WebSocket task on core 0 (Main loop runs on core 1)
    isRunning = true;
    xTaskCreatePinnedToCore(
        webSocketTaskFunction, // Task function
        "WebSocketTask",       // Task name
        10240,                 // Stack size (bytes) - increased
        this,                  // Parameter to pass
        1,                     // Task priority
        &webSocketTaskHandle,  // Task handle
        0                      // Run on core 0
    );

    Serial.println("[WSc] WebSocket task started on core 0");
}

void Dn_Websocket::stop()
{
    isRunning = false;

    int timeout = 10;
    while (webSocketTaskHandle != NULL && timeout-- > 0)
    {
        delay(100);
    }

    if (webSocketTaskHandle != NULL)
    {
        vTaskDelete(webSocketTaskHandle);
        webSocketTaskHandle = NULL;
    }

    webSocket.disconnect();

    Serial.println("WebSocket client stopped");
}

bool Dn_Websocket::sendMessage(const char *to, const char *message)
{
    if (!isRunning)
    {
        Serial.println("Cannot send message: client not running");
        return false;
    }

    if (!isConnected)
    {
        Serial.println("Cannot send message: not connected");
        return false;
    }

    WSMessage msg;
    msg.valid = true;

    strncpy(msg.to, to, sizeof(msg.to) - 1);
    msg.to[sizeof(msg.to) - 1] = '\0';

    strncpy(msg.content, message, sizeof(msg.content) - 1);
    msg.content[sizeof(msg.content) - 1] = '\0';

    if (xQueueSend(messageQueue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Serial.println("Message queue full, message dropped");
        return false;
    }

    return true;
}

bool Dn_Websocket::isWebSocketConnected()
{
    return isConnected;
}

void Dn_Websocket::loop()
{
    if (!isConnected)
    {
        Serial.println("WebSocket disconnected, checking task...");

        if (webSocketTaskHandle == NULL && isRunning)
        {
            Serial.println("WebSocket task died, attempting restart...");
            stop();
            delay(1000);
        }
    }
}

void Dn_Websocket::handleMessage(char *payload)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    if (doc.containsKey("type") && doc["type"] == "registration")
    {
        if (doc["status"] == "success")
        {
            Serial.println("Username registration successful!");
        }
        else
        {
            Serial.println("Username registration failed");
        }
    }

    if (doc.containsKey("type") && doc["type"] == "message")
    {
        const char *from = doc["from"];
        const char *message = doc["message"];

        lastMessage = message;
    }

    ESP.getFreeHeap();
}

void Dn_Websocket::registerUsername()
{
    if (username[0] != '\0')
    {
        JsonDocument doc;
        doc["type"] = "register";
        doc["username"] = username;

        char jsonBuffer[128];
        size_t len = serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

        webSocket.sendTXT(jsonBuffer, len);
        Serial.println("Username registration sent");
    }
}
