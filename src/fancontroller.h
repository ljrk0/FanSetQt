#ifndef FANCONTROLLER_H
#define FANCONTROLLER_H

#ifdef _MSC_VER
#include <tchar.h>
#include <Windows.h>
#include <assert.h>
#endif

#include <stdio.h>
#include <stdlib.h>


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
#endif
public:
    FanController();
    bool setFan(bool on);
};

#endif // FANCONTROLLER_H
