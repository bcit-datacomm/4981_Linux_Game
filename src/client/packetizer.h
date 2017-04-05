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
-- 1.5 - Mar/15/17 - EY - added control packetizing & parsing
-- 2.0 - Mar/30/17 - JA - Removed class
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- This is the packetizer class that holds static functions to be
-- used to pack and unpack game packets.
-- The server should only be sending one type of packet out (GameSync)
-- clients will be making serveral different types of packets which are
-- identified by the headers
------------------------------------------------------------------------------*/
#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <cstdint>

void parseGameSync(const void *syncBuff, size_t bytesReads);
void parseControlMsg(const void *msgBuff, size_t bytesReads);
int packControlMsg(char *buff, size_t bufflen, const char *msg, int32_t id = -1, const char type = 'T');

#endif
