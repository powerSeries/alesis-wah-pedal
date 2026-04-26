#pragma once
#ifndef USERDATA_H
#define USERDATA_H


#include <EEPROM.h>

const int EEPROM_SIZE = 512;

#define DEFAULT_BUFFER_SIZE 64

#define FIRST_RUN_FLAG_ADDR 0
#define FIRST_RUN_FLAG_VALUE 123

struct UserConfig {
    byte first_run_flag;
    char router_id[DEFAULT_BUFFER_SIZE];
    char router_secret[DEFAULT_BUFFER_SIZE];
};
#endif
