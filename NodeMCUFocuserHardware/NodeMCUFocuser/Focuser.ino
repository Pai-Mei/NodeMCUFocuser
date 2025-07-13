//------Stepper----------------------------------

void SetupStepper() {
  myStepper.setMaxSpeed(500.0);
  myStepper.setAcceleration(50.0);
  myStepper.setSpeed(20);
  targetPosition = currentPosition = myStepper.currentPosition();
}

void DoStep() {
  if (isMoving) {
    isMoving = myStepper.run();
    currentPosition = myStepper.currentPosition();
    if (!isMoving && targetPosition == currentPosition) Serial.println(WithTermination(Status_Stoped));
  }
  if (targetPosition != currentPosition) {
    myStepper.moveTo(targetPosition);
    isMoving = true;
  }
}

void SetMaxPosition(int steps) {
  targetPosition = currentPosition = steps / 2;
  myStepper.setCurrentPosition(currentPosition);
  isMoving = false;
}

void Move(int steps) {
  targetPosition += steps;
  isMoving = true;
}

void MoveTo(int position) {
  targetPosition = position;
  isMoving = true;
}

void Stop() {
  myStepper.setSpeed(0);
  targetPosition = currentPosition;
  isMoving = false;
}

void StopBrake() {
  digitalWrite(pinIN1, LOW);
  digitalWrite(pinIN2, LOW);
  digitalWrite(pinIN3, LOW);
  digitalWrite(pinIN4, LOW);
}
