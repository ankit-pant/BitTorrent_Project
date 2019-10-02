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

#ifndef INC_GET_FRM_TRAKR_H
#define INC_GET_FRM_TRAKR_H

void Get_Info_from_Tracker(string tracker1,string tracker2, string mtorrent_file,string dest_path, string &received_data, string log_file_name);
void Remove_Info_from_Tracker(string tracker1, string tracker2, string abs_file_path, string log_file_name);


#endif