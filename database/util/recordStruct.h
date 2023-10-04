#ifndef RECORDSTRUCT_H
#define RECORDSTRUCT_H
#include <ctime>
// Include the string library
#include <string>
#include "Address.h"

struct recordStruct
{
    /* data */
    Address address;

    std::string GAME_DATE_EST;
    int TEAM_ID_home;
    int PTS_home;
    float FG_PCT_home;
    float FT_PCT_home;
    float FG3_PCT_home;
    int AST_home;
    int REB_home;
    bool HOME_TEAM_WINS;
};


#endif