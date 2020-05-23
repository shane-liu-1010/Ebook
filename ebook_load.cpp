#include<string>
#include<iostream>
#include<fstream>

#include"ebook.h"

using namespace std;

string LoadFileContent(string path){
    ifstream fin(path);
    string buf,res;
    while(getline(fin,buf))res+=buf+"\n";
    return res;
}

#define CHECK(X)if(!(X)){cerr<<__FILE__<<":"<<__LINE__<<" Read data error!"<<endl;return false;}

bool LoadPage(ifstream &fin,EBook &book,int pid){
    string buf;
    Page page;
    bool appendGoto=true;
    page.cmd['s']="Search";
    page.cmd['q']="Qsave";
    page.cmd['w']="Qload";
    page.cmd['e']="exit";
    page.cmd['r']="RandomPage";
    while(true){
        CHECK(getline(fin,buf));
        if(buf=="")continue;
        stringstream ss(buf);
        ss>>buf;
        if(buf=="end")break;
        if(buf=="markend")appendGoto = false;
        else if(buf=="image"){
            string ImgPath;
            ss>>ImgPath;
            page.image=LoadFileContent(("img" DIRSPILTTER)+ImgPath);
        }
        else if(buf=="text-begin"){
            int len=0;
            memset(page.text,0,sizeof(page.text));
            while(true){
                string Text;
                getline(fin,Text);
                if(Text=="text-end")break;
                len+=Text.length()+1;
                if(len>=MAX_LEN - 1){
                    cerr<<"Too much content... ignored."<<endl;
                    break;
                }
                strcat(page.text,(Text + "\n").c_str());
            }
        }
        else if(buf=="goto"){
            appendGoto=false;
            string c;
            int pid;
            ss>>c>>pid;

            if(c.size()==1){
                page.cmd[c[0]]="goto "+to_string(pid);
            }
            else{
                cerr<<"Unknown Key : "<<c<<endl;
            }
        }
        else{
            cerr<<"Unknown Command : "<<buf<<endl;
            cerr<<"-- ignore..."<<buf<<endl;
        }
    }

    if(appendGoto){
        if(page.cmd['d'].empty() && pid+1<book.MaxResourceId)
            page.cmd['d']="goto " +to_string(pid+1);
    }
    book.page[pid]=page;
    return true;
}

bool LoadBook(const char *Path,EBook &book){
    ifstream fin(Path);
    if(!fin){
        cerr<<"Can not open : "<<Path<<endl;
        return false;
    }

    CHECK(fin>>book.MaxResourceId);
    book.page.resize(book.MaxResourceId);

    int page;
    while(fin>>page){
        fin.get();
        CHECK(LoadPage(fin, book, page))
    }

    if(!fin.eof()){
        cerr<<"Read data error..."<<endl;
    }

    return true;
}

#undef CHECK
