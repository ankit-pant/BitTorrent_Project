//Flash_Share Torrentz
//By:
//  Ankit Pant
//  2018201035

#include "gen_mtor.h"
#include <fstream>
#include <string.h>
#include <openssl/sha.h>
#include <sstream>

using namespace std;

string ReturnSHA1(char* chunk){
    unsigned char *chnk = (unsigned char *)chunk;
    unsigned char chunk_SHA[20];
    SHA1(chnk,524288,chunk_SHA);
    char buffer[20];
    for(int i=0;i<19;i++)
        sprintf(&buffer[i],"%02x",(unsigned int)chunk_SHA[i]);
    cout<<"Sha1 of chunk "<<buffer<<endl;
    string returned_SHA = (reinterpret_cast<char*>(buffer));
    return returned_SHA;
}

string GetSHA1(string filepath){
    string file_SHA = "";
    ifstream input_file;
    //cout<<filepath<<"\n";
    input_file.open(filepath,ios::binary);
    if(!input_file){
        cout<<"Cannot Open Input File! Please ensure that the Filename and Pathname are correct\n";
        return "-1";
    }
    else{
        long long count = 0;
        char buffer[524288];
        cout<<sizeof(buffer)<<"\n";
        while(!input_file.eof()){
            input_file.read(buffer,sizeof(buffer));
            string temp_sha = ReturnSHA1(buffer);
            file_SHA+=temp_sha;
            count++;
            cout<<"Count: "<<count<<endl;
        }
        input_file.close();
    }
    return file_SHA;
}

int Build_mTorrent_File(string tracker1, string tracker2,string file_path, string mtorrent_file_name, string combined_SHA1){
    ofstream output_file;
    vector<string>file_name;
    //Extracting File Name
    stringstream strstrm(file_path); 
    string temp="";
    while(getline(strstrm,temp,'/')){
        file_name.push_back(temp);
    }
    vector<string>::reverse_iterator riter = file_name.rbegin();
    string file = *riter;
    cout<<"File Name: "<<file<<endl;
    ifstream input_file;
    input_file.open(file_path,ios::binary);
    input_file.seekg(0,ios::end);
    unsigned long long file_size  = input_file.tellg();
    input_file.close();
    cout<<"File Size: "<<file_size<<endl;
    output_file.open(mtorrent_file_name);
    if(!output_file){
        return -1;
    }
    output_file<<tracker1<<"\n";
    output_file<<tracker2<<"\n";
    output_file<<file_path<<"\n";
    output_file<<file_size<<"\n";
    output_file<<combined_SHA1<<"\n";
    output_file.close();
    return 0;
}

