//incluimos la libreria para hacer suo del modulo LCD asi como de sus metodos 
#include <LiquidCrystal_I2C.h>

//establecemos la direccion del modulo LCD 
LiquidCrystal_I2C lcd ( 0x27, 16, 2 );

//declaramos la varible para crear un ojo izquierdo

byte left_eye[] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000
};

//declaramos la varible para crear un ojo derecho

byte right_eye[] = {
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000
};

//declaramos la varible para crear un ojo guiñando

byte close_eye[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B00000
};

//declaramos la varible para crear una parte de la sonrisa

byte smile_1[] = {
  B00000,
  B01100,
  B01100,
  B01100,
  B01100,
  B01111,
  B00111,
  B00000
};

//declaramos la varible para crear una parte de la sonrisa

byte smile_2[] = {
  B00000,
  B00110,
  B00110,
  B00110,
  B00110,
  B11110,
  B11100,
  B00000
};



//creamos una clase que albergara de manera publica todos los metodos correspondientes 
class LCD_P{
  public:
    
    //metodo para inicializar el modulo lcd
    void lcd_begin(void);
    //metodo para dar la bienvenida
    void lcd_bienvenida(void);
    //metodo para mostrar una carita
    void lcd_face(void);
    //metodo para mostrar la hora y fecha en el lcd
    void lcd_hora_fecha( String hora, String fecha);
    //metodo para mostrar la lectura de movimiento en el lcd
    void lcd_move_d(void);
    //metodo para mostrar la lectura de fuego en el lcd
    void lcd_fire_d(void);
    //metodo para mostrar la lectura de gas en el lcd
    void lcd_gas_d(void);

};

void LCD_P :: lcd_begin(void){

    lcd.init ( ); 

}

void LCD_P :: lcd_bienvenida(void){

  //prendemos luz de fondo
  lcd.backlight();
  //seleccionamos posicion para mostrar
  lcd.setCursor(0, 0);
  //mostramos mensaje
  lcd.print("Welcome mr Esli!");
  //seleccionamos posicion para mostrar
  lcd.setCursor(0, 1);
  //mostramos mensaje
  lcd.print("Welcome mr Alex!");
  //retraso de 5 segundos
  delay(5000);
  //limpiamos pantalla lcd
  lcd.clear();

  //seleccionamos posicion para mostrar
  lcd.setCursor(0, 0);
  //mostramos mensaje
  lcd.print("  And Welcome");
  //seleccionamos posicion para mostrar
  lcd.setCursor(0, 1);
  //mostramos mensaje
  lcd.print("   Everyone!");
  //retraso de 3.5 segundos
  delay(3500);
  //limpiamos pantalla lcd
  lcd.clear();

}

void LCD_P :: lcd_face(void){

  //creamos un char con las variables antes declaradas, 
  //que nos permitiran mostrar una carita
  lcd.createChar ( 0, left_eye );
  lcd.createChar ( 1, right_eye );
  lcd.createChar ( 2, smile_1);
  lcd.createChar ( 3, smile_2);
  lcd.createChar ( 4, close_eye);

  //CARA FELIZ

  //prendemos luz de fondo
  lcd.backlight();
  //seleccionamos posicion para mostrar

  lcd.setCursor(0,0);
  //imprimimos espacios para mostrar la carita en medio del LCD
  lcd.print("       ");
  //imprimimos los caracteres
  lcd.write( 0 );
  lcd.write( 1 );
  //seleccionamos posicion para mostrar

  lcd.setCursor(0, 1);
  //imprimimos espacios para mostrar la carita en medio del LCD
  lcd.print("       ");
  //imprimimos los caracteres
  lcd.write( 2 );
  lcd.write( 3 );
  //retraso
  delay(850);
  //limpiamos pantalla lcd
  lcd.clear();

  /*SE REPITE EL MISMO PROCESO*/

  //CARA GUIÑANDO
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("       ");
  lcd.write( 4 );
  lcd.write( 1 );

  lcd.setCursor(0, 1);
  lcd.print("       ");
  lcd.write( 2 );
  lcd.write( 3 );

  delay(500);
  lcd.clear();
  
  //CARA FELIZ
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("       ");
  lcd.write( 0 );
  lcd.write( 1 );

  lcd.setCursor(0, 1);
  lcd.print("       ");
  lcd.write( 2 );
  lcd.write( 3 );

  delay(1200);
  lcd.clear();


}


void LCD_P :: lcd_hora_fecha(String hora, String fecha){

  //activamos la luz de fondo del lcd
  lcd.backlight ( );
  //posicionamos donde mostramos la informacion 
  lcd.setCursor ( 0, 0 ); 
  // mostramos la hora con los parametros recibidos
  lcd.print("Hour: ");
  lcd.print(hora);
  //posicionamos donde mostramos la informacion 
  lcd.setCursor ( 0, 1 ); 
  // mostramos la fecha con los parametros recibidos
  lcd.print("Date: ");
  lcd.print(fecha);
  //retraso de 1 segundo
  delay ( 1000 );	
  //limpiamos la pantalla lcd
  lcd.clear();

}


void LCD_P :: lcd_move_d(void){

   //activamos la luz de fondo del lcd
   lcd.backlight ( ); 
  //posicionamos donde mostramos la informacion 
   lcd.setCursor ( 0, 0 ); 
   //imprimimos en el lcd que se detecto movimiento
   lcd.print("MOVE DETECTED !!");
   //retraso de 2 segundos
   delay(2000);
   //limpiamos pantalla lcd
   lcd.clear();

}

void LCD_P :: lcd_fire_d(void){

  //activamos la luz de fondo del lcd
  lcd.backlight ( ); 
  //posicionamos donde mostramos la informacion 
  lcd.setCursor ( 0, 0 ); 
  //imprimimos en el lcd que se detecto fuego
  lcd.print("FIRE DETECTED!!");
  //retraso de 2 segundos 
  delay(2000);
  //limpiamos pantalla lcd
  lcd.clear();


}

void LCD_P :: lcd_gas_d(void){

  //activamos la luz de fondo del lcd
  lcd.backlight ( ); 
  //posicionamos donde mostramos la informacion 
  lcd.setCursor ( 0, 0 ); 
  //imprimimos en el lcd que se detecto fuego
  lcd.print("GAS DETECTED !!");
  //retraso de 2 segundos 
  delay(2000);
  //limpiamos pantalla lcd
  lcd.clear();

}