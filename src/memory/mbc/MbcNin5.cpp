/*
   hhugboy Game Boy emulator
   copyright 2013-2016 taizou
   Based on GEST
   Copyright (C) 2003-2010 TM
   Incorporating code from VisualBoyAdvance
   Copyright (C) 1999-2004 by Forgotten

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "MbcNin5.h"

#include <stdio.h>
#include "../../GB.h"
#include "../../main.h"
// ^ can we not

void MbcNin5::writeMemory(unsigned short address, register byte data) {

    if(address < 0x2000)// Is it a RAM bank enable/disable?
    {
        RAMenable = ( (data&0x0A) == 0x0A ? 1 : 0);
        return;
    }

    if(address < 0x3000)
    {
        rom_bank = data|(MBChi<<8);
        cart_address = rom_bank<<14;

        cart_address &= rom_size_mask[(*gbRom)->ROMsize];

        cart_address += multicartOffset;

        MBClo = data;

        gbMemMap[0x4] = &(*gbCartridge)[cart_address];
        gbMemMap[0x5] = &(*gbCartridge)[cart_address+0x1000];
        gbMemMap[0x6] = &(*gbCartridge)[cart_address+0x2000];
        gbMemMap[0x7] = &(*gbCartridge)[cart_address+0x3000];

        //  if(origData == 0x69) {
        //    	char buff[100];
        //		sprintf(buff,"%X %X %X",origData,data,cart_address);
        //		debug_print(buff);
        //  }



        return;
    }

    if(address < 0x4000) // Is it a ROM bank switch?
    {
        data = data&1;

        rom_bank = MBClo|(data<<8);

        cart_address = rom_bank<<14;

        cart_address &= rom_size_mask[(*gbRom)->ROMsize];

        cart_address += multicartOffset;

        MBChi = data;

        gbMemMap[0x4] = &(*gbCartridge)[cart_address];
        gbMemMap[0x5] = &(*gbCartridge)[cart_address+0x1000];
        gbMemMap[0x6] = &(*gbCartridge)[cart_address+0x2000];
        gbMemMap[0x7] = &(*gbCartridge)[cart_address+0x3000];

        return;
    }

    if(address < 0x6000) // Is it a RAM bank switch?
    {
        if((*gbRom)->rumble)
        {
            if(data&0x08) {
                *gbRumbleCounter += 4;
            } else {
                *gbRumbleCounter = 0;
            }

            data &= 0x07;
        }

        if((*gbRom)->RAMsize <= 2) // no need to change it if there isn't over 8KB ram
            return;

        data &= 0x0F;

        if(data > maxRAMbank[(*gbRom)->RAMsize])
            data = maxRAMbank[(*gbRom)->RAMsize];

        ram_bank = data;

        int madr = (data<<13) + multicartRamOffset;
        gbMemMap[0xA] = &(*gbCartRam)[madr];
        gbMemMap[0xB] = &(*gbCartRam)[madr+0x1000];
        return;
    }

    if(address<0x8000)
    {
        /*  if(++bc_select == 2 && rom->ROMsize>1)
          {
             MBChi = (data&0xFF);

             cart_address = (MBChi<<1)<<14;

             mem_map[0x0] = &cartridge[cart_address];
             mem_map[0x1] = &cartridge[cart_address+0x1000];
             mem_map[0x2] = &cartridge[cart_address+0x2000];
             mem_map[0x3] = &cartridge[cart_address+0x3000];

             mem_map[0x4] = &cartridge[cart_address+0x4000];
             mem_map[0x5] = &cartridge[cart_address+0x5000];
             mem_map[0x6] = &cartridge[cart_address+0x6000];
             mem_map[0x7] = &cartridge[cart_address+0x7000];
          }*/
        return;
    }

    /*  if(address >= 0xA000 && address < 0xC000)
      {
         if(!RAMenable || !rom->RAMsize)
            return;
      }*/

    gbMemMap[address>>12][address&0x0FFF] = data;

}
