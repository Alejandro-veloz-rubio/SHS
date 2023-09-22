//declaramos la libreria RTC 
#include "RTC.h"    
//delcaramos la libreria MQTT 
#include "MQTT.h"
//declaramos la libreria microSD
#include "MicroSD.h"
//declaramos la libreria de LCD
#include "LCD.h"
//declaramos la libreria de ACTUADORES 
#include "ACTUADORES.h"
//declaramos la libreria para usar JSON
#include <ArduinoJson.h>

//definimos los tiempos en milisegundos para las tareas a realizar 
#define TIEMPO_TAREA_1 1000
#define TIEMPO_TAREA_2 10000
#define TIEMPO_TAREA_3 15000
#define TIEMPO_TAREA_4 60000

//instanciamos cada libreria para usar los metodos de cada una 

DS1307_RTC rtc_13;
MQTT mqtt;
MicroSD MSD;
LCD_P LCD_I2C;
act actuadores;

//

DynamicJsonDocument JSON ( 1024 );

//declaramos una varible de tipo booleano para el sensor PIR

bool motionDetected = false;

//una funcion que nos permitira cambiar el valor de la variable 
//motionDetected cuando se detecte movimiento

void IRAM_ATTR detectsMovement() {
  motionDetected = true;
} 


//declaramos una clase llamada TAREAS

class TAREAS{

  //con acceso publico declaramos variables y metodos 

  public:
  // declaramos variables que nos permitiran realizar operaciones 
  // necesarias para cada tarea y el mqtt
    uint64_t tiempoActual = 0,
		     tiempoTranscurrido_tarea1 = 0,
         tiempoTranscurrido_tarea2 = 0,
         tiempoTranscurrido_tarea3 = 0,
         tiempoTranscurrido_tarea4 = 0,
         lastMsg = 0;


    //declaramos un JSON que almacenara la informacion de cada sensor para mandarlo al servidor MQTT
    String JSON_FINAL = " ";
    //declaramos un JSON que almacenera la informacion de cada sensor para almacenarlo en la Memoria SD
    String JSON_FINAL_SD = " ";
    //declaramos la variable fecha que contendra la fecha del dia correspondiente y servira como nombre del archivo
    //a guardar en la memoria SD
    String fecha = " ";
    //declaramos el tipo de archivo donde se guardara toda la informacion de los sensores
    String formato = ".txt";
    //declaramos un una diagonal para que nos permita almacenar la informacion en la memoria SD
    String diagonal = "/";
    //esta variable nos concatenara diagonal+fecha+formato
    String formato_final = " ";

    //declaramos los metodos en un estado publico
  public:
    //en este metodo, usaremos los metodos inicializadores de mqtt, rtc, sd y lcd
    void var_begin(void);
    //en este metodo, usaremos la funcion millis()
    void new_milis(void);
    //en este metodo usaremos el sensor de gas 
    void tarea1_gas(void);
    //en este metodo usaremos el sensor de fuego
    void tarea2_fuego(void);
    //en este metodo usaremos el sensor PIR
    void tarea3_move(void);
    //en este metodo almacenaremos la informacion recabada por los sensores para 
    //almacenarlos en la micro SD
    void tarea4(void);
    //metodo para obtener la hora 
    void time_get(void);
    //metodo para mostrar hora y fecha en el lcd 
    void hour_lcd(void);


};

void TAREAS :: hour_lcd(){
    //mandamos llamar al metodo de la libreria LCD para mostrar la hora y fecha con los metodos
    //de la libreria RTC
    LCD_I2C.lcd_hora_fecha( rtc_13.show_time() , rtc_13.show_date() );

}

void TAREAS :: time_get(void){

  //mandamos llamar al metodo de la libreria RTC para obtener el tiempo

    rtc_13.get_time();

}

void TAREAS :: var_begin (void){

  //inicializamos el wifi con el metodo de la libreria mqtt
  mqtt.setup_WiFi ( );
  //inicializamos el servidor mqtt para poder hacer uso de el 
  mqtt.set_MQTT_server ( );
  //funcion callback
  mqtt.set_MQTT_callback (  );
  //inicializamos la pantalla LCD para su uso de la libreria LCD
  LCD_I2C.lcd_begin();
  //metodo que nos dara la bienvenida 
  LCD_I2C.lcd_bienvenida();
  //metodo que nos mostrara una carita guiñando
  LCD_I2C.lcd_face();
  //inicializamos el modulo RTC de la libreria RTC
  rtc_13.RTC_init();
  //inicializamos el modulo SD para guardar informacion ahí
  MSD.MicroSD_init( );

}

