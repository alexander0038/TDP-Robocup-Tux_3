#include <Arduino.h>
#include "CoRoSoN_Main.h"
#include "CoRoSoN_IR-Ring.h"
#include "CoRoSoN_Compass.h"
#include "CoRoSoN_Pixy.h"
#include <elapsedMillis.h>
#include "DFRobot_URM09.h"

#define winkelToleranz 10
#define drehToleranz 5
#define AVS 65
#define MIS 30
#define MAS 90

float kP = 0.8;
float kI = 0.01;
float kD = 2.0;
float Iterm = 0;

byte fehlschlag = 0;
byte durchlauf = 0;
byte eckenStep = 0;
byte gesetzt = 0;

unsigned long startTimer;
unsigned long timer;
unsigned long steckenTimer;
unsigned long befreienTimer;

bool kompassGerichtet = false;
bool sucheBall = false;
bool machTor = false;
bool timerGesetzt = false;
bool varianteEins = false;
bool modusGesetzt = false;
bool varianteZwei = false;
bool zurueckStandBy = false;
bool steckenGeblieben = false;
bool lgroeßer = false;
bool rgroeßer = false;
bool startGesetzt = false;
bool IsInGame = false;
bool kickenErlaubt = false;
bool pixyVorher = false;
bool pixyJetzt = false;
bool torNichtGefunden = false;
bool befreit = false;

int speed = 0;
int steckenKompass = 0;
int eigenesTor = 0;
int ecke1 = 0;
int ecke2 = 0;
int GegnerTor = 0;
int lastL = 0;
int lastR = 0;
int Comp;
int Dir;
int rUS;
int lUS;
int lastDiff = 0;
int richtung = 0;

enum State {
  Setzen,
  Drehen,
  Angreifen,
  Notfall,
  Befreien
};

State step = Setzen;
IRRing irRing(I2C_ADD_IR);
Compass compass(I2C_ADD_Compass);
Pixy pixy(I2C_ADD_PIXY, 1, 2);

elapsedMillis buttonDeadTime = 0;
elapsedMillis angriffStecken = 0;
elapsedMillis kickenZeit = 0;
elapsedMillis checkSuche = 0;

DFRobot_URM09 URM09_R;
DFRobot_URM09 URM09_L;
DFRobot_URM09 URM09_V;


void setup() {

  if (!URM09_R.begin(0x12)) {
    Serial.println("I2C device number error R");
    delay(1000);
  }
  if (!URM09_L.begin(0x11)) {
    Serial.println("I2C device number error L");
    delay(1000);
  }
  if (!URM09_V.begin(0x13)) {
    Serial.println("I2C device number error L");
    delay(1000);
  }

  URM09_R.setModeRange(MEASURE_MODE_AUTOMATIC, MEASURE_RANG_500);
  delay(100);

  URM09_L.setModeRange(MEASURE_MODE_AUTOMATIC, MEASURE_RANG_500);
  delay(100);

  pinMode(17, OUTPUT);

  delay(500);
  SetBoardLEDColor(LEFT, BLUE);
  SetBoardLEDColor(RIGHT, BLUE);



  CoRoSoN_Init();
  pixy.Init();

  SetI2CLEDColor(I2C_MODULE_1, LEFT, RED);
  SetI2CLEDColor(I2C_MODULE_1, RIGHT, RED);

  delay(200);
  SetBoardLEDColor(LEFT, GREEN);
  SetBoardLEDColor(RIGHT, GREEN);
  delay(200);
  SetBoardLEDColor(LEFT, RED);
  SetBoardLEDColor(RIGHT, RED);
  delay(200);
  SetBoardLEDColor(LEFT, YELLOW);
  SetBoardLEDColor(RIGHT, YELLOW);
  delay(200);
  SetBoardLEDColor(LEFT, BLUE);
  SetBoardLEDColor(RIGHT, BLUE);
  delay(200);
  SetBoardLEDColor(LEFT, CYAN);
  SetBoardLEDColor(RIGHT, CYAN);
  delay(200);
  SetBoardLEDColor(LEFT, MAGENTA);
  SetBoardLEDColor(RIGHT, MAGENTA);
  delay(200);
  SetBoardLEDColor(LEFT, WHITE);
  SetBoardLEDColor(RIGHT, WHITE);
  delay(200);
  SetBoardLEDColor(LEFT, OFF);
  SetBoardLEDColor(RIGHT, OFF);
  delay(500);
}

