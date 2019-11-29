#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#include "semphr.h"
#define STR 1 //Define que STR debe utilizar el programa (1,2,3)

// destroyFreeRTOS()
Serial pc (USBTX, USBRX); // tx, rx
SemaphoreHandle_t xSemaphore = 0;

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
    { 2, 4, 4, 0 },
    { 1, 5, 5, 1 },
    { 1, 6, 6, 2 } };
task str2[] =
  {
    { 3, 5, 5, 0 },
    { 1, 7, 7, 1 },
    { 2, 10, 10, 2 } };
task str3[] =
  {
    { 2, 5, 5, 0 },
    { 1, 6, 6, 1 },
    { 1, 7, 7, 2 },
    { 2, 10, 10, 3 },
    { 1, 15, 15, 4 } };

task *strs[] =
  { str1, str2, str3 };

int
getIndex ()
{
  if (STR == 1)
    {
      return 3;
    }
  if (STR == 2)
    {
      return 3;
    }
  return 5;
}

void
access_shared_resource (TickType_t ticks)
{
  TickType_t current = xTaskGetTickCount ();
  while ((xTaskGetTickCount () - current) < ticks)
    {
    }
  return;
}

int
main ()
{
  // Initializes the trace recorder, but does not start the tracing.
  vTraceEnable ( TRC_INIT);
  xSemaphore = xSemaphoreCreateMutex (); // @suppress("Function cannot be resolved")
  task *actual = strs[STR - 1];
  int prioridad = configMAX_PRIORITIES;
  int cantTareas = getIndex ();
  for (int i = 0; i < cantTareas; i++)
    {
      pc.printf ("Entre al for la vez numero: %d\n\r", i);
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
  int x = 1;
  TickType_t t = xTaskGetTickCount ();
  const TickType_t xFrequency = datos->t * 1000;
  if (x == 1)
    {
      t = 0;
    }
  while (1)
    {

      led1 = !led1;
      pc.printf ("Tarea [%d], instancia [%d]\n\r", datos->name, x);
      if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("La tarea %d accede al recurso\n\r", datos->name);
          access_shared_resource (datos->c * 1000);
          xSemaphoreGive(xSemaphore); // @suppress("Invalid arguments")
          pc.printf ("La tarea %d libera al recurso\n\r", datos->name);
        }
      x++;
      vTaskDelayUntil (&t, xFrequency);
    }
}

