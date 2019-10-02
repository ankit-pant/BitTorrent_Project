//Flash_Share Torrentz
//By:
//  Ankit Pant
//  2018201035

#include "trakr.h"

struct seeder_info{
    string client_addr = "";
    string file_name = "";
    string sha1_str = "";
};

void Update_Seeder_File(int connfd){
    char *buffer = new char [65536];
    string received_data="";
    cout<<"Will Read from Client Here\n";
    while(recv(connfd,buffer,65536,0)){
        received_data+=buffer;
    }
    received_data+='\0';
    cout<<"Received in string: "<<received_data<<endl;
    struct seeder_info seeder_details;
    long long i=0;
    while(received_data[i]!='\n'){
        seeder_details.client_addr+=received_data[i];
        i++;
    }
    i++;
    while(received_data[i]!='\n'){
        seeder_details.file_name+=received_data[i];
        i++;
    }
    i++;
    while(received_data[i]!='\n'){
        seeder_details.sha1_str+=received_data[i];
        i++;
    }
    ofstream output_file;
    output_file.open("seeders_list.bin",ios::binary | ios::app);
    if(!output_file){
        cout<<"Error Writing to seeders_list file\n";
    }
    output_file<<seeder_details.client_addr<<" "<<seeder_details.file_name<<" "<<seeder_details.sha1_str<<"\n";
    output_file.close();
    
}

void Find_Seeder_Info(int connfd){
    cout<<"Send seeder info\n";
    char *buffer = new char [65536];
    string received_data="";
    cout<<"Will Read from Client Here\n";
    recv(connfd,buffer,65536,0);
    received_data+=buffer;
    cout<<"Received in string: \n"<<received_data<<endl<<endl;
    ifstream input_file;
    input_file.open("seeders_list.bin",ios::binary | ios::in);
    string req_sha1 = "";
    string temp;
    string temp2="";
    long long i;
    while(getline(input_file,temp)){
        temp2="";
        req_sha1 = "";
        temp+='\n';
        for(i=0;temp[i]!=' ';i++)
            temp2+=temp[i];
        temp2 = "";
        i++;
        for(;temp[i]!=' ';i++)
            temp2+=temp[i];
        i++;
        for(;temp[i]!='\n';i++){
            req_sha1+=temp[i];
        }
        req_sha1+='\n';
        cout<<"REG_SEQ: "<<req_sha1<<endl;
        if(req_sha1.compare(received_data)==0){
            temp+='\0';
            send(connfd,temp.c_str(),temp.length(),0);
            temp="";
        }
        temp =  "";
        req_sha1 = "";
    }
    close(connfd);
    input_file.close();
}

void Remove_Seeder_Info(int connfd){
    cout<<"Remove Seeder info\n";
    char *buffer = new char [65536];
    string received_data="";
    recv(connfd,buffer,65536,0);
    received_data+=buffer;
    string recv_sha="",recv_addr="";
    long long i;
    for(i=0;received_data[i]!=' ';i++){
        recv_sha+=received_data[i];
    }
    i++;
    for(i=0;received_data[i]!='\n';i++){
        recv_addr+=received_data[i];
    }
    ifstream input_file;
    ofstream temp_file;
    input_file.open("seeders_list.bin",ios::binary | ios::in);
    temp_file.open("temp.bin",ios::binary | ios::app);
    string req_sha1 = "";
    string temp;
    string temp2="";
    string temp3="";
    
    while(getline(input_file,temp)){
        temp2="";
        req_sha1 = "";
        temp+='\n';
        for(i=0;temp[i]!=' ';i++)
            temp2+=temp[i];
        
        temp3 = "";
        i++;
        for(;temp[i]!=' ';i++)
            temp3+=temp[i];
        i++;
        for(;temp[i]!='\n';i++){
            req_sha1+=temp[i];
        }
        req_sha1+='\n';
        if(!(req_sha1.compare(recv_sha)==0 && temp2.compare(recv_addr)==0)){
            temp_file<<temp2<<" "<<temp3<<" "<<req_sha1<<"\n";
        }
        temp =  "";
        req_sha1 = "";
    }
    close(connfd);
    input_file.close();
    remove("seeders_list.bin");
    rename("temp.bin","seeders_list.bin");
}

void Read_from_Seeder(int connfd){
    char *ch = new char;
    recv(connfd,ch,1,0);
    if (*ch=='u'){
        Update_Seeder_File(connfd);
    }
    else if(*ch=='g'){
        Find_Seeder_Info(connfd);
    }
    else if(*ch=='r'){
        Remove_Seeder_Info(connfd);
    }
}