void kick() {
  digitalWrite(17, HIGH);
  delay(30);
  digitalWrite(17, LOW);
}


bool lightbarrierTriggered() {
  int val = analogRead(4);

  if ((val > 2300 || val < 2100) && abs(irRing.BallDirection()) < 5)
    SetI2CLEDColor(I2C_MODULE_1, LEFT, GREEN);
  else
    SetI2CLEDColor(I2C_MODULE_1, LEFT, RED);

  return ((val > 2300 || val < 2100) && abs(irRing.BallDirection()) < 5);
}

void drive(int speed, int rotation) {
  int MaxSpeed = ABS(speed) + ABS(rotation);
  if (MaxSpeed > 100) {
    speed = (speed * 100) / MaxSpeed;
    rotation = (rotation * 100) / MaxSpeed;
  }
  int spd1 = rotation - speed;
  int spd2 = rotation + speed;
  DIRECTION dir1 = (spd1 >= 0 ? FORWARD : BACKWARD);
  DIRECTION dir2 = (spd2 >= 0 ? FORWARD : BACKWARD);
  SetMotor(MOTOR_1, dir1, spd1);
  SetMotor(MOTOR_2, dir2, spd2);
}

void standby() {
  if (zurueckStandBy == true) {
    SetI2CLEDColor(I2C_MODULE_1, LEFT, GREEN);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, GREEN);
  }

  SetMotor(MOTOR_1, FORWARD, 0);
  SetMotor(MOTOR_2, FORWARD, 0);
  SetMotor(MOTOR_3, FORWARD, 0);
  SetMotor(MOTOR_4, FORWARD, 0);

  if (irRing.BallSeen()) {
    if (ABS(irRing.BallDirection()) < 3) {
      SetBoardLEDColor(LEFT, GREEN);
    } else if (ABS(irRing.BallDirection()) < 15 && ABS(irRing.BallDirection()) >= 3) {
      SetBoardLEDColor(LEFT, CYAN);
    } else {
      SetBoardLEDColor(LEFT, BLUE);
    }
  } else {
    SetBoardLEDColor(LEFT, RED);
  }

  if (compass.Angle() < 90) {
    SetBoardLEDColor(RIGHT, BLUE);
  } else if (compass.Angle() >= 90 && compass.Angle() < 180) {
    SetBoardLEDColor(RIGHT, CYAN);
  } else if (compass.Angle() >= 180 && compass.Angle() < 270) {
    SetBoardLEDColor(RIGHT, MAGENTA);
  } else {
    SetBoardLEDColor(RIGHT, YELLOW);
  }

  if (zurueckStandBy == true) {
    if (buttonDeadTime < 500) {
      SetI2CLEDColor(I2C_MODULE_1, LEFT, RED);
      SetI2CLEDColor(I2C_MODULE_1, RIGHT, RED);
    } else {
      zurueckStandBy = false;
      SetI2CLEDColor(I2C_MODULE_1, LEFT, GREEN);
      SetI2CLEDColor(I2C_MODULE_1, RIGHT, GREEN);
    }
  }

  if (zurueckStandBy == false) {
    if (ButtonIsPressed(I2C_MODULE_1, LEFT)) {
      if (buttonDeadTime > 500) {
        SetI2CLEDColor(I2C_MODULE_1, RIGHT, GREEN);
        compass.Update();
        GegnerTor = compass.Angle();
        eigenesTor = (GegnerTor - 180 + 360) % 360;
        ecke1 = (GegnerTor + 45) % 360;
        ecke2 = (GegnerTor - 45 + 360) % 360;
        buttonDeadTime = 0;
      }
    }

    if (ButtonIsPressed(I2C_MODULE_1, RIGHT)) {
      if (buttonDeadTime > 500) {
        IsInGame = true;
        buttonDeadTime = 0;
        SetBoardLEDColor(RIGHT, OFF);
        SetBoardLEDColor(LEFT, OFF);
        SetI2CLEDColor(I2C_MODULE_1, LEFT, OFF);
        SetI2CLEDColor(I2C_MODULE_1, RIGHT, OFF);
      }
    }
  }
}

