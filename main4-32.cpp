#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#include "queue.h"

QueueHandle_t xQueue3;

// destroyFreeRTOS()
Serial pc (USBTX, USBRX); // tx, rx
void
thread1 (void*);
void
thread2 (void*);
void
thread3 (void*);

struct Task
{
  int c;
  int t;
  int d;
  int name;
};

Task task1 =
  { 1, 6, 6, 0 };
Task task2 =
  { 2, 6, 6, 1 };
Task task3 =
  { 3, 6, 6, 2 };

Task *t1 = &task1;
Task *t2 = &task2;
Task *t3 = &task3;

void
simularEjec (TickType_t ticks)
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

  xQueue3 = xQueueCreate (5, sizeof(int)); // @suppress("Function cannot be resolved")

  int prioridad = configMAX_PRIORITIES;
  xTaskCreate (thread1, "t0", 256, (void*) t1, prioridad, NULL);
  xTaskCreate (thread2, "t1", 256, (void*) t2, prioridad-1, NULL);
  xTaskCreate (thread3, "t2", 256, (void*) t3, prioridad-3, NULL);


  pc.printf ("Iniciando planificador...\n\r");
  // Starts the tracing.
  vTraceEnable ( TRC_START);
  vTaskStartScheduler ();

  for (;;)
    ;
}

void
thread1 (void *params)
{
  Task *task = (Task*) params;

  int message = 1;
  int instance = 0;

  TickType_t startTime;
  startTime = xTaskGetTickCount ();

  while (1)
    {

      pc.printf ("\n> Tarea [%d] - instancia: %d\n\r", task->name, instance);
      simularEjec (task->c * 1000);

      if ( xQueueSend(xQueue3, (void* ) &message, portMAX_DELAY) == pdTRUE) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("\nMensaje de T1 a T3\n\r");
        }
      else
        {
          pc.printf ("\nNo se pudo enviar el mensaje de T1 a T3\n\r");
        }

      instance++;
      vTaskDelayUntil (&startTime, task->d*1000);
    }
}

void thread2(void *params) {
  Task *task = (Task*) params;

  int message = 1;
  int instance = 0;

  TickType_t startTime;
  startTime = xTaskGetTickCount();

  while (1) {

    pc.printf("\n> Tarea [%d] - instancia: %d\n\r", task->name, instance);
    simularEjec(task->c*1000);

    if ( xQueueSend(xQueue3, (void* ) &message, portMAX_DELAY) == pdTRUE) { // @suppress("Invalid arguments")
      pc.printf("\nMensaje de T2 a T3\n\r");
    } else {
      pc.printf("\nNo se pudo enviar el mensaje de T2 a T3\n\r");
    }
    instance++;
    vTaskDelayUntil(&startTime, task->d*1000);

  }
}

void thread3(void *params) {
  Task *task = (Task*) params;

  int message = 1;
  int instance = 0;

  TickType_t startTime;
  startTime = xTaskGetTickCount();

  while (1) {

    if (xQueueReceive(xQueue3, &message, portMAX_DELAY)) { // @suppress("Invalid arguments")
      pc.printf("\n T3 Recibio un mensaje, ya puede ejecutar.\n\r");

      pc.printf("\n> Tarea [%d] - instancia: %d\n\r", task->name,
          instance);
      simularEjec(task->c*1000);
    }
    instance++;
    vTaskDelayUntil(&startTime, task->d*1000);

  }
}

