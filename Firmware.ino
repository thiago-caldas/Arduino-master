#include <Wire.h>
#include <string.h>
#include <stdint.h>

float SENSORS_convertReceivedValue( uint16_t value );

void setup()
{
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);             
}
 
void loop()
{
  byte address;
  uint8_t data[2], commandNumber = 0;
  int index = 0; 
  float receivedValue; 
  uint16_t aux, command;
  char dataName[40];
  
  address = 0x34;
  
  if (Serial.available()) {
    commandNumber = Serial.read();

    switch(commandNumber)
    {
      case '0':   // Tensao do painel
        command = 0x00; 
        strcpy(dataName, "Tensao do painel"); 
        break;
      case '1':   // Tensao da bateria
        command = 0x01; 
        strcpy(dataName, "Tensao da bateria"); 
        break;
      case '2':   // Tensao da carga
        command = 0x02; 
        strcpy(dataName, "Tensao da carga"); 
        break;
      case '3':   // Corrente do painel
        command = 0x03; 
        strcpy(dataName, "Corrente do painel"); 
        break;
      case '4':   // Corrente da bateria
        command = 0x04; 
        strcpy(dataName, "Corrente da bateria"); 
        break;
      case '5':   // Corrente da carga
        command = 0x05; 
        strcpy(dataName, "Corrente da carga"); 
        break;         
    }
    Wire.beginTransmission(address);
    Wire.write(command);    
    Wire.endTransmission();
        
    Wire.requestFrom(address, 2);    
    while(Wire.available()) 
    {                      
        char c = Wire.read();    
        data[index] = c;
        index++;             
    }
    aux = data[0];
    aux = aux << 8;
    aux += (data[1] & 0xFF);
    
    receivedValue = SENSORS_convertReceivedValue(aux);
    
    Serial.print(dataName);
    Serial.print(": ");
    Serial.print(receivedValue);
    Serial.print('\n');
  }      
}

float SENSORS_convertReceivedValue( uint16_t value )
{
    float convertedValue;
    uint8_t signal;
    uint16_t aux;

    signal = (uint8_t) ((value & 0x8000) >> 15);     // Pega  bit mais significativo como sinal
    
    if (signal)                                      // Se for negativo, calcula a partir do complemento de dois
    {
        aux = value;
        aux -= 1;                                    // Subtrai 1
        aux ^= 0xFFFF;                               // Inverte os bits
        convertedValue = aux;
        convertedValue *= -1;
    }
    else convertedValue = value;
    
    convertedValue /= 100;                           // Divide por 100 para obter o valor decimal                    
    
    return convertedValue;
}
