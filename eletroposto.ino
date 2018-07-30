#include <LiquidCrystal.h>
#include "EmonLib.h"
#include <SPI.h>
EnergyMonitor emon1;

LiquidCrystal lcd(2,3,4,6,7,8,9,10,11,12,13);

//Tensao da rede eletrica
int rede = 127.0;

//Pino do sensor SCT
int pino_sct = A1;

unsigned long tempoLigado = 0;
double kwh = 0;

double tarifa = 1.14;
double taxaLucro = 50;
double totalPagar = 0;

void setup() {
  
  Serial.begin(115200);   

  //Pino, calibracao - Cur Const= Ratio/BurdenR. 2000/35 = 57
  emon1.current(pino_sct, 57);
  //Ajuste iniciais display
  lcd.begin(20,4);
  
}

void loop() {

  unsigned long timer = millis();
  double irms = emon1.calcIrms(1480);
  
  lcd.setCursor(0,0);
  lcd.print("TENSAO:");

  lcd.setCursor(8,0);
  lcd.print(rede);

  lcd.setCursor(11,0);
  lcd.print("V");
  
  lcd.setCursor(0,1);
  lcd.print("CORRENTE:");

  lcd.setCursor(10,1);
  lcd.print(irms);
  
  if (irms < 5) {
    if (kwh > 0) {
      delay(10000);
    }
    tempoLigado = 0;
  }

  lcd.setCursor(0,2);
  lcd.print("KW/H:");

  kwh = (irms*rede*tempoLigado) / 1000;
  lcd.setCursor(6,2);
  lcd.print(kwh);
  
  lcd.setCursor(0,3); 
  lcd.print("TOTAL R$");

  totalPagar = (kwh * tarifa) + (((kwh * tarifa) * taxaLucro) / 100);
  lcd.setCursor(9,3); 
  lcd.print(totalPagar);

  tempoLigado += 1;
  delay(1000);
  
}
