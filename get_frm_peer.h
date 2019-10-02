//Flash_Share Torrentz
//By:
//  Ankit Pant
//  2018201035

#include <string>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono;

#ifndef INC_GET_FRM_PEER_H
#define INC_GET_FRM_PEER_H

void Get_File_from_Peer(string peer_addr, string filepath,string sha_of_file);


#endif