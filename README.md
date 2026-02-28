# TCP Client-Server Project

This is a client-server project based on the MVC pattern. It is a very good example if you want to learn how to work with the following technologies and concepts:

- **Socket Programming**
- **ProtoBuf (Protocol Buffer)**
- **Signals and Slots**
- **Threading**
- **Qt**
- **Regex**
- **C++**

**Note:** This is a very early version of the application. The code is not completely optimized yet.

## Dependencies

To build and run this project, you need a few things set up:
- A working version of Qt.
- Protobuf downloaded and installed on your system.
- You also need to configure and add specific dependencies to your `CMakeLists.txt` file.

---

The project is divided into two parts: the client side and the server side.

## Server Side

When you run the server project, you can use these commands to manage your servers:

- `create {IP} {Port}` : Starts a server. You can use `127.0.0.1` as the IP to create a test server on your local system.
- `off {Server ID}` : Turns off the server using its ID. The Server ID is just an integer (make sure not to enter anything else by mistake).
- `on {Server ID} -i {IP} -p {Port}` : Turns the server back on with a new IP and port. 
- `status` : Shows the status of all your servers.
- `exit` : Exits the app.

## Client Side

When you run the client project, a client is created. You can use these commands:

- `connect {IP} {Port}` : Connects to a server. You can use `127.0.0.1` to test it locally. The IP format should be `X.X.X.X` and the port is an integer.
- `send to {receiver_id} "{Message}"` : Sends a message to a specific client. When a client connects, the server gives it a 1-based integer ID. You must know this ID to send a packet. Note: The message must be inside double quotes.
- `retry` : Tries to connect again to the last IP and port you used.
- `disconnect` : Disconnects from the current server.
- `status` : Shows your current connection status.
- `exit` : Exits the app.

## Technical Notes

I learned a lot while creating this project, especially about network programming in Qt:

- **Using Separate Threads for I/O:** In socket programming, you should run your input/output commands on a separate thread from the main thread. If you don't, waiting for user inputs will block your background tasks.
- **Handling Output with a Logger:** When more than one thread wants to print or change the output at the same time, it can cause problems. It is better to use a logger to handle this safely. I created a custom Logger class that uses Qt's Signals and Slots. This Qt feature made thread management much easier.
- **How TCP Sockets Connect:** I finally understood exactly how TCP sockets connect to a server. In Qt, `QTcpSocket` and `QTcpServer` connect using a handshake method. First, the socket sends a packet to the host. If the host is found, the socket's state changes from "unconnected" to "connecting" and it waits for the server. If the server is started, it responds to the request and they connect together.
