#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<ctime>
#include"help.h"
#include"ebook.h"

void listAllTxt(){
    #ifdef _WIN32
        system("dir /S /B *.txt");
    #else
        system("find . -name '*.txt'");
    #endif
}

using namespace std;

int current=0;
int qs=0;

int *find_position(const char text[], const char pattern[]){
    string st(text);
    static int index[MAX_LEN];
    int d=0;
    int pl=strlen(pattern);
    for(int i=0;i<st.length();i++){
        if(st.substr(i,pl)==pattern){
            index[d]=i;
            d++;
        }
    }
    index[d]=-1;
    return index;
}

bool RunCmd(EBook &book,int &pid,const char cmd[]){
    if (!strcmp(cmd,"Search")){
        FindString(book,pid);
        return true;
    }
    else if(cmd[0]=='g'){
        current=0;
        for(int i=5;cmd[i]!='\0';i++){
            current*=10;
            current+=(cmd[i]-'0');
        }
        PrintPage(book,current);
        return true;
    }
    else if(!strcmp(cmd,"exit")){
        cout<<"See you again,BYE~"<<endl;
        exit(0);
    }
    else if(!strcmp(cmd,"Qsave")){
        qs=current;
        cout<<"Save done!"<<endl;
        return true;
    }
    else if(!strcmp(cmd,"Qload")){
        current=qs;
        PrintPage(book,current);
        return true;
    }
    else if(!strcmp(cmd,"RandomPage")){
        current=rand()%(book.MaxResourceId);
        PrintPage(book,current);
        return true;
    }
    return false;
}

int main(){

    printf("\nHere is the list of all .txt:\n");
    listAllTxt();
    srand(time(NULL));
    EBook book;
    cout<<"Enter the ebook file name:";
    string bookname;
    cin>>bookname;
    if(!LoadBook(bookname.c_str(),book)){
        return -1;
    }
    Getch();
    cout<<"load book "<<bookname<<"!"<<endl;
    cout<<"Press any key to read the book"<<endl;
    Getch();

    PrintPage(book, current);
    while (true){
        if(!GetCmd(book, current)){
            cerr<<"Something went wrong..."<<endl;
            break;
        }
    }

    return 0;
}
