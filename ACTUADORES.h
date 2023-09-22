//creamos una clase para los actuadores con los metodos por medio de acceso publico
class act{
  public:
    //este metodo activa un buzzer 
    void act_buzz(void);
    //este metodo activa un led de color rojo
    void act_led(void);
};

void act :: act_buzz(void){
   //declaramos el pin por el cual sonara el buzzer
  const int buzzer = 13;
  //establecemos el pin como salida
  pinMode(buzzer, OUTPUT);
  
  //usamos tone() para hacer sonar el buzzer
  //con una frecuencia de 261 hz
  tone(buzzer,261);
  //retraso de 350 milisegundos
  delay(350);
  //detenemos de hacer ruido
  noTone(buzzer);

}

void act :: act_led(void){
   
  //declaramos el pin de donde se usara el led 
  const int led = 12;
  //lo establecemos como salida 
  pinMode(led, OUTPUT);
  
  //prendemos el led con este metodo y HIGH
  digitalWrite(led, HIGH);
  //lo dejamos prendido por un segundo
  delay(1000);
  //apagamos el led con LOW
  digitalWrite(led, LOW);


}