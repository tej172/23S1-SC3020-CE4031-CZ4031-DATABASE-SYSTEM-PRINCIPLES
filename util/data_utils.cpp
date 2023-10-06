// const size_t FIELD_SIZES[] = {
//     10, // GAME_DATE_EST
//     4,  // TEAM_ID_home
//     4,  // PTS_home
//     4,  // FG_PCT_home
//     4,  // FT_PCT_home
//     4,  // FG3_PCT_home
//     4,  // AST_home
//     4,  // REB_home
//     1   // HOME_TEAM_WINS
// };

// const size_t NUM_FIELDS = sizeof(FIELD_SIZES) / sizeof(FIELD_SIZES[0]);

// // Function to calculate the size of a record
// size_t calculateRecordSize() {
//     size_t recordSize = 0;
//     for (size_t i = 0; i < NUM_FIELDS; ++i) {
//         recordSize += FIELD_SIZES[i];
//     }
//     return recordSize;
// }