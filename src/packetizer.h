/*------------------------------------------------------------------------------
-- SOURCE FILE: Packetizer.h - Packetizer class
--
-- PROGRAM: packetizer (class)
--
-- FUNCTIONS:
-- static void parse(const void * syncBuff, size_t len);
--
--
-- DATE: Feb. 25, 2017
--
-- REVISIONS:
-- 1.0 - Feb/25/17 - EY - Created Class
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- This is the packetizer class that holds static functiosn to be
-- used to pack and unpack game packets.
-- The server should only be sending one type of packet out ( GameSync )
-- clients will be making serveral different types of packets which are
-- identified by the headers
------------------------------------------------------------------------------*/
#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <iostream>
#include <stdint.h>

class Packetizer
{
  public:
    static void parseGameSync(const void * syncBuff, size_t bytesReads);
    static void parseControlMsg(const void * msgBuff, size_t bytesReads);
    static int packControlMsg(char * buff, size_t bufflen, const char * msg, int32_t id = -1, const char type = 'T');
    //static void packGamePlayMsg(const void * msgBuff, size_t bytesReads);
  private:

};

#endif
