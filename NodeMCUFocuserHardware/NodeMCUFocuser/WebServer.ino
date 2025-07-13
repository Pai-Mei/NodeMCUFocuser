//------Web server implementation----------------------------------

void SetupServer() {
  server.on("/", handle);
  server.onNotFound(handleNotFound);
  server.begin();
}

void CheckServer() {
  server.handleClient();
}

void handle() {
  PerformAction();
  if (server.method() == HTTP_GET)
    server.send(200, "text/html", GetCientPage());
  else if (server.method() == HTTP_POST)
    server.send(200, "text/json", GetPostResponse());
  else
    server.send(405, "text/json", "{ \"error\": \"method not allowed\" }");
}

void handleNotFound() {
  if (server.method() == HTTP_GET)
    server.send(404, "text/html", Get404Page());
  else
    server.send(404, "text/plain", "Error: page not found");
}

String GetPostResponse() {
  //returns http response content
  String result = "{ \"position\":" + String(currentPosition) + ",\r\n";
  result += "\"targetPosition\":" + String(targetPosition) + ",\r\n";
  result += "\"moving\":" + String(isMoving) + "}\r\n";
  return result;
}

void PerformAction() {
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == MoveCommand) {
      int steps = String(server.arg(i)).toInt();
      if (steps != 0) Move(steps);
    } else if (server.argName(i) == MoveToCommand) {
      int position = String(server.arg(i)).toInt();
      MoveTo(position);
    } else if (server.argName(i) == SetMaxPositionCommand) {
      int position = String(server.arg(i)).toInt();
      SetMaxPosition(position);
    } else if (server.argName(i) == StopCommand) {
      int stop = String(server.arg(i)).toInt();
      if (stop > 0) Stop();
    } else if (server.argName(i) == StopBrakeCommand) {
      int stopBrake = String(server.arg(i)).toInt();
      if (stopBrake > 0) StopBrake();
    } else if (server.argName(i) == "opencover") {
      int level = String(server.arg(i)).toInt();
      if(level == 0)
        CloseCover();
      else
        OpenCover();
    } else if (server.argName(i) == "setlight") {
      int level = String(server.arg(i)).toInt();
      SetLight(level);
    }
  }
}