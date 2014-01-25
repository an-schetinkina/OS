#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <process.h>
#include <windows.h>
using namespace std;

typedef struct telb telb;
typedef struct telf telf;

struct telb {
    string block;
    int size;
    telb *next_block;
};

struct telf {
    string fname;
    telb *first_block;
    telb *last_block;
    telf *next_file;
	bool interested[2];
	int turn;
};

typedef telb *PTRb;
typedef telf *PTRf;

PTRf fsystem=NULL;
PTRf current=NULL;
int size_of_block = 16;
 
void enterRegion(int threadId, PTRf file)
{
    int other = 1 - threadId;                    // Идентификатор второго потока
    file->interested[threadId] = true;                 // Индикатор интереса текущего потока
    file->turn = other;                                // Флаг очереди исполнения
    while (file->turn == other && file->interested[other]);
}
 
void leaveRegion(int threadId, PTRf file)
{
    file->interested[threadId] = false;
}

void readfile (string filename)
{
    PTRf top=fsystem;
    while (top!=NULL && filename.compare(top->fname)!=0){
        top=top->next_file;
    }

    if(top==NULL) {
        cout << "no such file in file system\n";
    } 
    else {
        PTRb b = top->first_block;
        while (b!=NULL){
            cout << b->block;
            b=b->next_block;
        }
        cout << "\n";
    }
return;
}

void touchfile (string filename)
{
    PTRf top=fsystem;
    while (top!=NULL && filename.compare(top->fname)!=0){
        top=top->next_file;
    }

    current = top;

    if(top==NULL) {
        PTRb newb= new telb;
        newb->block= "";
        newb->size=0;
        newb->next_block=NULL;

        PTRf newf= new telf;
        newf->fname = filename;
        newf->first_block=newb;
        newf->last_block=newb;
        newf->next_file=fsystem;
        fsystem=newf;
        current = newf;

        cout << "file have been created\n";

    } else {
        cout << "file exists\n";
    }

return;
};

void writefile (string filename)
{
    touchfile(filename);
    PTRb top = current->last_block;
    string l = "";
    
    cout << "to stop writing press command \"stop\"\n";

    while(cin >> l && l != "stop") {
		l += "\n";
		int len = l.length();
		int lenb;
		int ost=len;
        lenb = size_of_block - top->size;
		while (ost != 0) {
			if(ost <= lenb){        
				top->block.append(l,len-ost,ost); 
				top->size += ost;
				ost = 0;
			} 
			else {
				(top->block).append(l,len-ost,lenb); 
				top->size = size_of_block;

				PTRb newb= new telb;
				newb->block="";
				newb->size=0;
				newb->next_block=NULL;
				ost -= lenb;

				//(newb->block).append(l,lenb,16); 
				top->next_block = newb;
				top=newb;
				current->last_block = newb;
			}

		}

    }

std::cout << "text have been added\n";

return;
}

void removefile (string filename)
{
    PTRf top=fsystem;
    while (top!=NULL && filename.compare(top->fname)!=0){
        top=top->next_file;
    }

    if(top==NULL) {
        cout << "no such file in file system\n";
    } 
    else {
        //free blocks
        PTRb b1 = top->first_block;
        PTRb b2;
        while (b1!=NULL){
            b2=b1->next_block;
            delete b1;
            b1=b2;
        }
        //rem from fs

		if (top->next_file != NULL ) {
			PTRf f1 = top->next_file;
			top->fname=top->next_file->fname;
			top->first_block=top->next_file->first_block;
			top->last_block=top->next_file->last_block;
			top->next_file=top->next_file->next_file;
			delete f1;
		} else {
			//delete top;
			fsystem = NULL;
		}
        cout << "file have been removed from file system\n";      
    }

return;
}

void sizefiles (void* arg) {
	int threadId = *(int*)arg;
	PTRf top=fsystem;
	int fsize =0;
    while (top!=NULL){
		enterRegion(threadId, top);
		fsize =0;
		PTRb b = top->first_block;
        while (b!=NULL){
            fsize+=b->size;
            b=b->next_block;

        }
		cout << top->fname << " ";
		cout << fsize << endl;
		leaveRegion(threadId, top);
        top=top->next_file;
    }
}

void test ()
{
	PTRf top = fsystem;
	while (top!=NULL){
		top->interested[0]=false;
		top->interested[1]=false;
		top->turn=0;
        top=top->next_file;
    }

	HANDLE thread0 = (HANDLE)_beginthread(sizefiles,0,new int(0));
	HANDLE thread1 = (HANDLE)_beginthread(sizefiles,0,new int(1));

	WaitForSingleObject(thread0,INFINITE);
	WaitForSingleObject(thread1,INFINITE);
	   //pthread_create();

}

int main (void)
{   

    string com;
    string filename;
    while(1) {
        cout << ">";
        //scanf("%[a-z]%s", com, filename);
        //scanf("%*c");
		cin >> com ;
		if (com == "test") {
			 test();
			 continue;
		} else if (com == "exit") {
            return 0;
        }

		cin	>> filename;
        if (com.compare("touch")==0) {
            touchfile(filename);
        } else if (com.compare("read")==0) {
            readfile(filename);
        } else if (com.compare("write")==0) {
            writefile(filename);
        } else if (com.compare("remove")==0) {
            removefile(filename);
        } 
    }

cout << "bye!";
return 0;
}











