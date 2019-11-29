#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS
#include "task.h"       // Funciones para el control de las tareas
#include "queue.h"

QueueHandle_t xQueue1;
QueueHandle_t xQueue2;
QueueHandle_t xQueue3;
QueueHandle_t xQueue4;
QueueHandle_t xQueue5;

// destroyFreeRTOS()
Serial pc (USBTX, USBRX); // tx, rx
void
thread1 (void*);
void
thread2 (void*);
void
thread3 (void*);
void
thread4 (void*);
void
thread5 (void*);

struct Task
{
  int c;
  int t;
  int d;
  int name;
};

Task task1 =
  { 1, 6, 6, 1 };
Task task2 =
  { 1, 6, 6, 2 };
Task task3 =
  { 1, 6, 6, 3 };
Task task4 =
  { 1, 6, 6, 4 };
Task task5 =
  { 1, 6, 6, 5 };

Task *t1 = &task1;
Task *t2 = &task2;
Task *t3 = &task3;
Task *t4 = &task4;
Task *t5 = &task5;

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

  xQueue1 = xQueueCreate (5, sizeof(int)); // @suppress("Function cannot be resolved")
  xQueue2 = xQueueCreate (5, sizeof(int)); // @suppress("Function cannot be resolved")
  xQueue3 = xQueueCreate (5, sizeof(int)); // @suppress("Function cannot be resolved")
  xQueue4 = xQueueCreate (5, sizeof(int)); // @suppress("Function cannot be resolved")
  xQueue5 = xQueueCreate (5, sizeof(int)); // @suppress("Function cannot be resolved")

  int prioridad = configMAX_PRIORITIES;
  xTaskCreate (thread1, "t1", 256, (void*) t1, prioridad, NULL);
  xTaskCreate (thread2, "t2", 256, (void*) t2, prioridad-2, NULL);
  xTaskCreate (thread3, "t3", 256, (void*) t3, prioridad-1, NULL);
  xTaskCreate (thread4, "t4", 256, (void*) t4, prioridad-3, NULL);
  xTaskCreate (thread5, "t5", 256, (void*) t5, prioridad-4, NULL);

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

      if ( xQueueSend(xQueue2, (void* ) &message, portMAX_DELAY) == pdTRUE) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("\nMensaje de T1 a T2\n\r");
        }
      else
        {
          pc.printf ("\nNo se pudo enviar el mensaje de T1 a T2\n\r");
        }

      vTaskDelayUntil (&startTime, task->d * 1000);
      instance++;
    }
}

void
thread2 (void *params)
{
  Task *task = (Task*) params;

  int message = 1;
  int instance = 0;

  TickType_t startTime;
  startTime = xTaskGetTickCount ();

  while (1)
    {

      if (xQueueReceive (xQueue2, &message, portMAX_DELAY)) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("\n T2 Recibio un mensaje, ya puede ejecutar.\n\r");

          pc.printf ("\n> Tarea [%d] - instancia: %d\n\r", task->name,
                     instance);
          simularEjec (task->c * 1000);

          if ( xQueueSend(xQueue5, (void* ) &message, portMAX_DELAY) == pdTRUE) // @suppress("Invalid arguments")
            { // @suppress("Invalid arguments")
              pc.printf ("\nMensaje de T2 a T5\n\r");
            }
          else
            {
              pc.printf ("\nNo se pudo enviar el mensaje de T2 a T5\n\r");
            }

          if ( xQueueSend(xQueue4, (void* ) &message, portMAX_DELAY) == pdTRUE) // @suppress("Invalid arguments")
            { // @suppress("Invalid arguments")
              pc.printf ("\nMensaje de T2 a T4\n\r");
            }
          else
            {
              pc.printf ("\nNo se pudo enviar el mensaje de T2 a T4\n\r");
            }
        }

      vTaskDelayUntil (&startTime, task->d * 1000);
      instance++;
    }
}

void
thread3 (void *params)
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

      if ( xQueueSend(xQueue5, (void* ) &message, portMAX_DELAY) == pdTRUE) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("\nMensaje de T3 a T5\n\r");
        }
      else
        {
          pc.printf ("\nNo se pudo enviar el mensaje de T3 a T5\n\r");
        }
      vTaskDelayUntil (&startTime, task->d * 1000);
      instance++;
    }
}

void
thread4 (void *params)
{
  Task *task = (Task*) params;

  int message = 1;
  int instance = 0;

  TickType_t startTime;
  startTime = xTaskGetTickCount ();

  while (1)
    {
      if (xQueueReceive (xQueue4, &message, portMAX_DELAY)) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("\n T4 Recibio un mensaje, ya puede ejecutar.\n\r");

          pc.printf ("\n> Tarea [%d] - instancia: %d\n\r", task->name,
                     instance);
          simularEjec (task->c * 1000);
        }

      vTaskDelayUntil (&startTime, task->d * 1000);
      instance++;
    }
}

void
thread5 (void *params)
{
  Task *task = (Task*) params;

  int message = 1;
  int instance = 0;

  TickType_t startTime;
  startTime = xTaskGetTickCount ();

  while (1)
    {
      if (xQueueReceive (xQueue5, &message, portMAX_DELAY)) // @suppress("Invalid arguments")
        { // @suppress("Invalid arguments")
          pc.printf ("\n T5 Recibio un mensaje, ya puede ejecutar.\n\r");

          pc.printf ("\n> Tarea [%d] - instancia: %d\n\r", task->name,
                     instance);
          simularEjec (task->c * 1000);
        }

      vTaskDelayUntil (&startTime, task->d * 1000);
      instance++;
    }
}

