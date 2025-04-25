# Qt_Chat (Windows)
Qt Network + QML practice work

## How to use this app:
1. Build and run Server (keep it working all time, otherwise UB), it will create it's own database in /build folder to store user data (server is running on localhost:8080).
2. Build Client; run the executable to open new client app.

UB if:
- Login twice by one user while his session is still active;
- Send '|' symbol in message of any type
- Send a message while server is OFF;
