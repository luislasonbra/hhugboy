
#ifndef HHUGBOY_MBCUNLMAKONOLD1_H
#define HHUGBOY_MBCUNLMAKONOLD1_H



#include "MbcNin1.h"
class MbcUnlMakonOld1: public BasicMbc {
   public:
        MbcUnlMakonOld1(int originalRomSize);
        virtual void writeMemory(unsigned short address, register byte data) override;

        virtual void resetVars(bool preserveMulticartState) override;

        virtual void readMbcSpecificVarsFromStateFile(FILE *savefile) override;
        virtual void writeMbcSpecificVarsToStateFile(FILE *savefile) override;

        int originalRomSize;

    protected:
        void handleOldMakonCartModeSet(unsigned short address, byte data);
        bool isWeirdMode;

    private:
        byte flippo1[8] = {0,1,2,4,3,6,5,7};
};


#endif //HHUGBOY_MBCUNLMAKONOLD1_H
