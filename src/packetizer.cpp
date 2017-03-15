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

#include <iostream>
#include <stdint.h>
#include <string>
#include <string.h>
#include "packetizer.h"
#include "UDPHeaders.h"

int Packetizer::packControlMsg(char * buff, size_t bufflen, const char * msg, int32_t id , const char type)
{
    //make sure buffer can fit message, otherwise, truncate
    size_t maxmsglen = ( ( bufflen < (strlen(msg) + 7) )? bufflen - 7 : strlen(msg) );
    *(reinterpret_cast<int32_t *>(buff)) = id; //insert id to buffer
    buff[4] = type;
    buff[5] = '/';
    memcpy(buff+6, msg, maxmsglen);
    buff[maxmsglen + 6] = '\0';
    return static_cast<int>(strlen(msg))+6;
}

void Packetizer::parseControlMsg(const void * msgBuff, size_t bytesReads){
    char *pBuff;
    int32_t id;
    id = *(reinterpret_cast<const int32_t *>(msgBuff));
    pBuff = reinterpret_cast<char *>(const_cast<void *>(msgBuff));
    pBuff += sizeof(int32_t);
    switch(*pBuff++)
    {
        case 'C':
            if(*pBuff++ == '/'){
                std::string msg(pBuff, bytesReads-sizeof(int32_t)-sizeof(char));
                //insertplayer(id,msg);
            }
            break;

        case 'T':
            std::cout << "\nId: " << id << "\tMsg: " << pBuff;
            break;

        default:
            std::cerr << "cannot parse control message.";
    }
}

/*------------------------------------------------------------------------------
-- FUNCTION: parse
--
-- DATE: Feb. 26, 2017
--
-- REVISIONS:
-- 1.0 - Feb/12/17 - IM - completed design and pseudo code
-- 2.0 - Feb/25/17 - EY - implmeted functon and made minor changes design
-- 2.1 - Feb/27/17 - EY - changed cstyle casts to use reinterpret casts
--
-- DESIGNER: Isaac Morneau & Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: void parse (const void * syncBuff, size_t bytesReads)
-- syncBuff - represents the starting locations of the buffer that was
--              read into from socket's rcv call
-- bytesRead - represents the number of bytes that were read
--            (the size of the gamesync struct to be casted)
--
-- NOTES:
-- This is a static function from Packetizer class that
--  parses a sync packet and immediately calls the upadtes to the
-- game of the palyers, zombies, and actions
-- note: the packet passed from the server must match
-- the gamesync packet exactly
--------------------------------------------------------------------------*/
 void Packetizer::parseGameSync(const void * syncBuff, size_t bytesReads)
{
  int32_t *pBuff;
  int32_t *pEnd;
  PlayerData * player;
  MoveAction * moves;
  AttackAction * attacks;
  ZombieData * zombie;
  pBuff = reinterpret_cast<int32_t *>(const_cast<void *>(syncBuff)); // cast the buff to read 4 bytes at a time

  if (*pBuff++ != SYNC) return; // Check that it is a sync pack

  size_t len = bytesReads / sizeof(int32_t); // get end location
  pEnd = pBuff + len;

  while (pBuff <= pEnd) {
    switch(*pBuff++) {

      case PLAYERH: {  // enclose in braces to prevent
        int32_t pCount = *pBuff++;
        for(int32_t i = 0; i  < pCount; i++) {
          player = reinterpret_cast<PlayerData *>(pBuff);
          std::cout << "\nPlayer playerid:" << player->playerid;
          std::cout << "\n\tPlayer xpos:" << player->xpos;
          std::cout << "\n\tPlayer ypos:" << player->ypos;
          std::cout << "\n\tPlayer vel:" << player->vel;
          std::cout << "\n\tPlayer direction:" << player->direction;
          std::cout << "\n\tPlayer health:" << player->health;
          std::cout << std::endl;
          pBuff = reinterpret_cast<int32_t *>(&(player->moves));
          moves = player->moves;
          /*
          UpdatePlayer (player->playerid,
                          player->xpos,
                          player->ypos,
                          player->xvel,
                          player->yvel,
                          player->direction);
          */
          for(int32_t j = 0; j  < player->nmoves; j++) {
            /*
            UpdatePlayerAction (player->playerid,
                            action->actionid,
                            action->xpos,
                            action->ypos,
                            action->direction);
            */
            moves++;
          }
          pBuff = reinterpret_cast<int32_t *> (&(player->attacks));
          attacks = player->attacks;

          for(int32_t j = 0; j  < player->nattacks; j++) {
            /*
            UpdatePlayerAction (player->playerid,
                            action->actionid,
                            action->xpos,
                            action->ypos,
                            action->direction);
            */
            attacks++;
          }
          pBuff = reinterpret_cast<int32_t *>(++player);
        }
        break;
      } // End of PLAYERH case, must be enclosed in braces.
      case ZOMBIEH: {
        int32_t zCount = *pBuff++;
        for(int32_t i = 0; i  < zCount; i++){
          zombie = reinterpret_cast<ZombieData *>(pBuff);
          std::cout << "\nZombie zombieid:" << zombie->zombieid;
          std::cout << "\n\tZombie xpos:" << zombie->xpos;
          std::cout << "\n\tZombie ypos:" << zombie->ypos;
          std::cout << "\n\tZombie direction:" << zombie->direction;
          std::cout << "\n\tZombie health:" << zombie->health;
          std::cout << std::endl;
          pBuff = reinterpret_cast<int32_t *>(++zombie);
          /*
          UpdateZombie (zombie->zombieid,
                          zombie->xpos,
                          zombie->ypos,
                          zombie->health,
                          zombie->direction);
          */
        }
        break;
      }
     }
   }
}
