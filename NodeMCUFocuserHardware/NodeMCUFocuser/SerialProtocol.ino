String WithTermination(String message) {
  return message + Termination;
}

String ProcessCommand(String command) {
  if (command.startsWith(SetMaxPositionCommand)) {
    int steps = command.substring(strlen(SetMaxPositionCommand) + 1).toInt();
    SetMaxPosition(steps);
    return WithTermination(String("yes"));
  } else if (command.startsWith(MoveCommand)) {
    int steps = command.substring(strlen(MoveCommand) + 1).toInt();
    Move(steps);
    return WithTermination(String("moving"));
  } else if (command.startsWith(MoveToCommand)) {
    int steps = command.substring(strlen(MoveCommand) + 1).toInt();
    Move(steps);
    return WithTermination(String("moving"));
  } else if (command.startsWith(StopCommand)) {
    Stop();
    return WithTermination(String("stoped"));
  } else if (command.startsWith(statusCommand)) {
    String status = isMoving ? Status_Moving : Status_Stoped;
    return WithTermination(String(status));
  } else if (command.startsWith(StopBrakeCommand)) {
    StopBrake();
    return WithTermination(String("brakeStoped"));
  } else if (command.startsWith(OpenCoverCmd)) {
    OpenCover();
    return WithTermination("opened");
  } else if (command.startsWith(CloseCoverCmd)) {
    CloseCover();
    return WithTermination("closed");
  } else if (command.startsWith(SetLightCmd)) {
    int level = command.substring(strlen(SetLightCmd) + 1).toInt();
    SetLight(level);
    return WithTermination("set");
  } else if (command.startsWith(GetSettings)) {
    return WithTermination(CurrentSettings());
  } else if (command.startsWith(SaveSettings)) {
    if (StoreSettings(command.substring(strlen(SaveSettings) + 1))) {
      WriteToEEPROM();
      SetupWiFi();
      SetupServer();
      return WithTermination(String("saved"));
    } else
      return WithTermination(String("failed"));
  } else
    return WithTermination(String("unknown command"));
}

String ProcessMoonliteCommand(String command) {
  int offset = command[1] == '2' ? 1 : 0;
  String cmd = command.substring(1 + offset, 2 + offset);
  if (cmd = "GV")
    return WithTermination(String("10"));
  else if (cmd == "GP")
    return WithTermination(String("0000"));
  else if (cmd == "GN")
    return WithTermination(String("0000"));
  else if (cmd == "GT" || cmd == "GD" || cmd == "GH" || cmd == "GI" || cmd == "GB")
    return WithTermination(String("0000"));

  return WithTermination(String(""));
}

void CheckSerial() {
  if (Serial.available() != 0) {
    while (Serial.available() == 0) {}  //wait for data available
    String teststr = Serial.readString();
    teststr.trim();
    String answer;
    if (teststr[0] == ':')
      answer = ProcessMoonliteCommand(teststr);
    else
      answer = ProcessCommand(teststr);
    Serial.println(answer);
    delay(100);
  }
}
