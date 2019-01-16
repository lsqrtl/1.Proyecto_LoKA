#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  9                    //Pin 9 para el reset del RC522
#define SS_PIN  10                    //Pin 10 para el SS (SDA) del RC522

byte bufferTarjeta[8];

MFRC522 mfrc522(SS_PIN, RST_PIN);     //Creamos el objeto para el RC522

void setup() 
{
  Serial.begin(9600);                 //Iniciamos la comunicación  serial
  SPI.begin();                        //Iniciamos el Bus SPI
  mfrc522.PCD_Init();                 //Iniciamos  el MFRC522
}

void loop() 
{
  // Revisamos si hay nuevas tarjetas  presentes
  if (mfrc522.PICC_IsNewCardPresent()) 
        {  
            //Seleccionamos una tarjeta
            if (mfrc522.PICC_ReadCardSerial()) 
            {
                  for (int i = 0; i < mfrc522.uid.size; i++) 
                  {       
                          //Se lee la tarjeta y se envia el buffer en la poscion i en formato HEX
                          bufferTarjeta[i] = mfrc522.uid.uidByte[i];
                          Serial.print(bufferTarjeta[i],HEX);    
                  } 

                  Serial.println();    
                        
                  //Terminamos la lectura de la tarjeta  actual
                  mfrc522.PICC_HaltA();         
            } 
            
        }          
}