int filterUS(int neu, int alt) {
  if (alt == 0) return neu;
  if (neu <= 0 || neu > 300) return alt;
  if (abs(neu - alt) > 25) return alt + (neu - alt) / 4;
  return neu;
}

int winkelFehler(int soll, int ist) {
  int diff = soll - ist;
  if (diff > 180) diff -= 360;
  if (diff < -180) diff += 360;
  return diff;
}

void checkSteckenGeblieben() {
  if (millis() - steckenTimer >= 1750) {
    if (abs(winkelFehler(steckenKompass, compass.Angle())) <= 2) {
      befreienTimer = millis();
      steckenGeblieben = true;
    } else {
      steckenKompass = compass.Angle();
      steckenTimer = millis();
    }
  }
}

void checkComp() {
  compass.Update();
  Comp = compass.Angle();
}

void checkDir() {
  irRing.Update();
  Dir = irRing.BallDirection();
}

int drehSpeedAusBall(int ballDir) {
  int absDir = abs(ballDir);

  absDir = constrain(absDir, 4, 11);

  return map(absDir, 4, 11, 35, 50);
}

int fahrSpeedzuBall(int Entfehrnung) {
  int absDis = abs(Entfehrnung);

  absDis = constrain(absDis, 15, 50);

  return map(absDis, 15, 50, 58, 90);
}

int drehSpeedAngriff(int compass, int drehZiel) {
  int fehler = winkelFehler(drehZiel, compass);
  int absFehler = abs(fehler);

  absFehler = constrain(absFehler, 0, 90);

  if (absFehler < drehToleranz / 2) return 0;

  return map(absFehler, 0, 90, 25, 70);
}

void ballVerfolgung() {
  checkDir();
  checkSteckenGeblieben();

  if (irRing.BallDirection() >= -1 && irRing.BallDirection() <= 1) {
    byte Distanz = irRing.BallDistance();
    int forwardSpeed = fahrSpeedzuBall(Distanz);
    SetMotor(MOTOR_1, FORWARD, forwardSpeed);
    SetMotor(MOTOR_2, FORWARD, forwardSpeed);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, GREEN);
  } else if (irRing.BallDirection() > 3 && irRing.BallDirection() <= 11) {
    speed = drehSpeedAusBall(Dir);
    SetMotor(MOTOR_1, BACKWARD, speed);
    SetMotor(MOTOR_2, FORWARD, speed);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, CYAN);
  } else if (irRing.BallDirection() < -3 && irRing.BallDirection() >= -11) {
    speed = drehSpeedAusBall(Dir);
    SetMotor(MOTOR_2, BACKWARD, speed);
    SetMotor(MOTOR_1, FORWARD, speed);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, CYAN);
  } else if (irRing.BallDirection() == -2 || irRing.BallDirection() == -3) {
    SetMotor(MOTOR_1, FORWARD, 28);
    SetMotor(MOTOR_2, FORWARD, 43);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, WHITE);
  } else if (irRing.BallDirection() == 2 || irRing.BallDirection() == 3) {
    SetMotor(MOTOR_1, FORWARD, 28);
    SetMotor(MOTOR_2, FORWARD, 43);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, WHITE);
  } else if (abs(irRing.BallDirection()) <= 12 && abs(irRing.BallDirection()) <= 16) {
    SetMotor(MOTOR_1, BACKWARD, 60);
    SetMotor(MOTOR_2, BACKWARD, 60);
  } else if (abs(irRing.BallDirection()) >= 17) {
    SetMotor(MOTOR_1, BACKWARD, 85);
    SetMotor(MOTOR_2, BACKWARD, 85);
  }
}

void angriffBefreien() {
  int fehler = winkelFehler(richtung, compass.Angle());

  if (angriffStecken >= 10000 && angriffStecken <= 11000) {
    float korr = computePID(fehler);
    korr = constrain(korr, -10, 10);

    byte baseSpeed = 90;

    int left = constrain(baseSpeed - korr, 0, 100);
    int right = constrain(baseSpeed + korr, 0, 100);

    SetMotor(MOTOR_1, BACKWARD, left);
    SetMotor(MOTOR_2, BACKWARD, right);
  }
}

