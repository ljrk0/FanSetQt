/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014   Eduard Nicodei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * */

#include "fancontroller.h"



FanController::FanController()
{

}

#ifdef _MSC_VER

#define LPCFILTER_IOPORT_READ_FUNC 0x800
#define LPCFILTER_IOPORT_WRITE_FUNC 0x801




bool FanController::read_uchar(const HANDLE hDevice, const unsigned char port,
                unsigned char* const value)
{
    const DWORD nInBufferSize = 8;
    const DWORD nOutBufferSize = 1;
    const DWORD dwIoControlCode = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800,
            METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS);
    assert(dwIoControlCode == 0x22E000);

    BYTE lpOutBuffer[nOutBufferSize];
    BYTE lpInBuffer[nInBufferSize] = {};
    DWORD bytesReturned;
    bool status;

    if (!hDevice) {
#ifndef QT_NO_DEBUG
        printf("Invalid device handle: %u\n", hDevice);
#endif
        return false;
    }

    lpInBuffer[0] = port;
    status = DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize,
                             lpOutBuffer, nOutBufferSize, &bytesReturned, NULL);

    if (!status || bytesReturned != 1) {
#ifndef QT_NO_DEBUG
        printf("%s: Unable to read from port 0x%02x\n", __FUNCTIONW__, port);
        printf("Error code: 0x%x; bytesReturned: %u\n", GetLastError(), bytesReturned);
#endif
        return false;
    }

    *value = lpOutBuffer[0];
    return true;
}

bool FanController::write_uchar(const HANDLE hDevice, const unsigned char port, const unsigned char value)
{
    const DWORD dwIoControlCode = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801,
            METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS);
    const DWORD nInBufferSize = 8;

    BYTE lpInBuffer[nInBufferSize] = {port, 0x00, 0x00, 0x00,
            value, 0x00, 0x00, 0x00};
    DWORD bytesReturned;
    BOOL status;

    if (hDevice < 0) {
#ifndef QT_NO_DEBUG
        printf("Invalid file handle %x\n", hDevice);
#endif
        return false;
    }

    status = DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize,
            NULL, 0, &bytesReturned, NULL);

    if (!status) {
#ifndef QT_NO_DEBUG
        printf("Error writing 0x%2x to port 0x%2x. Code: 0x%x", value, port, GetLastError());
#endif
        return false;
    }

    return true;
}

bool FanController::wait_until_bitmask_is_value(const HANDLE hDevice,
                                 const unsigned int bitmask,
                                 const unsigned char value)
{
    BYTE currentValue;
    for (WORD i = 0; i < 20000; ++i) {
        if (!read_uchar(hDevice, 0x6C, &currentValue)) {
            return false;
        }
        if ((currentValue & bitmask) == value) {
            return true;
        }
    }
#ifndef QT_NO_DEBUG
    printf("current value: 0x%2x\n", currentValue);
    printf("%s: timeout\n", __FUNCTION__);
#endif
    return false;
}

bool FanController::ec_intro_sequence(HANDLE hDevice)
{
    unsigned char value;
    if (!read_uchar(hDevice, 0x68, &value)) {
        return false;
    }
#ifndef QT_NO_DEBUG
    printf("%s: read 0x%02x from port 0x68\n", __FUNCTION__, value);
#endif
    if (!wait_until_bitmask_is_value(hDevice, 0x02, 0x00)) {
        return false;
    }
    if (!write_uchar(hDevice, 0x6C, 0x59)) {
        return false;
    }
#ifndef QT_NO_DEBUG
    printf("%s: successfully writting 0x59 to port 0x6C\n", __FUNCTION__);
#endif
    return true;
}

bool FanController::ec_close_sequence(HANDLE hDevice)
{
    unsigned char value;
    if (!read_uchar(hDevice, 0x68, &value)) {
        return false;
    }

    if (!wait_until_bitmask_is_value(hDevice, 0x02, 0x00)) {
        return false;
    }

    if (!write_uchar(hDevice, 0x6C, 0xFF)) {
        return false;
    }
#ifndef QT_NO_DEBUG
    printf("%s: Successful.\n", __FUNCTION__);
#endif
    return true;
}

