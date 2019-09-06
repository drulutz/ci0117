#ifndef Mailbox_H
#define Mailbox_H
#include <string>

using namespace std;
class Mailbox
{
    public:
    Mailbox(key_t,int);
    ~Mailbox();
    
    //Send and receive messages
	void send(int, float);
	float receive(int);
	
   private:
	int mailboxId;
	int key;

		
};



#endif // Mailbox_H
