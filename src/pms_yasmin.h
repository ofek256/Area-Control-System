/*
  PM2.5 Demo
  pm25-demo.ino
  Demonstrates operation of PM2.5 Particulate Matter Sensor
  ESP32 Serial Port (RX = 16, TX = 17)
  Derived from howtoelectronics.com - https://how2electronics.com/interfacing-pms5003-air-quality-sensor-arduino/

  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/


/*
a buffer is a temperery storing place for recieved data. sometimes we cant process all the data being 
recieved at once, so the buffer is a hardware place in uart to store it while recieving and processing.
A stream is an abstraction of a sequence of bytes that can be read from or written to
By using a pointer to a stream object,
the code can access the methods and properties of the stream object
to perform read and write operations on the UART buffer.
the pointer is not pointing at the buffer directly.
*/
#include <utils.h>


// Serial Port connections for PM2.5 Sensor
#define RXD2 16 // (pin RX2 on the esp) To sensor TXD
#define TXD2 17 // (pin TX2 on the esp) To sensor RXD
//vcc to vin, gnd to gnd.


void setup() {
  Serial.begin(115200);

  // Set up UART connection
  // Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  //SERIAL_8N1      should work as it is a common serial protocol 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

struct pms5003data
{
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
//struct pms5003data data; מחקתי את זה אחרי שקראתי על יצירה של כאלה
//for some reason, deliting thee word struct colored the line white, but it works now. 
//need to check if works with struct
pms5003data data;

/*
boolean readPMSdata(Stream *s) מחקתי כי זה לא אהב את ה- boolean
לא מצאתי עוד מקום בקוד בו יש את המילה stream
בגוגל מצאתי את ההסבר הזה:
https://reference.arduino.cc/reference/en/language/functions/communication/stream/
לא יודעת אם להחליף ל- serial.,read או ל- strem.read
*/
//A C++ stream is a flow of data into or out of a program
boolean readPMSdata(Stream *s) 
{
  /*
  available() checks how many bytes are available to be read in the stream (s pointer, the -> replaces
  the *). if there are no lines to read the condition will be true, thus returning false to the loop
  cause we have nothing to read, the communication failed.
  */
  if (! s->available()) 
  {
    return false;
  }

  /*Read a byte at a time until we get to the special '0x42' start-byte
  a start byte is the byte emplying the begginning of a new data line. 
  In Uart communication, one can create a special specific header, to avoid confusion with 
  other devices connected to the same reciever. our header is 0X42, which is hex for 66, or
  01000010 in binary.
  s is a pointer that points to the stream.
  
  
 every time we send data, there is a different data in the stream, which is 32 bytes.



  ok so i understood it: after cheking if we have any bytes to read in the buffer, and
  discovering we do, we wanna check if the next byte is the start byte, which is 0X42, before we read
  all the information. 
  peek() allowes us to take a peek at the next byte without pulling it 
  out of the buffer, because when we read a byte its thrown away. 
  if the first byte isnt the start byte, there has been an error in transmition. 
      in that case, we read the next byte just to get it out of the buffer, we dont need it
      and then we return false because there has been an error.
  if its the first byte, all is good and we can move on to reading all the data.
  */
  if (s->peek() != 0x42)
  {
    s->read();
    return false;
  }

  // Now read all 32 bytes
  /*
  available() check how many bytes are there to be read in the buffer.
  the reciever expects at least 32 bytes in total. if we get less than that, 
  that means an error has occured during transmition, so we return false.
  */
  if (s->available() < 32) 
  {
    return false;
  }

  //until now, we checked if the transmittion worked.
  //now we begin processing the information, knowing the transmition worked.


  /*
  we create an arrey of unsigned 8 bits ints named buffer with 32 spots, since we have at least 32 bytes
  in this arrey we will temporarely store the data.
  */
  uint8_t buffer[32];
  uint16_t sum = 0;

  /*
  the next line is the reading part. we read 32 bytes from the s stream and store them in the buffer arrey
  */
  s->readBytes(buffer, 32);

  // get checksum ready
  /*
  i guess the last two bytes are stop bytes.
  we summerize the first 30 bytes of the arrey. i is an uint_8 because it cant be higher than the uint_8 
  in the arrey or its length, so we save memory.
  */
 //need to check if it is 30 bytes or actually 28
  for (uint8_t i = 0; i < 30; i++)
  {
    sum += buffer[i];
  }


  // The data comes in endian'd, this solves it so it works on all platforms

  /*
  the first 2 bytes are headers, the third is frame length (instead of using stop byes,
  we tell the reciever in edvence how many bytes it should read). 
  we start inputing the bytes with the second byte. 


  note! check if the headers are also put in the buffer array!!!!!!!!!!!!!


  the actual data is 2 bytes long. (the information from the sensor is 2 bytes long for each measurement)
  now we orgenize only the data we need out of all the stream in an array fitted for 16 bits index.
  each following 2 bytes are a different measurement and shall be put together.
  the higher byte is sent first, and that is why we read the 3rd line before reading the second. 
  by calculating the i in the first row, we can see the first byte being 
  put in the new array is the 3rd in the old one, for the reason i explained earlier.
  
  explanation about the second line will come with the second line
  */
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    /*
      the next line puts in the higher 8 bits, which are located in the next index of the old array.
      before adding it to the 8 lower bits already in the value of this index, we shift these 8 bits
      to the left by 8 bits, making them actually high (their actual length)
      then we add them to the lower byte, thus getting our 16 bits long measurement.
    */
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
  }

  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
} 

void loop() 
{
  //the stream provided is the one in the serial port between sensor and esp
  if (readPMSdata(&Serial2)) 
  {
    // reading data was successful!
    //if successfuly read, the function will return true. with that, we start printing the information
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");
  }
}
/*how can SERIAL_8N1 be undefined
 struct explenation: https://www.w3schools.com/cpp/cpp_structs.asp
 code from: https://dronebotworkshop.com/air-quality/ note that its for 5003 not 7003
 https://www.4project.co.il/arduinoreference/unsigned-int unsigned int fuck my life
https://how2electronics.com/interfacing-pms5003-air-quality-sensor-arduino/ המקור של הקוד
*/