bool FanController::setFan(bool on)
{
    HANDLE lpcDriverHandle;
    //char port_number_string[128];
    //unsigned int port_number;
    //BYTE value;

    lpcDriverHandle = CreateFileA("\\\\.\\LPCFilter", GENERIC_READ|GENERIC_WRITE,
        FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    if (lpcDriverHandle == INVALID_HANDLE_VALUE) {
#ifndef QT_NO_DEBUG
        printf("Unable to open LPC driver. Error 0x%x\n", GetLastError());
#endif
        system("pause");
        return 1;
    }

    if (!wait_until_bitmask_is_value(lpcDriverHandle, 0x80, 0x0)) {
        goto closing_sequence;
    }
    if (!ec_intro_sequence(lpcDriverHandle)) {
        goto closing_sequence;
    }
    // magic sequence begins
    if (!wait_until_bitmask_is_value(lpcDriverHandle, 0x02, 0x00)) {
        goto closing_sequence;
    }
    // in the following line, replace 0x76 with 0x77 to have fan spin at
    // maximum speed
    if ( (on) )
    {
        if (!write_uchar(lpcDriverHandle, 0x68, 0x77)) {
            goto closing_sequence;
        } else {
#ifndef QT_NO_DEBUG
            printf("FAN ON\n");
#endif
        }
    } else
    {
        if (!write_uchar(lpcDriverHandle, 0x68, 0x76)) {
            goto closing_sequence;
        } else {
#ifndef QT_NO_DEBUG
            printf("FAN OFF\n");
#endif
        }
    }

closing_sequence:

    ec_close_sequence(lpcDriverHandle);

    if (CloseHandle(lpcDriverHandle)) {
#ifndef QT_NO_DEBUG
        printf("Driver handler closed successfully\n");
#endif
    } else {
#ifndef QT_NO_DEBUG
        printf("Unable to close handle. Error: %x\n", GetLastError());
#endif
    }
#ifndef QT_NO_DEBUG
    system("pause");
#endif
    return 1;

   //  printf("Enter port number: "); scanf("%s", &port_number_string);
   //  port_number = (unsigned int)strtoul(port_number_string, NULL, 0);
   //  printf("You entered 0x%2X\n", port_number);


   //  system("pause");
   //  return 0;
}






#elif LINUXT

//#define extern static
//#include <asm-generic/ioctl.h>


bool FanController::setFan(bool on)
{
    initialize_ioports();
    if (ec_intro_sequence())
    {
#ifndef QT_NO_DEBUG
        printf("Unable to perform intro sequence");
#endif
        //goto closing_sequence;
        if (ec_outro_sequence())
        {
#ifndef QT_NO_DEBUG
            printf("Unable to perform outro sequence");
#endif
        } else
        {
            printf("ec_outro_sequence successful");
        }
        close_ioports();
        return 1;
    } else {
#ifndef QT_NO_DEBUG
        printf("ec_intro_sequence successful");
#endif
    }

    if (wait_until_bitmask_is_value(0x02, 0x00)) //hDevice parameter?
    {
#ifndef QT_NO_DEBUG
        printf("Error waiting for magic value");
#endif
        //goto closing_sequence;
        if (ec_outro_sequence())
        {
#ifndef QT_NO_DEBUG
            printf("Unable to perform outro sequence");
#endif
        } else
        {
            printf("ec_outro_sequence successful");
        }
        close_ioports();
        return 1;
    }

    int value = 0x76;
    if (on)
    {
        value = 0x77;
    }
    if (write_uchar(0x68, value)) //hDevice?
    {
#ifndef QT_NO_DEBUG
//        printf("Unable to write magic 0x%02X command" + std::to_string(value));
#endif
    } else
    {
#ifndef QT_NO_DEBUG
//        printf("Successfully sent command 0x%02X" + std::to_string(value));
#endif
    }


    //closing_sequence:
    if (ec_outro_sequence())
    {
#ifndef QT_NO_DEBUG
        printf("Unable to perform outro sequence");
#endif
    } else
    {
        printf("ec_outro_sequence successful");
    }

    close_ioports();
}

bool FanController::initialize_ioports()
{
    port.open("/dev/port", std::fstream::in | std::fstream::out | std::fstream::app);
    backup = port.tellg();
}

void FanController::close_ioports()
{
    port.seekg(backup);
    port.close();
}

int FanController::read_uchar(int portN)
{
    int mybackup = port.tellg();
    port.seekg(portN);
    char * mybuffer = new char [1];
    port.read(mybuffer, 1);
    int myvalue = mybuffer[1];
    port.seekg(mybackup);
    return myvalue;
    //TODO? check if 1?
}

bool FanController::write_uchar(int portN, int value)
{
    if (value == 0xff)
    {
        //ERROR
        return 1;
    } else
    {
        int mybackup = port.tellg();
        char * mybuffer = new char [1];
        mybuffer[1]=value;
        port.seekg(portN);
        port.write(mybuffer,1);
        port.seekg(mybackup);
    }
}

bool FanController::wait_until_bitmask_is_value(int mask, int value)
{
    int mybackup = port.tellg();
    port.seekg(0x68);
    char * mybuffer = new char [1];

    for(int i=0; i<10000; i++)
    {
        port.read(mybuffer, 1);
        if ( (mybuffer[1] & mask) == value)
        {
            port.seekg(mybackup);
            return 0;
        } else
        {
        printf("Error; TIMEOUT");
        port.seekg(mybackup);
        return 1;
        }
    }
}

bool FanController::ec_intro_sequence()
{
    //int writePauseVal;
    if(wait_until_bitmask_is_value(0x80,0x00))
        return 1;
    read_uchar(0x68);
    //port.read(writePauseVal, 1);
    if(wait_until_bitmask_is_value(0x02,0x00))
        return 1;
    write_uchar(0x6C,0x59);
}

bool FanController::ec_outro_sequence()
{
    read_uchar(0x68);
    if (wait_until_bitmask_is_value(0x20,0x00))
        return 1;
    write_uchar(0x6C,0xFF);
}

int FanController::debug()
{
    //char * mybuffer = new char [1];
    //int myfd;
    //std::fstream myport;
    //myport.open("/dev/port", std::fstream::in | std::fstream::out | std::fstream::app);
    // int back;
    // back = myport.tellg();
    // myport.seekg(0x68);
    // myport.read(mybuffer,1);
    // myport.seekg(back);
//    off_t backup;
    //myfd = fileno(::fopen("/dev/port", "rw"));
   //  backup = lseek(myfd, 0x00, SEEK_SET);
   //  lseek(myfd, 0x68, SEEK_SET);
   //  read(myfd, );
   //  lseek(myfd, backup, SEEK_SET);
    return read_uchar(0x68);
    // myport.close();
    // char test = mybuffer[0];
    // return mybuffer;
}

#else

bool FanController::setFan(bool on)
{
    if (on)
        system("perl fancontroller.pl MAX");
    else
        system("perl fancontroller.pl NORMAL");
    return true;
}

#endif
