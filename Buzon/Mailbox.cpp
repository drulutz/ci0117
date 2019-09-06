#include "Mailbox.h"
#include <cstring>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

// structure to message a package 
struct mesg_buffer { 
    long mtype; 
    float casiPi;
}; 


//Constructor por llave
Mailbox::Mailbox(key_t key, int tipo)
{
  //Guarda la llave
  this->key = key;
  if(tipo == 0)
  {
    //Crea el buzon  
    this->mailboxId = msgget(key,0666 | IPC_CREAT);
  }
  else
  {
    //Busca el buzon 
    this->mailboxId = msgget(key,0666);
  }
  
}


Mailbox::~Mailbox()
{

   //destroy the message queue 
   msgctl(this->mailboxId, IPC_RMID, NULL); 
}

void Mailbox::send(int mtype, float casiPi)
{
  	mesg_buffer package;
 	package.mtype = mtype;
	package.casiPi = casiPi;

 	if(msgsnd(this->mailboxId, &package, sizeof(package),0) == -1)
 	{
		perror("msgsnd error");
        exit(EXIT_FAILURE);
    }
}

float Mailbox::receive(int mtype)
{
       mesg_buffer recv;  

       if(msgrcv(this->mailboxId, &recv, sizeof(recv),mtype, 0) == -1)
	   {
			if (errno != ENOMSG) 
			{
                 perror("msgrcv");
                 exit(EXIT_FAILURE);
			}
            		
	   }
                                                                                                                            	  
      return recv.casiPi;
}
