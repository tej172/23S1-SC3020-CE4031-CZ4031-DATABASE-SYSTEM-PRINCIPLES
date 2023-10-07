// Function to calculate the size of a record
size_t calculateRecordSize(const recordStruct& record) {
    size_t recordSize = 0;
    for (size_t i = 0; i < recordStruct::NUM_FIELDS; ++i) {
        switch (i) {
            case 0:
                recordSize += record.GAME_DATE_EST.size();
                break;
            case 1:
            case 2:
                recordSize += sizeof(int);
                break;
            case 3:
            case 4:
            case 5:
                recordSize += sizeof(float);
                break;
            case 6:
            case 7:
                recordSize += sizeof(int);
                break;
            case 8:
                recordSize += sizeof(bool);
                break;
            default:
                break;
        }
    }
    return recordSize;
}