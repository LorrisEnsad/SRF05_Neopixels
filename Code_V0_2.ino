// Code Version 0.2 du 10/07 a 00:24

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

const int BUFFER_SIZE = 10; //Nombre de points de mesures pris en compte pour calculer les moyenne mobile

// Pins des capteurs de distance
const int trigPin1 = 9;           
const int trigPin2 = 12;
const int trigPin3 = 11;
const int echoPin1 = 10;  
const int echoPin2 = 12;  
const int echoPin3 = 12;  

// Pins des ruban LEDs
const int LEDpin1 = 12;
const int LEDpin2 = 12;
const int LEDpin3 = 12;

const int numledsQ1 = 80; //Nombre de leds dans le quartier1


// Variable d'etats des quartiers
float distancebuff1[BUFFER_SIZE];
float moy_dist1;
float const seuil_dist1 = 100.0;
bool leds_state1 = false; //false = leds off, true = leds on

float distancebuff2[BUFFER_SIZE];
float moy_dist2;
float const seuil_dist2 = 100.0;
bool leds_state2 = false;

float distancebuff3[BUFFER_SIZE];
float moy_dist3;
float const seuil_dist3 = 100.0;
bool leds_state3 = false;

const int LEDcolorR = 200;
const int LEDcolorG = 120;
const int LEDcolorB = 0;

Adafruit_NeoPixel Q1Leds(numledsQ1, LEDpin1, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin (9600);        

  //Initialisation des modes des pins des capteurs
  pinMode(trigPin1, OUTPUT);   
  pinMode(echoPin1, INPUT); 
  pinMode(trigPin2, OUTPUT);   
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);   
  pinMode(echoPin3, INPUT);   

  //Initialisation des LEDs
  Q1Leds.begin(); //Neopixel Q1
  pinMode(LEDpin2, OUTPUT); //Ruban LED Q2
  pinMode(LEDpin3, OUTPUT); //__________Q3

  for (int i = 0; i<BUFFER_SIZE; i++)
  {
    distancebuff1[i] = 0.0;
    distancebuff1[i] = 0.0;
    distancebuff1[i] = 0.0;
  }
  
}

void loop() {

  //Mises a jour des buffer de mesures de capteurs de distance
  for (int i = 0; i<(BUFFER_SIZE-1); i++)
  {
    distancebuff1[i] = distancebuff1[i+1];
    distancebuff1[i] = distancebuff1[i+1];
    distancebuff1[i] = distancebuff1[i+1];
  }
  distancebuff1[BUFFER_SIZE] = getDistance(trigPin1,echoPin1); 
  distancebuff2[BUFFER_SIZE] = getDistance(trigPin2,echoPin2);
  distancebuff3[BUFFER_SIZE] = getDistance(trigPin3,echoPin3);

  //Calcul des moyennes mobiles de la mesures des capteurs de distances
  moy_dist1 = 0.0;
  moy_dist2 = 0.0;
  moy_dist3 = 0.0;

  for (int i = 0; i<BUFFER_SIZE; i++)
  {
    moy_dist1 += distancebuff1[i];
    moy_dist2 += distancebuff2[i];
    moy_dist3 += distancebuff3[i];
  }

  moy_dist1 = moy_dist1/float(BUFFER_SIZE);
  moy_dist2 = moy_dist2/float(BUFFER_SIZE);
  moy_dist3 = moy_dist3/float(BUFFER_SIZE);


  //On verifie si un changement d'etat des leds est necessaire, et on les operes si besoin
  if(moy_dist1 < seuil_dist1 && !leds_state1) //Quartier 1
  {
    leds_state1 = true;
    light_leds(1,true);
  }
  else if(moy_dist1 > seuil_dist1 && leds_state1)
  {
    leds_state1 = false;
    light_leds(1,false);
  }

  if(moy_dist2 < seuil_dist2 && !leds_state2) //Quartier 2
  {
    leds_state2 = true;
    light_leds(2,true);
  }
  else if(moy_dist2 > seuil_dist2 && leds_state2)
  {
    leds_state2 = false;
    light_leds(2,false);
  }

  if(moy_dist3 < seuil_dist3 && !leds_state3) //Quartier 3
  {
    leds_state3 = true;
    light_leds(3,true);
  }
  else if(moy_dist3 > seuil_dist3 && leds_state3)
  {
    leds_state3 = false;
    light_leds(3,false);
  }

  //Print de debuggage
  Serial.println("\n Moy dist 1 :"); 
  Serial.print(moy_dist1); 
  Serial.println("\n Moy dist 2 :"); 
  Serial.print(moy_dist2);
  Serial.println("\nMoy dist 3 :"); 
  Serial.print(moy_dist3);  
                 

  delay(5);      //delay 50ms between each reading
}

//------------------FONCTIONS-------------------------------

//Retourne la distance mesure par le capteur brancher aux pins donnes en arguments
float getDistance(int trig_pin, int echo_pin)
{
  float echoTime;                   
  float calculatedDistance;        

  digitalWrite(trig_pin, LOW);  // On efface l'etat logique de TRIG //
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  echoTime = pulseIn(echo_pin, HIGH);     

  calculatedDistance = echoTime * 0.034 / 2.0;  

  return calculatedDistance;
}

// Allume ou eteint les leds du quartier donne en argument
void light_leds(int quartier, bool ON) // si ON est donne false, eteint les leds, sinon les allumes
{
  Serial.println("Allumage du quartier "); 
  int colorR;
  int colorG;
  int colorB;
  int brightness;

  if (ON)
  {
    Serial.println("\n Allumage du quartier "); 
    Serial.print(quartier); 
    colorR = LEDcolorR;
    colorG = LEDcolorG;
    colorB = LEDcolorB;
    brightness = 100;
  }
  else{
    Serial.println("\n Eteignage du quartier "); 
    Serial.print(quartier); 
    colorR = 0;
    colorG = 0;
    colorB = 0;
    brightness = 2;
  }

  if (quartier == 1)
  {
    Q1Leds.clear();
    for(int i=0; i<numledsQ1; i++) 
    {
      Q1Leds.setPixelColor(i, Q1Leds.Color(colorR, colorG, colorB));
    }
    Q1Leds.show();
  }
  else if (quartier ==2)
  {
    analogWrite(LEDpin2, brightness);
  }
  else if (quartier == 3)
  {
    analogWrite(LEDpin3, brightness);
  }
}




