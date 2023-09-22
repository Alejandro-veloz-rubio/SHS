//incluimos la libreria tareas 
#include "TAREAS.h"

//instanciamos la libreria 
TAREAS sensor;

void setup () {
  //iniciamos comunicacion serial en 115200 baudios
  Serial.begin(115200);
  //mandamos llamar al metodo que contendra los iniciadores necesarios de diversos modulos
  sensor.var_begin();
}

void loop () {

  //metodo que obtiene los segundos que han paso desde que inicio el programa 
  sensor.new_milis();
  //sensor que obtiene el tiempo
  sensor.time_get();
  //mandamos llamar a las tareas 
  sensor.tarea1_gas();
  sensor.tarea2_fuego();
  sensor.tarea3_move();
  sensor.tarea4();
  //mandamos llamar a la pantalla lcd para que muestre la hora y fecha 
  sensor.hour_lcd();

}
