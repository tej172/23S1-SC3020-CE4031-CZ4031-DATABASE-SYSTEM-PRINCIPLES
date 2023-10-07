#ifndef RECORDSTRUCT_H
#define RECORDSTRUCT_H
#include <ctime>
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

    // Define the size of each field in bytes
    static const size_t FIELD_SIZES[];
    static const size_t NUM_FIELDS;
};

// Define the values of FIELD_SIZES and NUM_FIELDS
const size_t recordStruct::FIELD_SIZES[] = {
    10, // GAME_DATE_EST
    4,  // TEAM_ID_home
    4,  // PTS_home
    4,  // FG_PCT_home
    4,  // FT_PCT_home
    4,  // FG3_PCT_home
    4,  // AST_home
    4,  // REB_home
    1   // HOME_TEAM_WINS
};

const size_t recordStruct::NUM_FIELDS = sizeof(FIELD_SIZES) / sizeof(FIELD_SIZES[0]);

#endif