void torFinden() {
  int linksSuchen = (GegnerTor + 20) % 360;
  int rechtsSuchen = (GegnerTor - 20 + 360) % 360;
  static byte check = 0;

  if (checkSuche >= 6000) {
    torNichtGefunden = true;
  }

  if (check == 0) {
    if (abs(winkelFehler(linksSuchen, compass.Angle())) >= 3) {
      SetMotor(MOTOR_2, BACKWARD, 50);
      SetMotor(MOTOR_1, FORWARD, 50);
    } else check = 1;
  } else if (check == 1) {
    if (abs(winkelFehler(rechtsSuchen, compass.Angle())) >= 3) {
      SetMotor(MOTOR_2, FORWARD, 50);
      SetMotor(MOTOR_1, BACKWARD, 50);
    } else check = 0;
  }
}

float computePID(int error) {
  Iterm += error;
  Iterm = constrain(Iterm, -200, 200);

  float Dterm = error - lastDiff;
  lastDiff = error;

  return kP * error + kI * Iterm + kD * Dterm;
}

void torSuche() {
  //Möglicher Fehler durch nicht gesetzte Zeitbegrenzung bei Befreiung
  static bool lgroesser = false;
  static bool rgroesser = false;
  static bool sollGesetzt = false;
  static unsigned long befreienTimer = 0;
  bool lb = lightbarrierTriggered();

  pixyJetzt = pixy.OwnGoalSeen();

  if (pixyJetzt && !pixyVorher && lb && !befreit) {
    step = Befreien;
    befreienTimer = millis();
    befreit = true;
  }

  //if(befreit) befreit = false;

  if (step == Setzen) {
    Iterm = 0;
    lastDiff = 0;
    lastL = 0;
    lastR = 0;
    step = Drehen;
    angriffStecken = 0;
    lgroesser = false;
    rgroesser = false;
    sollGesetzt = false;
    befreit = false;
    checkSuche = 0;
  }

  else if (step == Drehen) {
    checkComp();
    checkSteckenGeblieben();
    if (steckenGeblieben) return;

    int rawL = URM09_L.getDistance();
    int rawR = URM09_R.getDistance();
    lUS = filterUS(rawL, lastL);
    rUS = filterUS(rawR, lastR);
    lastL = lUS;
    lastR = rUS;

    int fehler = winkelFehler(GegnerTor, Comp);
    float pid = computePID(fehler);

    int maxUS = 150;
    lUS = constrain(lUS, 0, maxUS);
    rUS = constrain(rUS, 0, maxUS);

    int lReduktion = map(rUS, 0, 90, 0, 40);
    int rReduktion = map(lUS, 0, 90, 0, 40);

    int leftSpeed = AVS - pid - lReduktion;
    int rightSpeed = AVS + pid - rReduktion;

    int l = constrain(leftSpeed, MIS, MAS);
    int r = constrain(rightSpeed, MIS, MAS);

    SetMotor(MOTOR_1, FORWARD, l);
    SetMotor(MOTOR_2, FORWARD, r);

    if (abs(fehler) <= 7) {
      if (lUS > rUS) lgroesser = true;
      else rgroesser = true;
      angriffStecken = 0;
      step = Angreifen;
      Iterm = 0;
      lastDiff = 0;
      sollGesetzt = false;
    } else if (!lb) {
      sucheBall = true;
      machTor = false;
      rgroesser = false;
      lgroesser = false;
      sollGesetzt = false;
      angriffStecken = 0;
      step = Setzen;
      befreit = false;
      checkSuche = 0;
    }
  }

  else if (step == Angreifen) {
    if (!lb) {
      rgroesser = false;
      lgroesser = false;
      sucheBall = true;
      machTor = false;
      sollGesetzt = false;
      angriffStecken = 0;
      step = Setzen;
      kickenZeit = 0;
      befreit = false;
    }
    if (!torNichtGefunden) {
      if (angriffStecken > 11000) {
        angriffStecken = 0;
      } else if (angriffStecken >= 10000 && angriffStecken <= 11000) {
        angriffBefreien();
        steckenGeblieben = false;
      }

      if (pixy.GoalSeen()) {
        checkSuche = 0;
        if (!sollGesetzt) {
          compass.Update();
          richtung = compass.Angle();
          sollGesetzt = true;
        }

        int fehler = winkelFehler(richtung, compass.Angle());
        float korrCompass = computePID(fehler);
        korrCompass = constrain(korrCompass, -9, 9);

        int pixyDir = pixy.GoalDirection();
        float korrPixy = pixyDir * 6;
        korrPixy = constrain(korrPixy, -6, 6);

        float korrGesamt = korrCompass + korrPixy;
        korrGesamt = constrain(korrGesamt, -15, 15);

        byte baseSpeed = 85;

        int left = constrain(baseSpeed - korrGesamt, 70, 100);
        int right = constrain(baseSpeed + korrGesamt, 70, 100);

        SetMotor(MOTOR_1, FORWARD, left);
        SetMotor(MOTOR_2, FORWARD, right);

        if (kickenZeit >= 2000 && kickenErlaubt == true) {
          kick();
          kickenZeit = 0;
        }
      } else {
        sollGesetzt = false;
        kickenZeit = 0;
        torFinden();
      }
    } else {
      step = Notfall;
    }
  } else if (step == Notfall) {
    if (pixy.GoalSeen()) {
      step = Angreifen;
      kickenZeit = 0;
      checkSuche = 0;
      return;
    }

    checkSteckenGeblieben();
    if (steckenGeblieben) return;

    if (!lb) {
      rgroesser = false;
      lgroesser = false;
      sucheBall = true;
      machTor = false;
      step = Setzen;
      checkSuche = 0;
    }

    static int leftSpeed = 0;
    static int rightSpeed = 0;

    checkComp();
    int ist = Comp;

    int rawL = URM09_L.getDistance();
    int rawR = URM09_R.getDistance();
    lUS = filterUS(rawL, lastL);
    rUS = filterUS(rawR, lastR);
    lastL = lUS;
    lastR = rUS;

    if (lUS > rUS) lgroesser = true;
    else rgroesser = true;

    int fehler = winkelFehler(GegnerTor, ist);
    float pid = computePID(fehler);
    pid = constrain(pid, -8, 8);

    lUS = constrain(lUS, 0, 90);
    rUS = constrain(rUS, 0, 90);

    int lReduktion = map(lUS, 0, 90, 40, 0);
    int rReduktion = map(rUS, 0, 90, 40, 0);

    bool mittig = abs(lUS - rUS) <= 5;

    if (mittig && abs(fehler) <= winkelToleranz) {
      leftSpeed = 80;
      rightSpeed = 80;
    } else if (lgroesser) {
      leftSpeed = 70 + pid + lReduktion;
      rightSpeed = 70 - pid;
    } else if (rgroesser) {
      leftSpeed = 70 - pid;
      rightSpeed = 70 + pid + rReduktion;
    }

    int l = constrain(leftSpeed, 50, 95);
    int r = constrain(rightSpeed, 50, 95);

    SetMotor(MOTOR_1, FORWARD, l);
    SetMotor(MOTOR_2, FORWARD, r);

  } else if (step == Befreien) {
    if (millis() - befreienTimer <= 2000) {
      int fehler = winkelFehler(GegnerTor, compass.Angle());

      float korr = computePID(fehler);
      korr = constrain(korr, -10, 10);

      byte baseSpeed = 90;

      int left = constrain(baseSpeed - korr, 80, 100);
      int right = constrain(baseSpeed + korr, 80, 100);

      SetMotor(MOTOR_1, BACKWARD, left);
      SetMotor(MOTOR_2, BACKWARD, right);
    } else {
      step = Setzen;
      befreit = true;
    }
  }
  pixyVorher = pixy.OwnGoalSeen();
}

