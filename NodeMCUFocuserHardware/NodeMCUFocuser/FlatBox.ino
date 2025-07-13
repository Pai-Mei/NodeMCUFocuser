//-----flat panel control--------------------------------------

void SetupFlatBox()
{
  servo.attach(pinServo, 500, 2500); 
  servo.write(176);
}

void OpenCover() {
  servo.write(5);
}

void CloseCover() {
  servo.write(176);
}

void SetLight(int level) {
  analogWrite(pinLight, level);
}