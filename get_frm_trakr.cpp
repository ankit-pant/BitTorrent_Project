//Flash_Share Torrentz
//By:
//  Ankit Pant
//  2018201035

#include "get_frm_trakr.h"



void Get_Info_from_Tracker(string tracker1, string tracker2, string mtorrent_file, string dest_path, string &received_data,string log_file_name){
    cout<<"Getting Info From Server\n";
    ofstream log_file;
    log_file.open(log_file_name,ios::app);
    if(!log_file){
        cout<<"Cannot write to log file\n";
    }
    int sockfd;
    struct sockaddr_in tracker1addr;
    struct sockaddr_in tracker2addr;
    memset(&tracker1addr,0,sizeof(tracker1addr));
    memset(&tracker2addr,0,sizeof(tracker2addr));

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        cout<<"Error Creating Socket!\n";
        time_t now = system_clock::to_time_t(system_clock::now());
        log_file<<"Error Creating Socket @ "<<ctime(&now);
    }
    else{
        tracker1addr.sin_family = AF_INET;
        tracker2addr.sin_family = AF_INET;
        unsigned int i;
        string tracker1_ip="";
        string tracker2_ip="";
        string tracker1_p="",tracker2_p="";

        for(i=0;i<tracker1.length();i++){
            if(tracker1[i]==':'){
                i++;
                break;
            }
            tracker1_ip+=tracker1[i];
        }
        while(i<tracker1.length()){
            tracker1_p+=tracker1[i];
            i++;
        }
        for(i=0;i<tracker2.length();i++){
            if(tracker2[i]==':'){
                i++;
                break;
            }
            tracker2_ip+=tracker2[i];
        }
        while(i<tracker2.length()){
            tracker2_p+=tracker1[i];
            i++;
        }
        tracker1_ip+='\0';
        tracker2_ip+='\0';
        int tracker1_port,tracker2_port;
        tracker1_port=atoi(tracker1_p.c_str());
        tracker2_port=atoi(tracker2_p.c_str());
        int a = inet_pton(AF_INET,tracker1_ip.c_str(),&tracker1addr.sin_addr);
        if(a<=0){
            cout<<"Invalid Tracker1 IP Address\n";
            time_t now = system_clock::to_time_t(system_clock::now());
            log_file<<"Invalid Tracker1 IP Address Entered @ "<<ctime(&now);
        }
        a = inet_pton(AF_INET,tracker2_ip.c_str(),&tracker2addr.sin_addr);
        if(a<=0){
            cout<<"Invalid Tracker2 IP Address\n";
            time_t now = system_clock::to_time_t(system_clock::now());
            log_file<<"Invalid Tracker2 IP Address Entered @ "<<ctime(&now);
        }
        tracker1addr.sin_port = htons(tracker1_port);
        tracker2addr.sin_port = htons(tracker2_port);

        int c = connect(sockfd,(struct sockaddr *)&tracker1addr, sizeof(tracker1addr));
         if(c<0){
                time_t now = system_clock::to_time_t(system_clock::now());
                log_file<<"Connect Failed with Tracker1 @ "<<ctime(&now);
                close(sockfd);
                c = connect(sockfd,(struct sockaddr *)&tracker2addr, sizeof(tracker2addr));
                if(c<0){
                    cout<<"Connect Failed!\n";
                    time_t now = system_clock::to_time_t(system_clock::now());
                    log_file<<"Connect Failed with Tracker2 @ "<<ctime(&now);
                }
                else{
                    send(sockfd,"g",1,0);
                    ifstream input_file;
                    input_file.open(mtorrent_file,ios::in);
                    string temp;
                    getline(input_file,temp);
                    getline(input_file,temp);
                    getline(input_file,temp);
                    getline(input_file,temp);
                    string sha1;
                    getline(input_file,sha1);
                    //cout<<sha1<<"\n";
                    sha1+='\n';
                    sha1+='\0';
                    send(sockfd,sha1.c_str(),sha1.length(),0);
                    sha1="";
                    char *buffer = new char [65536];
                    while(recv(sockfd,buffer,65536,0)){
                        received_data+=buffer;
                    }
                    close(sockfd);
                    received_data+='\0';
                    input_file.close();
                }
        }
        else{
            send(sockfd,"g",1,0);
            ifstream input_file;
            input_file.open(mtorrent_file,ios::in);
            string temp;

            getline(input_file,temp);
            getline(input_file,temp);
            getline(input_file,temp);
            getline(input_file,temp);
            string sha1;
            getline(input_file,sha1);
            cout<<sha1<<"\n";
            sha1+='\n';
            sha1+='\0';
            send(sockfd,sha1.c_str(),sha1.length(),0);
            sha1="";
            char *buffer = new char [65536];
            while(recv(sockfd,buffer,65536,0)){
                received_data+=buffer;
            }
            close(sockfd);
            received_data+='\0';
            input_file.close();
        }
    }
    log_file.close();
}