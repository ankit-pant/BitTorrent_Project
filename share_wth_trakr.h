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

#ifndef INC_SHARE_WTH_TRAKR_H
#define INC_SHARE_WTH_TRAKR_H

void Send_Info_to_Tracker(string file_name, string combined_sha, string tracker1, string tracker2, string client,string log_file_name);

#endif