void game() {
  if (!pixy.OwnGoalSeen()) kickenErlaubt = true;
  else kickenErlaubt = false;

  int rawL = URM09_L.getDistance();
  int rawR = URM09_R.getDistance();

  lUS = filterUS(rawL, lastL);
  rUS = filterUS(rawR, lastR);

  lastL = lUS;
  lastR = rUS;

  checkComp();

  SetMotor(MOTOR_3, FORWARD, 100);

  if (!modusGesetzt && buttonDeadTime > 500) {
    SetI2CLEDColor(I2C_MODULE_1, LEFT, RED);
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, RED);
    if (ButtonIsPressed(I2C_MODULE_1, LEFT) && !ButtonIsPressed(I2C_MODULE_1, RIGHT)) {
      varianteEins = true;
      startTimer = millis();
      modusGesetzt = true;
      buttonDeadTime = 0;
    } else if (ButtonIsPressed(I2C_MODULE_1, RIGHT) && !ButtonIsPressed(I2C_MODULE_1, LEFT)) {
      varianteZwei = true;
      modusGesetzt = true;
      buttonDeadTime = 0;
    }
  }

  if (varianteZwei) {
    varianteZ();
  } else if (varianteEins) {
    varianteE();
  }

  if (lUS > rUS + 5) SetBoardLEDColor(RIGHT, RED);
  else if(lUS <= rUS + 5 && rUS <= lUS + 5) SetBoardLEDColor(RIGHT, BLUE);
  else SetBoardLEDColor(RIGHT, GREEN);
}

