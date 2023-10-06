#ifndef RECORDSTRUCT_H
#define RECORDSTRUCT_H

#include <stdexcept>
#include <cstddef>
#include <string>

struct recordStruct {
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

#endif  // RECORDSTRUCT_H