void TAREAS :: new_milis(void){
    
    //obtenemos el tiempo transcurrido desde que inicio el programa 
    //hasta el tiempo actual con millis()
    tiempoActual = millis ( );

}

void TAREAS :: tarea1_gas(void){

  //si la operacion resultante da como igual el tiempo de la tarea 1 entonces 
  //ejecutaremos el codigo
  if ( tiempoActual - tiempoTranscurrido_tarea1 >= TIEMPO_TAREA_1 ) {

    //actualizamos el tiempo transcurrido al tiempo actual
    
    tiempoTranscurrido_tarea1 = tiempoActual;

    //declaramos el pin donde recibira informacion el esp-32 por parte del sensor

    const int gasPin = 35;

    //declaramos una variable para poder almacenar el valor recibido
    int gasValor = 0;

    //establecemos el pin como entrada de datos
    pinMode(gasPin, INPUT);

   //almacenamos el valor leido atraves del puerto de forma analoga
    gasValor = analogRead(gasPin);

    //si nuesto valor es mayor o igual a 4095 ejecutaremos el programa

    if(gasValor >= 4095){


        //creamos un JSON que nos indique que detecto y la hora y fecha a la que lo detecto

        JSON [ "Descripcion" ] = "Gas detectado";     
        JSON [ "Hora" ] = rtc_13.show_time();                     
        JSON [ "Fecha" ] = rtc_13.show_date();   

        serializeJsonPretty( JSON, JSON_FINAL ); 
        serializeJsonPretty( JSON, JSON_FINAL_SD );               /* Transformar json a string */
        //Serial.println(JSON_FINAL);

        //llamamos al metodo que nos permitira activar un led de color rojo       

        actuadores.act_led();

        //llamamos al metodo que nos permitira mostrar en el LCD lo que se detecto

        LCD_I2C.lcd_gas_d();

        //retraso de 10 milisegundos

        delay(10);

        //si hay una falla de conexion con el servidor mqtt el metodo volvera a conectar al cliente
        mqtt.reconnect_MQTT ( );

        //si la operacion es mayor que 5000 miliseguntos 
        //se ejecutara codigo
        if (tiempoActual - lastMsg > 5000) {

          //actualizamos la variable lastMsg con el tiempo actual
            lastMsg = tiempoActual;

          //publicamos en el topic del servidor la informacion, con estructura JSON,
          //lo que se detecto
            mqtt.publish_MQTT ( JSON_FINAL );

          //al final limpiamos la variable JSON_FINAL para que nos permita seguir mandando 
          //JSON´S
            JSON_FINAL=" ";
        }

      }      
  }

}

void TAREAS :: tarea2_fuego(void){

  //si la operacion resultante da como igual el tiempo de la tarea 2 entonces 
  //ejecutaremos el codigo
  if ( tiempoActual - tiempoTranscurrido_tarea2 >= TIEMPO_TAREA_2 ) {

    //actualizamos el tiempo transcurrido al tiempo actual
    
    tiempoTranscurrido_tarea2 = tiempoActual;

    //declaramos el pin donde recibira informacion el esp-32 por parte del sensor
    const int firePin = 33;

    //declaramos una variable para poder almacenar el valor recibido
    int fireValor = 0;


    //establecemos el pin como entrada de datos
    pinMode(firePin, INPUT);

    //almacenamos el valor leido atraves del puerto de forma analoga
    fireValor = analogRead(firePin);

    //si nuesto valor es menor o igual a 1000 ejecutaremos el programa
    if(fireValor <= 1000 ){


        //creamos un JSON que nos indique que detecto y la hora y fecha a la que lo detecto
        JSON [ "Descripcion" ] = "Fuego detectado";     
        JSON [ "Hora" ] = rtc_13.show_time();                 
        JSON [ "Fecha" ] = rtc_13.show_date(); 



        serializeJsonPretty( JSON, JSON_FINAL ); 
        serializeJsonPretty( JSON, JSON_FINAL_SD ); /* Transformar json a string */
        //Serial.println(JSON_FINAL);  

        //llamamos al metodo que nos permitira activar un buzzer pasivo generando un sonido de alarma        
        actuadores.act_buzz();
      
        //llamamos al metodo que nos permitira mostrar en el LCD lo que se detecto
        LCD_I2C.lcd_fire_d();


        //retraso de 10 milisegundos
        delay(10);

        //si hay una falla de conexion con el servidor mqtt el metodo volvera a conectar al cliente
        mqtt.reconnect_MQTT ( );
        //si la operacion es mayor que 5000 miliseguntos 
        //se ejecutara codigo
        if (tiempoActual - lastMsg > 5000) {

            //actualizamos la variable lastMsg con el tiempo actual
            lastMsg = tiempoActual;

            //publicamos en el topic del servidor la informacion, con estructura JSON,
            //lo que se detecto
            mqtt.publish_MQTT ( JSON_FINAL );

            //al final limpiamos la variable JSON_FINAL para que nos permita seguir mandando 
            //JSON´S
            JSON_FINAL=" ";

        }
    

  }

       
    
  }
}

