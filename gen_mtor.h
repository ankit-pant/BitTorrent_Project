//Flash_Share Torrentz
//By:
//  Ankit Pant
//  2018201035

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#ifndef INC_GEN_MTOR_H
#define INC_GEN_MTOR_H

string ReturnSHA1(unsigned char* chunk);
string GetSHA1(string filepath);
int Build_mTorrent_File(string tracker1, string tracker2, string file_path, string mtorrent_file_name, string combined_SHA1);




#endif
