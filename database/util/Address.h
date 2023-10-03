#ifndef ADDRESS_H
#define ADDRESS_H

class Address {
    public:
        int blkNumber;
        int offset;
        Address(int blkNumber, int offset){
            this->blkNumber = blkNumber;
            this->offset = offset;
        }
        Address(){}
};

#endif