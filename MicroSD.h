#ifndef MicroSD_h
#define MicroSD_h
/* Librerías fabricante necesarias para utilizar la MicroSD */
#include <SD.h>
#include <SPI.h>

#define MICROSD_PIN 5    /* Pin de control MicroSD */  
File MicroSD_File;       /* Archivo para escribir  o leer en la MicroSD */
class MicroSD  {
  // VARIABLES
  public:
    uint8_t contador = 0;      /* */
  // MÉTODOS
  public:
   void MicroSD_init ( void );
   void SaveFile ( String filename, String JSON_FINAL );
};

void MicroSD::MicroSD_init ( void ){

   while ( !SD.begin ( MICROSD_PIN ) ) {
        Serial.println ( F ( "Falló la inicialización del módulo MicroSD"  ));
        delay(1000);
    }
    Serial.println ( F ( "La librería MicroSD ha inicializado con éxito" ) );
}

void MicroSD :: SaveFile ( String filename, String JSON_FINAL ) {
    
    // Abrir o crear en caso de que no exista.
    MicroSD_File = SD.open ( filename, FILE_APPEND );
    if ( MicroSD_File ) {

        // Almacenar JSON en el archivo.
        //serializeJson(*doc, JSON);
        MicroSD_File.println ( JSON_FINAL );
        MicroSD_File.close ( );
      
    } else {
        // if the file didn't open, print an error:
        Serial.print ( F ( "Error opening " ) );
        Serial.println ( filename );
 
       
    }
    Serial.println ( "Verificando si existe el archivo: ");
    if ( SD.exists ( filename ) ) {
    Serial.println ( filename );
    Serial.println( " existe" );
  } else {
        Serial.println ( filename );
    Serial.println( " no existe" );
  }
     // Esperar un segundo entre lecturas
      delay ( 1000 );
}


#endif