//Flash_Share Torrentz
//By:
//  Ankit Pant
//  2018201035

#include "gen_mtor.h"
#include "share_wth_trakr.h"
#include "get_frm_trakr.h"
#include "get_frm_peer.h"
#include <iostream>
#include <sstream>
#include <linux/limits.h>
#include <chrono>
#include <ctime>
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]){
    if(argc<4){
        cout<<"Invalid Program Agruments... Program Will Exit\n";
        return -1;
    }
    cout<<"\033c";
    cout<<"\033[3J";
    string client_addr = argv[1];
    string tracker1 = argv[2];
    string tracker2 = argv[3];
    string log_file_name = argv[4];
    string input="";
    vector<string> command;
    cout<<"\t\t\t********Welcome to Flash_Torrentz********\n\n";
    cout<<">Enter the commands followed by enter\n";
    cout<<">Type quit and press enter to quit\n\n";
    unsigned int i,j;
    ofstream log_file;
    log_file.open(log_file_name,ios::app);
    if(!log_file){
        cout<<"Cannot write to log file\n";
    }
    while (input!="quit"){
        cout<<"Enter a command:  ";
        command.clear();
        getline(cin,input);
        string temp="";
        bool flag = false;
        for(i=0;i<input.length();){
            if(input[i]=='\"'){
                for(j=i+1;input[j]!='\"';j++){
                    temp+=input[j];
                    flag=true;
                }
                command.push_back(temp);
                temp="";
                i = j+2;
                continue;
            }
            else if(input[i]=='\\'){
                temp+=input[i+1];
                i+=2;
                continue;
            }
            else if(input[i]==' ' && flag==false){
                command.push_back(temp);
                temp="";
                i++;
                continue;
            }
            else{
                temp+=input[i];
                i++;
            }
        }
        command.push_back(temp);
        vector<string>::iterator iter = command.begin();
        if(*iter=="share"){
            if(command.size()<3){
                cout<<"Invalid Format! Please enter the command with the correct syntax\n";
                cout<<"Usage: share <local_file_path> <file_name>.mtorrent\n";
                continue;
            }
            iter++;
            string file_path = *iter;
            iter++;
            if(file_path[0]=='~'){
                string root = getenv("HOME");
                file_path.replace(0,1,root);
            }
            const char*  fpath = file_path.c_str();
            char  abfpath[PATH_MAX+1];
            realpath(fpath,abfpath);
            string absolute_file_path = (string)abfpath;
            string mtorrent_file_name = *iter;
            string combined_SHA1;
            combined_SHA1 = GetSHA1(absolute_file_path);
            if(combined_SHA1=="-1"){
                time_t now = system_clock::to_time_t(system_clock::now());
                log_file<<"Error Creating mTorrent File! Input File Not Found @ "<<ctime(&now);
                continue;
            }
            int status = Build_mTorrent_File(tracker1,tracker2,absolute_file_path,mtorrent_file_name,combined_SHA1);
            if(status==-1){
                cout<<"Error Creating mTorrent File! File Not Found.\n";
                time_t now = system_clock::to_time_t(system_clock::now());
                log_file<<"Error Creating mTorrent File! File Not Found @ "<<ctime(&now);
                continue;
            }
            Send_Info_to_Tracker(absolute_file_path, combined_SHA1, tracker1, tracker2, client_addr,log_file_name);
        }
        else if(*iter=="get"){
            cout<<"Will Download Now\n";
            if(command.size()<3){
                cout<<"Invalid Format! Please enter the command with the correct syntax\n";
                cout<<"Usage: get <path_to_.mtorrent_file> <destination_path>\n";
                continue;
            }
            string mtorrent_file_path;
            string output_file;
            ++iter;
            mtorrent_file_path = *iter;
            if(mtorrent_file_path[0]=='~'){
                string root = getenv("HOME");
                mtorrent_file_path.replace(0,1,root);
            }
            const char* mtor_path = mtorrent_file_path.c_str();
            char abmtorpath[PATH_MAX+1];
            realpath(mtor_path,abmtorpath);
            string mtorrent_file = (string)abmtorpath;
            ++iter;
            string dest_path = *iter;
            string received_data="";
            Get_Info_from_Tracker(tracker1,tracker2,mtorrent_file,dest_path,received_data,log_file_name);
            string peer_addr="", sha_of_file="", filepath="";
            long long i;
            if(received_data==""){
                cout<<"Cannot Receive from Trackers\n";
                time_t now = system_clock::to_time_t(system_clock::now());
                log_file<<"Cannot Receive from Trackers @ "<<ctime(&now);
            }
            else{
                for (i=0;received_data[i]!=' ';i++){
                    peer_addr+=received_data[i];
                }
                i++;
                for (;received_data[i]!=' ';i++){
                    filepath+=received_data[i];
                }
                i++;
                for (;received_data[i]!='\n';i++){
                    sha_of_file+=received_data[i];
                }
                Send_Info_to_Tracker(filepath,sha_of_file,tracker1,tracker2,client_addr,log_file_name);
                Get_File_from_Peer(peer_addr,filepath,sha_of_file);
            }
        }
        else if(*iter=="show"){
            iter++;
            if(*iter=="downloads"){
                cout<<"This section shows all your downloaded files\n";
                cout<<"Will be up and running as soon as Download from peer is implemented\n";
            }
            else{
                cout<<"Invalid Command!\n";
                continue;
            }
        }
        else if(*iter=="remove"){
            string file_name;
            ++iter;
            file_name = *iter;
            const char* file_path = file_name.c_str();
            char abs_file_path[PATH_MAX+1];
            realpath(file_path,abs_file_path);
            Remove_Info_from_Tracker(tracker1, tracker2, abs_file_path,log_file_name);
            unlink(abs_file_path);
            continue;
        }
        else if(*iter=="quit"){
            cout<<"Quitting the program...\n";
        }
        else{
            cout<<"Invalid Command!\n";
        }
        cout<<endl;
    }
    log_file.close();
    cout<<endl;
    return 0;
}
