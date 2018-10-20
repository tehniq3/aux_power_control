// sistem alimmentare accesorii in masina de nu are
// program scris de Nicu FLORICA pentru Tom

#define buton 3
#define releu 4
#define pinmasura A0

int starebuton = 0; //flag to see if the button has been pressed, used internal on the subroutine only
int duratamare = 3000; // valoare timp apaaare lunga, in ms
int duratapasare = duratamare; // initializare timp apasare
int aparare = 0;// sets default push value for the button going low
int eliberare = 0;// sets the default value for when the button goes back high

//int knobval; // value for the rotation of the knob
boolean buttonflag = false; // default value for the button flag

float R2 = 15;  // rezistenta intre borna plus si intrarea analogica
float R1 = 4.7;  // rezistenta intre intrare analogica si masa (GND)
//  +12V---| 15k |---- Analog In ---| 4k7 |------| GND
int treapta = 0; // valoare prag tensiune
float tensiune = 0;  // tensiune masurata
float tensiuneminima = 10.5;  // tensiune minima baterie
float referinta = 5.;  // tensiune referinta
byte contactpus = 0;  // variabila pentru contact pus
byte manual = 0;  // variabila pentru oprire manuala

void setup() {
  Serial.begin(9600);
  pinMode(buton,INPUT);//
    digitalWrite(buton,HIGH); //
    pinMode(releu,OUTPUT); //
    digitalWrite (releu, HIGH); //
}

void loop() {
   duratapasare = duratamare;
    duratapasare = aflareduratapasare();
    delay (10);
    
    if (duratapasare < duratamare) // daca s-a apasat scurt
    {  
      digitalWrite(releu, HIGH);  // cupleaza tensiunea pentru accesorii
    }
    
       
    if (duratapasare > duratamare) // daca s-a apasat lung
    {  
     digitalWrite(releu, LOW);  // deconecteaza tensiunea pentru accesorii 
     manual = 1;  // foartare oprire manual
     duratapasare = duratamare;
     };

treapta = analogRead(pinmasura);  
tensiune = (float)((R1+R2) * treapta * referinta / R1 / 1024.0);
Serial.print("Tensiune pe borna auxiliara = ");
Serial.print(tensiune);
Serial.println("V");

if (tensiune > tensiuneminima)  // daca s-a pus contactul
{
 contactpus = 1;
 }

if ((manual == 0) and (contactpus == 1))
{
digitalWrite(releu, HIGH);  // cupleaza tensiunea pentru accesorii
}

if ((contactpus == 1) and (tensiune < tensiuneminima))
{
  contactpus = 0;  // resetam variabila
  manual = 0;  // resetam variabila
  delay(5000);  // asteapta 5 secunde
  digitalWrite(releu, LOW);  // deconecteaza tensiunea pentru accesorii 
}
       
} // sfarsit program


int aflareduratapasare () {
  starebuton = digitalRead(buton);  
       if(starebuton == LOW && buttonflag == false) {     
              aparare = millis();
              buttonflag = true;
          };
          
   if (starebuton == HIGH && buttonflag == true) {
         eliberare = millis ();
         duratapasare = eliberare - aparare;
         buttonflag = false;
       };
       return duratapasare;
}