void TAREAS :: tarea3_move(void){

  //si la operacion resultante da como igual el tiempo de la tarea 3 entonces 
  //ejecutaremos el codigo

  if ( tiempoActual - tiempoTranscurrido_tarea3 >= TIEMPO_TAREA_3 ) {

    //actualizamos el tiempo transcurrido al tiempo actual
    tiempoTranscurrido_tarea3 = tiempoActual;

      //declaramos el pin donde recibira informacion el esp-32 por parte del sensor
      const int motionSensor = 27; 

      //establecemos el pin como entrada de datos y activando las resistencias Pullup
      pinMode(motionSensor, INPUT_PULLUP);


      //este metodo es un interruptor automatico, nos permite obtener los cambios de valores que haya en nuestro sensor PIR
      //tenemos que declarar el pin, llamar a la funcion que cambiara el valor de una variable antes definida,
      //y poner el metodo en modo RISING esto significa que si detecta un cambia de LOW a HIGH en el pin del sensor 
      //llamara a la funcion para cambiar el valor
      attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

      //si la vairiable antes declarada es igual a true ejecutamos codigo

      if(motionDetected==true){

        //creamos un JSON que nos indique que detecto y la hora y fecha a la que lo detecto
        JSON [ "Descripcion" ] = "Movimiento detectado";     
        JSON [ "Hora" ] = rtc_13.show_time();                   
        JSON [ "Fecha" ] = rtc_13.show_date();    
        

        serializeJsonPretty( JSON, JSON_FINAL );
        serializeJsonPretty( JSON, JSON_FINAL_SD );                /* Transformar json a string */
        //Serial.println(JSON_FINAL); 


        //llamamos al metodo que nos permitira mostrar en el LCD lo que se detecto
        LCD_I2C.lcd_move_d();

        //regresamos el valor de la variable a su estado original
        motionDetected = false;

        //retraso de 10 milisegundos
        delay(10);
        
        //si hay una falla de conexion con el servidor mqtt el metodo volvera a conectar al cliente
        mqtt.reconnect_MQTT ( );

        //si la operacion es mayor que 5000 miliseguntos 
        //se ejecutara codigo
        if (tiempoActual - lastMsg > 5000) {
          
            //actualizamos la variable lastMsg con el tiempo actual
            lastMsg = tiempoActual;

            //publicamos en el topic del servidor la informacion, con estructura JSON,
            //lo que se detecto
            mqtt.publish_MQTT ( JSON_FINAL );

            //al final limpiamos la variable JSON_FINAL para que nos permita seguir mandando 
            //JSON´S
            JSON_FINAL=" ";
        }
    }
      
  }

}

void TAREAS :: tarea4(void){

  //si la operacion resultante da como igual el tiempo de la tarea 4 entonces 
  //ejecutaremos el codigo

  if ( tiempoActual - tiempoTranscurrido_tarea4 >= TIEMPO_TAREA_4 ) {
    
    
    //actualizamos el tiempo transcurrido al tiempo actual
    tiempoTranscurrido_tarea4 = tiempoActual;

    //Serial.println(JSON_FINAL_SD);

    //obtenemos la fecha por medio del metodo de la libreria RTC
    //para alamcenarla en laa variable fecha

    fecha = rtc_13.show_date();
    
    //como formato final concatenamos las tres variables para poner nombre
    //y crear un archivo .txt en la micro SD
    formato_final = diagonal+fecha+formato;

    //Pasamos como parametros, el nombre y tipo de archivo a grabar en la micro SD, y la informacion 
    //que se recopilo de todos los sensores, al metodo que nos permitira grabar informacion en la memoria SD
    MSD.SaveFile ( formato_final , JSON_FINAL_SD);
    
    
  }

}