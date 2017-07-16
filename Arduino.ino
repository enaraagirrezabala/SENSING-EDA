/*pulsera detector de ansiedad, estres


   El objetivo es activar un led de aviso cuando el nivel del GSR (galvanic skin response sensor) sea mayor
   que el nivel base de estres. Para ello se cuenta con el sensor GSR como input y 3leds en un mismo pin
   en paralelo.
   Además se contará con un sensor de presión, el cual al tocarlo contabilizara momentos "happy" o positivos
   al mantenerlo presionado por más tiempo el sensor activara los leds que empezaran a marcar el ritmo de
   la respiración durante dos minutos.
   en caso de que se presione de nuevo durante esos dos minutos se apagarán los led


   Fuente del codigo inicial:
   Done with the aid of Chris Kairalla's documentation
   here: http://www.chris3000.com/archive/galvanic-skin-response/
   Basically acts as a variable resistor to measure skin conductance.
   fuente del codigo inicial de la respiracion:
   http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/


   Enara
   Junio 2017



*/
#include <math.h>

int led = 6;           // PMW pin
int brightness = 0;    // definir brightnees de led
int ValorSensor = 0;
int fadeAmount = 5;
int nuevoValorSensor;
float mns = 60; // Cambiar este numero dependiendo en el nivel base de tu estres
int valor;

long time = 0;
int periodo = 3000;
unsigned long previousMillis1;        // guardara la última vez que se guardo el millis
unsigned long prev = 0;        // guardara la última vez que se guardo el millis

int intervalo2 = 1200;           // intervalo del blink 1 en milisegundos quiero que haga fade in - out durante 2 minutos
int intervalo1 = 100;           // intervalo del sensor de presion (medir cada 0,1 s)
int tiempoPresion; // contador para sensor de presión para saber cuanto tiempo se mantiene pulsado esl sensor
int blinkinicio = 0;
//Queremos guardar media valores del gsr
const unsigned int numReadings = 60;
unsigned int gsrValores[numReadings];
unsigned int i = 0;
int promedioGSR;
int suma = 0;
int nuevoPromedioGSR;
int ValorSensorPresion;
unsigned long pressstart;
long fade_inout = -30 * 1000;

boolean gsr;
boolean presion;
boolean presionvalor;


void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(led, OUTPUT);//Leds color
  pinMode(A0, INPUT); //GSR sensor
  pinMode(A1, INPUT); //sensor de presion


  int _min = 1024;
  int _max = 0;


  // blucle
  // tomar muestra en una array
  for (unsigned int i = 0; i < numReadings; i++)
  {
    int last = analogRead(A0);
    suma += last;
    Serial.println(analogRead(A0));

    if (last < _min) _min = last;
    if (last > _max) _max = last;

    delay(100);
  }

  ValorSensorPresion = analogRead(A1);
  promedioGSR = suma / numReadings;
  Serial.print("promedio GSR = ");
  Serial.println(promedioGSR);
  Serial.println( ValorSensorPresion);

}


void loop() {

  // leer sensor GSR----------------------------------------------------------------
  ValorSensor = analogRead(A0);
  Serial.print("valor Sensor = "); Serial.println(ValorSensor);

  //sensor gsr y led

  if (gsr == false &&  ValorSensor > (promedioGSR + 50)) {
    gsr = true;
    digitalWrite(led, HIGH);

  }

  if (gsr == true &&  ValorSensor < (promedioGSR + 50)) {
    gsr = false;
    digitalWrite(led, LOW);

  }



  // leer sensor presion----------------------------------------------------------------

  int ValorSensor2 = analogRead(A1);
  //Serial.print("valor Sensor 2 = "); Serial.println(ValorSensor2);

  if (presion == false && ValorSensor2 > 0) {
    pressstart = millis();
    presion = true;
  }
  if (presion == true && ValorSensor2 < 5) {
    unsigned long tiempoPresion =  millis() - pressstart;
    Serial.println(tiempoPresion);
    presion = false;

    if (tiempoPresion >= 1500) {
      fade_inout = millis();


    }
    else if (tiempoPresion > 50 ) {
      Serial.println("momento happy");

    }
  }


  if (millis() - fade_inout < 30 * 1000) {
    Serial.println("fadin");

    if (millis() - prev > 30) {

      prev = millis();
      float val = (exp(sin(millis() / 4000.0 * PI)) - 0.36787944) * 108.0;
      analogWrite(led, val);
    }
    else {
      delay(1);
    }
  }
}