void varianteE() /*variante Eins*/ {
  static int startWinkel;

  if (!startGesetzt) {
    compass.Update();
    startWinkel = compass.Angle();
    startTimer = millis();
    startGesetzt = true;
  }

  if (millis() - startTimer < 6500) {
    compass.Update();

    int fehler = winkelFehler(GegnerTor, compass.Angle());
    float korr = computePID(fehler);
    korr = constrain(korr, -25, 25);

    int baseSpeed = 100;

    int left = constrain(baseSpeed - korr, 75, 100);
    int right = constrain(baseSpeed + korr, 75, 100);

    SetMotor(MOTOR_1, FORWARD, left);
    SetMotor(MOTOR_2, FORWARD, right);
  } else {
    varianteEins = false;
    varianteZwei = true;
  }
}

void varianteZ() /*variante Zwei*/ {
  if (irRing.BallSeen()) {
    if (!(lightbarrierTriggered())) {
      sucheBall = true;
      machTor = false;
    } else {
      sucheBall = false;
      machTor = true;
    }

    if (sucheBall) {
      gesetzt = 0;
      step = Setzen;
      steckenGeblieben = false;
      ballVerfolgung();
    } else if (machTor) {
      if (gesetzt == 0) {
        timer = millis();
        gesetzt = 1;
      }
      torSuche();
    }

    if (steckenGeblieben && step != Angreifen) {
      checkComp();
      if (millis() - befreienTimer < 1000) {
        SetMotor(MOTOR_1, FORWARD, 100);
        SetMotor(MOTOR_1, FORWARD, 100);
      } else {
        steckenGeblieben = false;
        steckenKompass = compass.Angle();
        steckenTimer = millis();
      }
    }
  } else {
    checkComp();
    SetI2CLEDColor(I2C_MODULE_1, RIGHT, RED);

    int drehFehler = winkelFehler(GegnerTor, Comp);
    int speedB = drehSpeedAngriff(Comp, GegnerTor);
    float pid = computePID(drehFehler);
    pid = constrain(pid, -6, 6);
    int speedL = 80 - pid;
    int speedR = 80 + pid;

    if (abs(drehFehler) > 10) {
      if (drehFehler > 0) {
        SetMotor(MOTOR_1, BACKWARD, speedB);
        SetMotor(MOTOR_2, FORWARD, speedB);
      } else {
        SetMotor(MOTOR_1, FORWARD, speedB);
        SetMotor(MOTOR_2, BACKWARD, speedB);
      }
    } else {
      SetMotor(MOTOR_1, BACKWARD, speedL);
      SetMotor(MOTOR_2, BACKWARD, speedR);
    }
  }
}


void loop() {
  if (IsInGame) {
    game();

    if ((ButtonIsPressed(I2C_MODULE_1, LEFT) || ButtonIsPressed(I2C_MODULE_1, RIGHT)) && modusGesetzt == true) {
      if (buttonDeadTime > 500) {
        IsInGame = false;
        modusGesetzt = false;
        varianteEins = false;
        varianteZwei = false;
        zurueckStandBy = true;
        startGesetzt = false;
        step = Setzen;
        buttonDeadTime = 0;
        eckenStep = 0;
        befreit = false;
      }
    }
  } else {
    standby();
  }

  /*
  DEBUG_PRINT(irRing.BallSeen());
  DEBUG_PRINT(irRing.BallDirection());
  DEBUG_PRINT(irRing.BallDistance());
  DEBUG_PRINT(pixy.GoalSeen());
  DEBUG_PRINT(pixy.GoalDirection());
  DEBUG_PRINT(pixy.OwnGoalSeen());
  DEBUG_PRINT(pixy.OwnGoalDirection());
  */

  compass.Update();
  irRing.Update();
  pixy.Update();
  delay(5);
}
