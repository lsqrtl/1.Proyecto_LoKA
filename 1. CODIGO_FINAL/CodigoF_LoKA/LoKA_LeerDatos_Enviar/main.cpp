#include <include/tc-rtos/rtos.h>
#include <include/tc-rtos/timer.h>
#include <include/tc-rtos/Loka.h>
#include <include/tc-rtos/console.h>

#include <stdlib.h>
#include <string.h>

#include <include/tc-rtos/drivers/SerialPort.h>
#include <include/tc-rtos/SigfoxProtocol.h>

//Defino el tamaño del buffer
#define bufferSIZE 30


//Variables
char buffer[bufferSIZE];
unsigned long ul = 0;
int count = 0;
char *p;


//SerialPort instancia
SerialPort serial;


//Funcion para imprimir el buffer
void printBuffer()
{
    if(count>0)
    {
        Board::digitalWrite(LED, HIGH);
        serial.write("Tag: ");
        for(int i = 0; i< count; i++)
        {
            serial.write(buffer[i]);
        }
        Board::digitalWrite(LED, LOW);
        sleep(5);
     }
}


//Funcion para convertir y enviar el buffer a la red Sigfox
void convertAndSend()
{
    ul = strtol(buffer, &p, 16);

    Board::digitalWrite(LED, HIGH);
    int result = Protocol::sendDataLogger(1,ul,0);
    Board::digitalWrite(LED, LOW);
}


//Setup -> inicializar baudrate y protocolo RCZ4 Sigfox
void setup()
{
    //Deshabilitar watchdog
    Loka::disableWatchdog();

    //Inicializar puerto serial con baudrate
    serial.init(0, 9600);

    //Inicializar Sigfox
    //RCZ1 - Europe, RCZ2 - Usa/Canada, RCZ4 - Brazil, NZ, AU, ETC
    SigfoxProtocol::init(RCZ4);

}


//Loop, pregunto si hay dato en el puerto serial si hay dato se convierte y se envia
void loop()
{
    if(serial.available())
    {
        while(serial.available())
        {
            buffer[count] = serial.read();

            if(buffer[count] == '\n')
            {
                count++;
                buffer[count] = '\0';
                printBuffer();
                convertAndSend();
                count = 0;
                break;
            }

            if(count == bufferSIZE)
            {
                buffer[count] = '\0';
                printBuffer();
                convertAndSend();
                count = 0;
                break;
            }
            count++;
        }
    }
}


int main(void)
{
    //Desactivo el debug.
    tc_rtos_init(0);

    setup();

    for(;;)
        loop();

    //return 0;
}
