
#include <cstring>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <omp.h>
#include <queue>
#include <stdlib.h>
#include "Mailbox.h"
#include <fstream>

using namespace std;



int main(int x , char* argv[])
{
	float Pi=0.0;
	
	//Crea el buzon
	key_t key = 0xB23753;
	Mailbox buzonPadre = Mailbox(key,0);

	//Busca el buzon del padre, esto lo tiene que hacer despues del fork el hijo
	Mailbox buzonHijo = Mailbox(key,1); 

	//Envia un mensaje al buzon con el tipo (1) y dato (0.1111)
	buzonPadre.send(1,0.1111);

	//Tambien el hijo puede mandar mensajes. tipo (1) y dato (0.13445)
	buzonHijo.send(1,0.13445);

	//Recibe dos mensajes del buzon
	float casiPiBuzon;
	casiPiBuzon = buzonPadre.receive(1);
	Pi += casiPiBuzon;
	printf("%f casiPi\n",casiPiBuzon);
	casiPiBuzon = buzonPadre.receive(1);
	Pi += casiPiBuzon;
	printf("%f casiPi\n",casiPiBuzon);
	printf("%f Suma de los casiPi\n",Pi);




	return 0;
}
