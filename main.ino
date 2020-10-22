#define BLYNK_PRINT Serial
#include<BlynkSimpleEsp8266.h>
#include <SPI.h>
#include "DHT.h"
#define DHTTYPE DHT11



#define DHT11_PIN 2
DHT dht(DHT11_PIN, DHTTYPE);

char auth[] = "_kBZI7Mv7jIWAjcGRf4x21I2BNCAIO41";
char ssid[] = "KK";
char pass[] = "abcdefgh";

//ORIENT
#include <Wire.h>


//PULSE
#include <Ticker.h>
#include <ESP8266WiFi.h>
#define pulsePin A0   //Analog input pin Number on ESP8266
#define blinkPin 2    //Led On Chip


//ORIENT
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D6;
const uint8_t sda = D7;

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;


//PULSE
//LOOP VARIABLES
////////////////
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


//////////////////////
//INTERRRUPTS VARIABLE
//////////////////////
Ticker flipper;
volatile int rate[10];                        // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;     // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;      // used to find IBI
volatile unsigned long current;
volatile int P = 512;                         // used to find peak in pulse wave, seeded
volatile int T = 512;                         // used to find trough in pulse wave, seeded
volatile int thresh = 560;                    // used to find instant moment of heart beat, seeded 530du
volatile int amp = 0;                         // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat = true;            // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = false;          // used to seed rate array so we startup with reasonable BPM
volatile unsigned long lastMillis = 0;        // used to determine pulse timing
volatile float tempSignal=0;

volatile int msTime = 0;


BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  
  //ORIENT
  Wire.begin(sda, scl);
  MPU6050_Init();


  //PULSE
  pinMode(blinkPin,OUTPUT);                 // pin that will blink to your heartbeat!                    // we agree to talk fast!
  delay(2); 

  interruptSetup();                          // sets up to read Pulse Sensor signal every 2mS
  lastMillis=millis();
  
}

void loop() {
  Blynk.run();
  timer.run();

  //ORIENT
  double Ax, Ay, Az, T, Gx, Gy, Gz;
  
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;

  Serial.print("Ax: "); Serial.print(Ax);
  Serial.print(" Ay: "); Serial.print(Ay);
  Serial.print(" Az: "); Serial.print(Az);
  Serial.print(" T: "); Serial.print(T);
  Serial.print(" Gx: "); Serial.print(Gx);
  Serial.print(" Gy: "); Serial.print(Gy);
  Serial.print(" Gz: "); Serial.println(Gz);
   Serial.print("\n");

   Blynk.virtualWrite(V1,T);
   Blynk.virtualWrite(V2,Gx);
   Blynk.virtualWrite(V5,Gy);
   Blynk.virtualWrite(V6,Gz);

  


  //PULSE
  if (QS == true) {    // A Heartbeat Was Found
        // BPM and IBI have been Determined
        // Quantified Self "QS" true when arduino finds a heartbeat
        serialOutputWhenBeatHappens();         // A Beat Happened, Output that to serial.
        Blynk.virtualWrite(V0,BPM);
        QS = false;                            // reset the Quantified Self flag for next time
    }

   Serial.print("Temperature = ");
  Serial.println(dht.readTemperature());
  Serial.print("Humidity = ");
  Serial.println(dht.readHumidity());
  Blynk.virtualWrite(V3,dht.readHumidity());
  Blynk.virtualWrite(V4,dht.readTemperature());

  if(dht.readHumidity() > 60)
  {
    Blynk.notify("Alert ! it is too Humid !!");
    Serial.println("Alert ! it is too Humid !!");
  }

  if(BPM < 50 || BPM >200)
  {
    Blynk.notify("Heartbeat is Off !! Send Help !!");
    Serial.println("Heartbeat is Off !! Send Help !!");
  }

  delay(1000);
}


//ORIENT
void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}



//PULSE

