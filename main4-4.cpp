#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#define STR 1 //Define que STR debe utilizar el programa (1,2,3)

// destroyFreeRTOS()
Serial pc (USBTX, USBRX); // tx, rx
void
thread1 (void*);

struct task
{
  int c;
  int t;
  int d;
  int name;
};

task str1[] =
  {
    { 2, 4, 4 ,0},
    { 1, 5, 5 ,1},
    { 1, 6, 6 ,2} };
task str2[] =
  {
    { 3, 5, 5 ,0},
    { 1, 7, 7 ,1},
    { 2, 10, 10 ,2} };
task str3[] =
  {
    { 2, 5, 5 ,0},
    { 1, 6, 6 ,1},
    { 1, 7, 7 ,2},
    { 2, 10, 10 ,3},
    { 1, 15, 15 ,4} };

task *strs[] =
  { str1, str2, str3 };

int getIndex(){
  if(STR == 1){
      return 3;
  }
  if(STR == 2){
      return 3;
  }
  return 5;
}

int
main ()
{

  // Initializes the trace recorder, but does not start the tracing.
  vTraceEnable ( TRC_INIT);
  task *actual = strs[STR - 1];
  //int nose = actual[0].d;
  //int nose1 = actual[1].d;
  //int nose2 = actual[2].d;
  int prioridad = configMAX_PRIORITIES;
  int cantTareas = getIndex();
  for(int i =0; i < cantTareas; i++){
   pc.printf("Entre al for la vez numero: %d\n\r", i);
   prioridad--;
   task *aux = &(actual[i]);
   xTaskCreate (thread1, "t", 256, (void*) aux, prioridad, NULL);
   }

  // Starts the tracing.
  vTraceEnable ( TRC_START);

  vTaskStartScheduler ();

  for (;;)
    ;
}

void
thread1 (void *params)
{
  DigitalOut led1 (LED1);
  task *datos = (task*) params;
  int x =1;
  TickType_t t = xTaskGetTickCount();
  if(x == 1){
      t = 0;
  }
  while (1)
    {

      led1 = !led1;
      TickType_t t2 = xTaskGetTickCount();
      TickType_t t3 = t2;
      //Multiplicamos por 1100 para que todas las tareas ejecuten un poco de tiempo de mas
      while(t3 < t2+(datos->c*1100)){
          t3 = xTaskGetTickCount();
      }
      pc.printf("Tarea %d [%d, %d, %d]\n\r",datos->name,t,t3,x);
      x++;
      //Si el tick actual es mayor que el tiempo en que la tarea debio vencerse, efectivamente se vencio
      if( (t + datos->t*1000) < xTaskGetTickCount()){
          pc.printf("Tarea %d SE VENCIOOOOO, tendria q haber terminado en %d\n\r",datos->name,(t + datos->t*1000));
      }
      vTaskDelayUntil (&t, datos->t*1000);
    }
}


