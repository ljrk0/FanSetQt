#ifndef FANCONTROLLER_H
#define FANCONTROLLER_H

#ifdef _MSC_VER
#include <tchar.h>
#include <Windows.h>
#include <assert.h>

#elif LINUXT

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>

#else

#include <stdlib.h>

#endif

//#define LINUXT 1
/* activates Linux testing re-implementation of the perl script */

class FanController
{
private:
#ifdef _MSC_VER
    bool read_uchar(const HANDLE hDevice,
                    const unsigned char port,
                    unsigned char* const value);
    bool write_uchar(const HANDLE hDevice,
                     const unsigned char port,
                     const unsigned char value);
    bool wait_until_bitmask_is_value(const HANDLE hDevice,
                                     const unsigned int bitmask,
                                     const unsigned char value);
    bool ec_intro_sequence(HANDLE hDevice);
    bool ec_close_sequence(HANDLE hDevice);
#elif LINUXT
    std::fstream port;
    int backup;
    bool wait_until_bitmask_is_value(int mask, int value);
    int read_uchar(int portN);
    bool write_uchar(int portN, int value);
    bool ec_outro_sequence();
    void close_ioports();
    bool initialize_ioports();
    bool ec_intro_sequence();
#endif
public:
    FanController();
    bool setFan(bool on);
#ifdef LINUXT
    int debug();
#endif
};

#endif // FANCONTROLLER_H
