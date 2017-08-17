#include <wiringSerial.h>

int spinMotor(double angle){
    int fd = serialOpen ("/dev/ttyUSB0", 9400);
    if(fd > 0){
        error("ERROR connecting to serial");
    }

    // write stuff to the serial port
    int command = 123;
    serialPutchar(fd, command);
    return command;
}
