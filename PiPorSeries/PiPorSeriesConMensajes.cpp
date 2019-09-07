/*
  Calcula el numero PI utilizando una serie "infinita"
  Debe recibir la cantidad de iteraciones como parametro
        __
       \
  pi =  >  (-1)^i x 4/(2xi + 1)
       /__
       i=0
*/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "../Buzon/Mailbox.h"

key_t key = 0xB23753;

/*
   Realiza la acumulacion de terminos desde una posicion inicial hasta un termino final
*/
double calcularSumaParcialPi( Mailbox buzon, int proceso, long inicial, long terminos ) {
   float casiPi = 0;
   double alterna = 4;
   long divisor = 0;
   msgbuf mensaje;

   // 2019 es el tipo de mensaje elegido elegido para el calculo de PiPorSeries

   for ( long termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		// 2 x termino + 1
      casiPi += alterna/divisor;		// 4 / (2xi + 1)
      alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos

   }

   buzon.send(2019, casiPi);

   exit( 0 );

}


int main( int argc, char ** argv ) {
   // Definimos el buzon que recibira todos los mensajes de los procesos hijos
   Mailbox buzonReceptor = Mailbox(0,key);
   long terminos, inicio, fin;
   int proceso;
   int pid;
   float resultado;

   terminos = 1000000;
   if ( argc > 1 ) {
      terminos = atol( argv[ 1 ] );
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
      if ( ! pid ) {
         calcularSumaParcialPi( buzonReceptor, proceso, inicio, fin );
      } else {
         printf("Creando el proceso %d: termino inicial %ld, termino final %ld\n", pid, inicio, fin );
      }
   }

   resultado = 0.0;
   for ( proceso = 0; proceso < 10; proceso++ ) {
      float piParcial;
      // Recibimos el tipo de mensaje 2019
      piParcial = buzonReceptor.receive(2019);
      printf( "Resultado parcial recibido %15.10g \n", piParcial );
      resultado += piParcial;
   }

   printf( "Valor calculado de Pi es \033[91m %15.10g \033[0m con %ld terminos\n", resultado, terminos );

}
