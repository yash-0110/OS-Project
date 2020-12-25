#include <bits/stdc++.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
// #include <conio.h>
// #include <windows>

using namespace std;

set<char> DELIM{' ','\r','\t','\n','\a'};

char *builtinstrs[]={(char*)"cd",(char*)"help",(char*)"exit"};

int num_builtins=sizeof(builtinstrs)/sizeof(char*);

bool cd(char** args){
    if(args[1]==NULL){
        cerr<<"arguements expected in the command \"cd\""<<endl;
        return true;
    }
    if(args[2]!=NULL){
        cerr<<"more than expected arguements in the command \"cd\""<<endl;
        return true;
    }

    chdir(args[1]);
    return true;
}

bool help(char** args){
    if(args[1]!=NULL){
        cerr<<"more than expected arguements in the command \"help\""<<endl;
        return true;
    }

    cout<<"Yash Gandhi's Shell:"<<endl;
    cout<<"Type commands and their arguements to run them."<<endl;
    cout<<"These are the builtin commands: "<<endl;
    for(int i=0;i<num_builtins;i++)
        cout<<builtinstrs[i]<<endl;

    return true;
}

bool exit(char** args){
    return false;
}

bool (*builtinfuncs[])(char**)={&cd,&help,&exit};

bool cmnds(char** args){
    pid_t pid;
    int status;

    pid=fork();
    if(pid<0){
        cerr<<"fork() failed"<<endl;
        exit(1);
    }
    else if(pid==0){
        int execstatus=execvp(args[0],args);
        if(execstatus==-1){
            cerr<<"execvp() failed"<<endl;
            exit(1);
        }
    }
    else{
        do{
            waitpid(pid,&status,WUNTRACED);
        }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
    }

    return true;
}

bool execute(char** args){
    if(args[0]==NULL){
        cerr<<"No commands entered!"<<endl;
        return true;
    }

    for(int i=0;i<num_builtins;i++){
        if(strcmp(args[0],builtinstrs[i])==0)
            return (*builtinfuncs[i])(args);
    }

    return cmnds(args);
}

void myShell(){

    int status;
    char path[1000];
    cout<<endl;
    do{
        getcwd(path,sizeof(path));
        cout<<"\033[1;34mMyShell:\033[1;32m";
        cout<<path<<"\033[1;31m>> \033[0m ";

        //READ LINE
        string line;
        getline(cin,line);

        //PARSING THE LINE
        char *args[100];
        int curr=0,i=0;
        string s;

        for(int i=0;i<line.size();i++){
            if(DELIM.count(line[i])){
                // cout<<s<<endl;
                if(s.size()){
                    char* temp=new char(100);
                    for(int j=0;j<s.size();j++)
                        temp[j]=s[j];
                    temp[s.size()]='\0';
                    args[curr++]=temp;
                }
                s.clear();
            }
            else
                s.push_back(line[i]);
        }
        if(s.size()){
            char* temp=new char(100);
            for(int j=0;j<s.size();j++)
                temp[j]=s[j];
            temp[s.size()]='\0';
            args[curr++]=temp;
        }
        args[curr]=NULL;

        //EXECUTING THE COMMAND
        status=execute(args);
    }while(status);

    cout<<endl;
}

int main(int argc, char* argv[]){

    myShell();

    return 0;
}