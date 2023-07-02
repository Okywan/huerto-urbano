//Sensor de Temperatura A0
//Sensor de luz en A1
//Sensor de humedad en A2
//optoacoplador en D1
//https://www.luisllamas.es/como-emplear-un-esp8266-como-cliente-de-un-api-rest-con-json/

#define TEMPSENSOR A0
#define LIGHTSENSOR A1
#define MOISTSENSOR A2
#define PUMP 2
#define DHTPOWER 9
//

//#include <SPI.h> // para IoT
//#include <Ethernet.h> // para IoT
//#include <EnableInterrupt.h> para Botón de regar
//
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress myIP(192, 168, 1, 177);
//IPAddress myDns(192, 168, 1, 1);
//EthernetClient client;
//IPAddress server(192, 168, 1, 200);


int tTreshold = 30; // limite temperatura
int hTreshold = 45; // limite humedad
int lTreshold = 500; // limite luz
unsigned long pumpTime = 10000; // 10 seg (10 s* 1000ms)
unsigned long sleepTime = 1500000; //Tiempo entre mediciones 25min * 60 seg * 1000 mseg
int luz = 0;
int humedad = 0;
int temp =0;
float tempC =0;
int iFallidos =0;
int maxFallidos=4;
const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ
int V = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(TEMPSENSOR, INPUT);
  pinMode(LIGHTSENSOR, INPUT);
  pinMode(MOISTSENSOR, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(DHTPOWER, OUTPUT);
  digitalWrite(DHTPOWER,LOW);
  digitalWrite(PUMP,LOW);
  //enableInterrupt(5,regar,RISING) para boton de regar
  Serial.begin(9600);

//  Serial.println("Initialize Ethernet with DHCP:");
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // Check for Ethernet hardware present
//    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
//      while (true) {
//        delay(1); // do nothing, no point running without Ethernet hardware
//      }
//    }
//    if (Ethernet.linkStatus() == LinkOFF) {
//      Serial.println("Ethernet cable is not connected.");
//    }
//    // try to congifure using IP address instead of DHCP:
//    Ethernet.begin(mac, myIP, myDns);
//}else {
//    Serial.print("  DHCP assigned IP ");
//    Serial.println(Ethernet.localIP());
//  }
}
void loop()
{
	
	V = analogRead(LIGHTSENSOR); //leemos intensidad de luz
	luz = ((long)(1024-V)*A*10)/((long)B*Rc*V);  //usar si LDR entre GND y A0 
  //luz = ((long)V*A*10)/((long)B*Rc*(1024-V));    //usar si LDR entre A0 y Vcc (como en el esquema anterior)x
	Serial.print("luz: "); 
	Serial.println(luz);
	temp = analogRead(TEMPSENSOR); //leemos valor temperatura
	tempC =(5.0 * temp * 100.0)/1024.0; 
	Serial.print("temp: ");
	Serial.println(tempC);
	humedad = medirHumedad();
	
	if(tempC <= tTreshold && humedad <= hTreshold && luz <= lTreshold ){ //regamos si se dan condiciones idoneas y es necesario
//    if (client.connect(server, 80)) {
  //  Serial.println("connected");
    // Make a HTTP request:
    // Send a sensor name (needs URL encoding) and value (integer or float).
    // This name 'My Arduino Sensor' and value '13' are just examples.
    // Put your sensor name and data here.
   // client.send( F("My%20Arduino%20Sensor"), 13 );
  //}
  //else {
    // kf you didn't get a connection to the server:
    //Serial.println("connection failed");
  //}
//}

		regar();
		
}else{
	if(humedad < hTreshold){
		Serial.print("no se dan condiciones para regar: Intentos: ");
		iFallidos++;
        Serial.println(iFallidos);
	}
    
	Serial.println("No hace falta regar");
}
if (iFallidos>maxFallidos){ // regamos de todas formas si la humedad lleva demasiado tiempo baja
	Serial.println("regamos, hace falta y llevamos muchos fallos.");
	regar();
	iFallidos=0;
}
Serial.print("esperando para nuevo ciclo...");
Serial.println(sleepTime);
delay(sleepTime); // esperamos para la próxima medición.
}

void regar(){
  humedad=medirHumedad();
	//while(humedad<hTreshold){ // empapamos demasiado, mejor regar mas veces cada poco
		Serial.println("regando...");
		digitalWrite(PUMP, HIGH);
		delay(pumpTime);
		digitalWrite(PUMP, LOW);
    humedad=medirHumedad();
	//}
	Serial.println("fin del riego.");
	iFallidos=0;
}

int medirHumedad(){
	digitalWrite(DHTPOWER,HIGH); //encendemos higrometro
	delay(5000);
	humedad = analogRead(MOISTSENSOR);
 	delay(100);
	digitalWrite(DHTPOWER,LOW); // apagamos higrometro
	humedad = map(humedad, 400, 1000, 100, 0); 
	Serial.print("humedad: ");
	Serial.println(humedad);
	return humedad;
}