void * detach_thread( void* args){
    int connfd;
    connfd = *((int *) args);
    free(args);
    pthread_detach(pthread_self());
    Read_from_Seeder(connfd);
    close(connfd);
    return (NULL);
}

int main(int argc, char *argv[] ){
    if(argc<4){
        cout<<"Invalid Program Agruments... Program Will Exit\n";
        return -1;
    }
    cout<<"\033c";
    cout<<"\033[3J";
    cout<<"\t\t\t********Flash_Torrentz Tracker:1********\n\n";
    string self_addr = argv[1];
    string oth_trakr_addr = argv[2];
    string seederlist_file_name = argv[3];
    string log_file_name = argv[4];
    ofstream log_file;
    log_file.open(log_file_name,ios::app);
    if(!log_file){
        cout<<"Cannot write to log file\n";
    }
    unsigned i;
    string self_ip="",self_p="", oth_trakr_ip="", oth_trakr_p="";
    for(i=0;i<self_addr.length();i++){
        if(self_addr[i]==':'){
            i++;
            break;
        }
        self_ip+=self_addr[i];
    }
    while(i<self_addr.length()){
        self_p+=self_addr[i];
        i++;
    }
    for(i=0;i<oth_trakr_addr.length();i++){
        if(oth_trakr_addr[i]==':'){
            i++;
            break;
        }
        oth_trakr_ip+=oth_trakr_addr[i];
    }
    while(i<oth_trakr_addr.length()){
        oth_trakr_p+=oth_trakr_addr[i];
        i++;
    }
    self_ip+='\0';
    oth_trakr_ip+='\0';
    int self_port, oth_trakr_port;
    self_port = atoi(self_p.c_str());
    oth_trakr_port = atoi(oth_trakr_p.c_str());
    struct sockaddr_in selfaddr,oth_trakraddr;
    memset(&selfaddr,0,sizeof(selfaddr));
    memset(&oth_trakraddr,0,sizeof(oth_trakraddr));
    selfaddr.sin_family = AF_INET;
    oth_trakraddr.sin_family = AF_INET;
    int a = inet_pton(AF_INET,self_ip.c_str(),&selfaddr.sin_addr);
    if(a<=0){
        cout<<"Invalid Tracker2 IP Address\n";
        time_t now = system_clock::to_time_t(system_clock::now());
        log_file<<"Invalid Tracker1 IP Address Entered @ "<<ctime(&now);
    }
    selfaddr.sin_port = htons(self_port);
    oth_trakraddr.sin_port = htons(oth_trakr_port);
    a = inet_pton(AF_INET,oth_trakr_ip.c_str(),&oth_trakraddr.sin_addr);
    if(a<=0){
        cout<<"Invalid Tracker2 IP Address\n";
        time_t now = system_clock::to_time_t(system_clock::now());
        log_file<<"Invalid Tracker2 IP Address Entered @ "<<ctime(&now);
    }
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        cout<<"Error Creating Socket!\n";
        time_t now = system_clock::to_time_t(system_clock::now());
        log_file<<"Error Creating Socket @ "<<ctime(&now);
    }
    int b = bind(sockfd,(struct sockaddr*)&selfaddr,sizeof(selfaddr));
    if(b<0){
        cout<<"Bind Failed!\n";
        time_t now = system_clock::to_time_t(system_clock::now());
        log_file<<"Bind Failed @ "<<ctime(&now);
    }
    int l = listen(sockfd,100);
    if(l<0){
        cout<<"Listen Failed!\n";
        time_t now = system_clock::to_time_t(system_clock::now());
        log_file<<"Listen Failed @ "<<ctime(&now);
    }
    pid_t pid = fork();
    if(pid==0){
        //Communicate with Other Tracker
        cout<<"\nWill implement to communicate with other tracker";
        //exit(0);
    }
    else{
        while(1){
            cout<<"\nAccepting Clients connections\n";
            cout<<inet_ntoa(selfaddr.sin_addr)<<":"<<htons(selfaddr.sin_port)<<endl;
            pthread_t tid;
            struct sockaddr_in client_addr;
            socklen_t csize = sizeof(struct sockaddr_in);
            int new_sockfd = accept(sockfd,(struct sockaddr*)&client_addr,&csize);
            cout<<"\nCommunicating with Client: "<<inet_ntoa(client_addr.sin_addr)<<":"<<htons(client_addr.sin_port);
            int *cli_sock = new int;
            *cli_sock = new_sockfd;
            pthread_create(&tid,NULL,detach_thread,cli_sock);
        }
    }
    close(sockfd);
}