//  Decides How To OutPut BPM and IBI Data
void serialOutputWhenBeatHappens() {
    sendDataToSerial('B',BPM);                 // send heart rate with a 'B' prefix
    sendDataToSerial('Q',IBI);                 // send time between beats with a 'Q' prefix
}

void sendDataToSerial(char symbol, int data ) {
    Serial.print(symbol);
    Serial.println(data);
}

void interruptSetup() {
// Initializes Ticker to have flipper run the ISR to sample every 2mS as per original Sketch.
    flipper.attach_ms(2, ISRTr);
}

void ISRTr() {
    noInterrupts();                            // disable interrupts while we do this

    Signal = analogRead(pulsePin);             // read the Pulse Sensor
    current=millis();
    int difference = current-lastMillis;
    lastMillis=current;
    sampleCounter += difference;               // keep track of the time in mS with this variable
    msTime+=difference;
    int N = sampleCounter - lastBeatTime;      // monitor the time since the last beat to avoid noise

    //  find the peak and trough of the pulse wave
    if(Signal < thresh && N > (IBI/5)*3) {     // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T) {                      // T is the trough
            T = Signal;                        // keep track of lowest point in pulse wave
        }
    }

    if(Signal > thresh && Signal > P) {        // thresh condition helps avoid noise
        P = Signal;                            // P is the peak
    }                                          // keep track of highest point in pulse wave

    //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
    // signal surges up in value every time there is a pulse
    if (N > 250) {                                   // avoid high frequency noise
        if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ) {
            Pulse = true;                            // set the Pulse flag when we think there is a pulse
            digitalWrite(blinkPin,LOW);              // turn on pin 13 LED
            IBI = sampleCounter - lastBeatTime;      // measure time between beats in mS
            lastBeatTime = sampleCounter;            // keep track of time for next pulse

            if(secondBeat) {                         // if this is the second beat, if secondBeat == TRUE
                secondBeat = false;                  // clear secondBeat flag
                for(int i=0; i<=9; i++) {            // seed the running total to get a realisitic BPM at startup
                    rate[i] = IBI;
                }
            }

            if(firstBeat) {                          // if it's the first time we found a beat, if firstBeat == TRUE
                firstBeat = false;                   // clear firstBeat flag
                secondBeat = true;                   // set the second beat flag
                interrupts();                        // enable interrupts again
                return;                              // IBI value is unreliable so discard it
            }


            // keep a running total of the last 10 IBI values
            word runningTotal = 0;                   // clear the runningTotal variable

            for(int i=0; i<=8; i++) {                // shift data in the rate array
                rate[i] = rate[i+1];                 // and drop the oldest IBI value
                runningTotal += rate[i];             // add up the 9 oldest IBI values
            }

            rate[9] = IBI;                           // add the latest IBI to the rate array
            runningTotal += rate[9];                 // add the latest IBI to runningTotal
            runningTotal /= 10;                      // average the last 10 IBI values
            BPM = 60000/runningTotal;                // how many beats can fit into a minute? that's BPM!
            QS = true;                               // set Quantified Self flag
            // QS FLAG IS NOT CLEARED INSIDE THIS ISR
        }
    }

    if (Signal < thresh && Pulse == true) {  // when the values are going down, the beat is over

        digitalWrite(blinkPin,HIGH);           // turn off pin 13 LED
        Pulse = false;                         // reset the Pulse flag so we can do it again
        amp = P - T;                           // get amplitude of the pulse wave
        thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
        P = thresh;                            // reset these for next time
        T = thresh;
    }

    if (N > 2500) {                            // if 2.5 seconds go by without a beat
        thresh = 530;                          // set thresh default
        P = 512;                               // set P default
        T = 512;                               // set T default
        lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date
        firstBeat = true;                      // set these to avoid noise
        secondBeat = false;                    // when we get the heartbeat back
        BPM=0;
    }
    interrupts();                              // enable interrupts when youre done!
}
