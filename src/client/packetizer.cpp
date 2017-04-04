/*------------------------------------------------------------------------------
 * SOURCE FILE: Packetizer.h - Packetizer class
 *
 * PROGRAM: packetizer (class)
 *
 * FUNCTIONS:
 * static void parse(const void * syncBuff, size_t len);
 *
 *
 * DATE: Feb. 25, 2017
 *
 * REVISIONS:
 * 1.0 - Feb/25/17 - EY - implemented functon
 *
 * DESIGNER: Eva Yu
 *
 * PROGRAMMER: Eva Yu
 *
 * NOTES:
 * This is the packetizer class that holds static functiosn to be
 * used to pack and unpack game packets.
 * The server should only be sending one type of packet out (GameSync)
 * clients will be making serveral different types of packets which are
 * identified by the headers
 ------------------------------------------------------------------------------*/

#include <iostream>
#include <cstdint>
#include <string>
#include <cstring>

#include "packetizer.h"
#include "../UDPHeaders.h"
#include "../game/GameManager.h"
#include "../server/servergamestate.h"

/*------------------------------------------------------------------------------
 * FUNCTION: packControlMsg
 *
 * DATE: Mar. 15, 2017
 *
 * REVISIONS:
 * 1.0 - Feb/25/17 - EY - implmeted functon
 *
 * DESIGNER: Eva Yu
 *
 * PROGRAMMER: Eva Yu
 *
 * INTERFACE: int packControlMsg (char * buff, size_t bufflen,
 *                                               const char * msg, int32_t id ,
 *                                               const char type)
 * buff    -- buffer to write to
 * bufflen -- size of buff
 * msg     -- the message to write to the buffer
 * id      -- id of the user ( default -1 if no id )
 * type    -- type of control message : T for text or C for control ( defaults to type T )
 *
 * RETURNS:
 * int representing the number of bytes in the message to send
 *
 * NOTES:
 * this is a packetizer function for TCP messages
 ------------------------------------------------------------------------*/
int packControlMsg(char *buff, size_t bufflen, const char *msg, int32_t id , const char type) {
    //make sure buffer can fit message, otherwise, truncate
    const size_t maxmsglen = ((bufflen < (strlen(msg) + 7)) ? bufflen - 7 : strlen(msg));
    *(reinterpret_cast<int32_t *>(buff)) = id; //insert id to buffer
    buff[4] = type;
    buff[5] = '/';
    memcpy(buff + 6, msg, maxmsglen);
    buff[maxmsglen + 6] = '\0';
    return static_cast<int>(strlen(msg)) + 6;
}

/*------------------------------------------------------------------------------
 * FUNCTION: parseControlMsg
 *
 * DATE: Mar. 15, 2017
 *
 * REVISIONS:
 * Version, Date and Description
 *
 * DESIGNER: Eva Yu
 *
 * PROGRAMMER: Eva Yu
 *
 * INTERFACE: void parseControlMsg (const void * msgBuff, size_t bytesReads)
 * msgBuff  --  the pointer to the beginning of the buffer
 * bytesRead -- the number of bytes read into the buffer
 *
 * NOTES:
 * static function that parses an incoming control message
 --------------------------------------------------------------------------*/
void parseControlMsg(const void *msgBuff, size_t bytesReads) {
    const int32_t id = *(reinterpret_cast<const int32_t *>(msgBuff));
    const char *pBuff = reinterpret_cast<const char *>(msgBuff) + sizeof(int32_t);
    switch(*pBuff++) {
        case 'C': // NEW CONNECTIONS ONLY!
            if (*pBuff++ == '/') {
                // insertplayer new player
                std::string msg(pBuff, bytesReads - sizeof(int32_t) - sizeof(char));
                GameManager::instance()->createMarine(id);
            }
            break;
        case 'T':
            logv("\nId: %d\tMsg: %s", id, ++pBuff);
            break;
        default:
            loge("Cannot parse control message.\n");
            break;
    }
}

/*------------------------------------------------------------------------------
 * FUNCTION: parse
 *
 * DATE: Feb. 26, 2017
 *
 * REVISIONS:
 * 1.0 - Feb/12/17 - IM - completed design and pseudo code
 * 2.0 - Feb/25/17 - EY - implmeted functon and made minor changes design
 * 2.1 - Feb/27/17 - EY - changed cstyle casts to use reinterpret casts
 *
 * DESIGNER: Isaac Morneau & Eva Yu
 *
 * PROGRAMMER: Eva Yu
 *
 * INTERFACE: void parse (const void * syncBuff, size_t bytesReads)
 * syncBuff - represents the starting locations of the buffer that was
 *              read into from socket's rcv call
 * bytesRead - represents the number of bytes that were read
 *            (the size of the gamesync struct to be casted)
 *
 * NOTES:
 * This is a static function from Packetizer class that
 *  parses a sync packet and immediately calls the updates to the
 * game of the players, zombies, and actions
 * note: the packet passed from the server must match
 * the gamesync packet exactly
 --------------------------------------------------------------------------*/
void parseGameSync(const void *syncBuff, size_t bytesReads) {
    int32_t *pBuff = reinterpret_cast<int32_t *>(const_cast<void *>(syncBuff)); // cast the buff to read 4 bytes at a time

    if (*pBuff++ != static_cast<int32_t>(UDPHeaders::SYNCH)) {
        return; // Check that it is a sync pack
    }
    int32_t *pEnd = pBuff + bytesReads / sizeof(int32_t); // get end location

    PlayerData *player;
    AttackAction *attack;
    ZombieData *zombie;
    DeleteAction *deletion;

    while (pBuff <= pEnd) {
        switch(static_cast<UDPHeaders>(*pBuff++)) {
            case UDPHeaders::PLAYERH:
                for (int32_t i = 0, pCount = *pBuff++; i < pCount; ++i) {
                    player = reinterpret_cast<PlayerData *>(pBuff);
                    GameManager::instance()->updateMarine(*player);
                    pBuff = reinterpret_cast<int32_t *>(++player);
                }
                break;
            case UDPHeaders::ATTACKACTIONH: 
                for (int32_t i = 0, aCount = *pBuff++; i < aCount; ++i) {
                    attack = reinterpret_cast<AttackAction *>(pBuff);
                    GameManager::instance()->handleAttackAction(*attack);
                    pBuff = reinterpret_cast<int32_t *>(++attack);
                }
                break;
            case UDPHeaders::ZOMBIEH: 
                for (int32_t i = 0, zCount = *pBuff++; i < zCount; ++i){
                    zombie = reinterpret_cast<ZombieData *>(pBuff);
                    GameManager::instance()->updateZombie(*zombie);
                    pBuff = reinterpret_cast<int32_t *>(++zombie);
                }
                break;
            case UDPHeaders::DELETE:
                for (int32_t i = 0, dCount = *pBuff++; i < dCount; ++i){
                    deletion = reinterpret_cast<DeleteAction *>(pBuff);
                    deleteEntity(*deletion);
                    pBuff = reinterpret_cast<int32_t *>(++deletion);
                }
                break;
            default:
                break;
        }
    }
}
