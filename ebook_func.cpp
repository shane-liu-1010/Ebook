#include <string>
#include <iostream>
#include <fstream>

#include "ebook.h"
void CleanScreen(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

using namespace std;

int text_line=0;
int cmd_line=0;

void CleanText(Page &page){
    for (int i=0;i<cmd_line+text_line;i++){
        cout<<PLINE;
        for (int i=0;i<100;i++)cout<<'\b';
        for (int i=0;i<100;i++)cout<<' ';
        for (int i=0;i<100;i++)cout<<'\b';
    }
    text_line = cmd_line = 0;
}

void CleanCmd(Page &page){
    for (int i=0;i<cmd_line;i++){
        cout<<PLINE;
        for (int i=0;i<100;i++)cout<<'\b';
        for (int i=0;i<100;i++)cout<<' ';
        for (int i=0;i<100;i++)cout<<'\b';
    }
    cmd_line=0;
}

void PrintCmd(Page &page){
    cmd_line=1;
    cout<<"Please enter command: "<<endl;
    for (int i=0;i<256;i++){
        if (page.cmd[i]!=""){
            cout<<"["<<char(i)<<"]: "<<page.cmd[i]<<endl;
            cmd_line++;
        }
    }
}

void PrintText(Page &page,int pid,bool animation,int *indices,int p_len){
    int len=strlen(page.text);
    int cur=0;
    text_line=0;
    for (int i=0,char_count=0;i<len;i++){
        if (animation)
            SleepMs(50);
        if (page.text[i]=='\n'||(page.text[i]==' '&&char_count>=60)){
            cout<<endl;
            text_line++;
            char_count = 0;
        }
        else{
            if (p_len && i==indices[cur])cout<<GREEN;
            cout<<page.text[i]<<flush;
            if (p_len && i==indices[cur]+p_len-1){
                cout<<RESET;
                cur++;
            }
            char_count++;
        }
    }
    cout<<string(50,'=')<<endl;
    cout<<"page "<<pid<<"."<<endl<<endl;
    text_line+=3;
    PrintCmd(page);
}

void PrintPage(EBook &book, int pid){
    CleanScreen();
    cout<<book.page[pid].image<<endl;
    cout<<string(50,'=')<<endl;
    PrintText(book.page[pid], pid);
}

void FindString(EBook &book, int& pid){
    CleanCmd(book.page[pid]);
    cout<<"Please enter the string to look for: "<<flush;
    ++cmd_line;
    string pattern;
    cin>>pattern;
    CleanScreen();
    int *indices=find_position(book.page[pid].text, pattern.c_str());
    PrintText(book.page[pid],pid,false,indices, pattern.length());
}

int GetCmd(EBook &book, int &pid){
    while(int c=Getch()){
        if(book.page[pid].cmd[c]!=""){
            return RunCmd(book,pid,book.page[pid].cmd[c].c_str());
        }
    }
    return -1;
}
