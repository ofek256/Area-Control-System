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
bool com;
// Serial Port connections for PM2.5 Sensor
#define RXD2 16 // (pin RX2 on the esp) To sensor TXD
#define TXD2 17 // (pin TX2 on the esp) To sensor RXD
// vcc to vin, gnd to gnd.

const int buzzer = 18;
bool success;
struct pms7003data
{
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

// struct pms5003data data; מחקתי את זה אחרי שקראתי על יצירה של כאלה

/*
  we buils a struct called data with all the members empty, about to be filled.
  the members are in the order of the measurements data bytes!!!!!
*/
pms7003data data;

uint16_t number[12] = {1, 222, 3, 4, 590, 6, 7, 8, 9, 1110, 11, 12};
int higher[12];
uint16_t *pointerArr[12] = {&data.pm10_standard, &data.pm25_standard, &data.pm100_standard,
                            &data.pm10_env, &data.pm25_env, &data.pm100_env, &data.particles_03um,
                            &data.particles_05um, &data.particles_10um, &data.particles_25um,
                            &data.particles_50um, &data.particles_100um};
int all_good;
/*
boolean readPMSdata(Stream *s) מחקתי כי זה לא אהב את ה- boolean
לא מצאתי עוד מקום בקוד בו יש את המילה stream
בגוגל מצאתי את ההסבר הזה:
https://reference.arduino.cc/reference/en/language/functions/communication/stream/
לא יודעת אם להחליף ל- serial.,read או ל- strem.read
*/
// A C++ stream is a flow of data into or out of a program
boolean readPMSdata(Stream *s)
{
  all_good = 0;
  /*
  available() checks how many bytes are available to be read in the stream (s pointer, the -> replaces
  the *). if there are no lines to read the condition will be true, thus returning false to the loop
  cause we have nothing to read, the communication failed.
  */
  if (!s->available())
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

  // until now, we checked if the transmittion worked.
  // now we begin processing the information, knowing the transmition worked.

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

  /*
    the pms data is like so:
    2 first bytes:  headers.         (go into buffer, !!!NOT!!! into buffer_u16)
    2 next bytes: frame length.      (they are the first two bytes in buffer_u16)
    26 next bytes: data.             every two bytes are a measurement. HIGH before LOW
    2 last bytes: check bytes.       (a sum of the highs to compare to. go into buffer_u16)

    32 in total.

    and the arrays:


    buffer array: all 32 bytes seperately.


    buffer_u16:
    index     #1:                        frame length.               (buffer[2] + buffer[3])
    indexes   #2-14 (13 in total):       data.                       (buffer[i] + buffer[i+1])
    index     #15:                       check bytes.                (buffer[30] + buffer[31])

  */

  // get checksum ready
  /*
  the last two bytes are check bytes! adding all the highs to compare with them, so we dont put
  them in the calculation.

  we summerize the first 30 bytes of the arrey. i is an uint_8 because it cant be higher than the uint_8
  in the arrey or its length, so we save memory.
  */

  for (uint8_t i = 0; i < 30; i++)
  {
    // Serial.println("i:    " + String(i) + "   buffer[i]:    " + String(buffer[i]));
    sum += buffer[i];
  }

  // The data comes in endian'd, this solves it so it works on all platforms

  /*
  the first 2 bytes are headers, the third is frame length (instead of using stop byes,
  we tell the reciever in edvence how many bytes it should read).
  we start inputing the bytes with the third byte.


  !!!!!!!!!!   THE HEADERS ARE IN THE BUFFER ARRAY, BUT NOT THE BUFFER_U16   !!!!!!!!!!!!!


  the actual data is 2 bytes long. (the information from the sensor is 2 bytes long for each measurement)
  now we orgenize only the data we need out of all the stream in an array fitted for 16 bits index.
  each following 2 bytes are a different measurement and shall be put together.
  the higher byte is sent first, and that is why we read the 3rd line before reading the second.
  by calculating the i in the first row, we can see the first byte being
  put in the new array is the 3rd in the old one, for the reason i explained earlier.

  explanation about the second line will come with the second line
  */
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++)
  {
    buffer_u16[i] = buffer[2 + i * 2 + 1];
    /*
    Serial.println("i:  " + String(i));
    Serial.println("2+i*2+1  (buffer's index):  " + String(2+i*2+1));
    Serial.println("buffer_u16[low]:   " + String(buffer_u16[i]));
    if (i==14)
    {
      Serial.println("buffer[31]=   " + String(buffer[2+i*2+1]));
    }
    */
    /*
      the next line puts in the higher 8 bits, which are located in the next index of the old array.
      before adding it to the 8 lower bits already in the value of this index, we shift these 8 bits
      to the left by 8 bits, making them actually high (their actual length)
      then we add them to the lower byte, thus getting our 16 bits long measurement.
    */
    buffer_u16[i] += (buffer[2 + i * 2] << 8);
    /*
    Serial.println("HIGH:  2+i*2  (buffer's index):  " + String(2+i*2));
    Serial.println("buffer_u16[i]:   " + String(buffer_u16[i]));
    if (i==14)
    {
      Serial.println("buffer[30]=   " + String(buffer[2+i*2]));
    }
    */
  }
  delay(500);

  /*
  memcpy coppies bytes from a one place in the memory to another.
  memcy(pointer to the location to copy TO, pointer to the location to copy FROM, number of bytes)
  we put the data in the struct we built before!
  we do have a variable for framelength and for check!!!!!!!
  the order of the struct member is the order of the data! frame length first, data second
  (also by its order) and check last!!!
  */
  memcpy((void *)&data, (void *)buffer_u16, 30);

  // now we check if the sum equals the check bytes

  if (sum != data.checksum)
  {
    Serial.println("Checksum failure");
    return false;
  }
  for (int i = 0; i < 12; i++)
  {
    if (*pointerArr[i] >= number[i])
    {
      higher[i] = 1;
      all_good++;
    }
    else
    {
      higher[i] = 0;
    }
  }

  return true;
}
int alarm_on;
void pms_func()
{
  Serial.println("voidloopin");
  success = (readPMSdata(&Serial2));
  // the stream provided is the one in the serial port between sensor and esp
  if (success)
  {
    // reading data was successful!
    // if successfuly read, the function will return true. with that, we start printing the information
    Serial.println();
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (standard)");
    Serial.print("PM 1.0: ");
    Serial.print(data.pm10_standard);
    Serial.print("\t\tPM 2.5: ");
    Serial.print(data.pm25_standard);
    Serial.print("\t\tPM 10: ");
    Serial.println(data.pm100_standard);
    Serial.println("---------------------------------------");
    Serial.println("Concentration Units (environmental)");
    Serial.print("PM 1.0: ");
    Serial.print(data.pm10_env);
    Serial.print("\t\tPM 2.5: ");
    Serial.print(data.pm25_env);
    Serial.print("\t\tPM 10: ");
    Serial.println(data.pm100_env);
    Serial.println("---------------------------------------");
    Serial.print("Particles > 0.3um / 0.1L air:");
    Serial.println(data.particles_03um);
    Serial.print("Particles > 0.5um / 0.1L air:");
    Serial.println(data.particles_05um);
    Serial.print("Particles > 1.0um / 0.1L air:");
    Serial.println(data.particles_10um);
    Serial.print("Particles > 2.5um / 0.1L air:");
    Serial.println(data.particles_25um);
    Serial.print("Particles > 5.0um / 0.1L air:");
    Serial.println(data.particles_50um);
    Serial.print("Particles > 10.0 um / 0.1L air:");
    Serial.println(data.particles_100um);
    Serial.println("---------------------------------------");

    for (int i = 0; i < 12; i++)
    {
      Serial.print("i=");
      Serial.print(i);
      Serial.print(" higher[i] =");
      Serial.print(higher[i]);
      Serial.print(" measurement= ");
      Serial.print(*pointerArr[i]);
      Serial.print(" numbers[i]=");
      Serial.println(number[i]);
    }
    delay(2000);
    if (all_good != 0)
    {
      tone(buzzer, 900);
      Serial.println("evacuate");
      alarm_on = 1;
      for (int i = 0; i < 12; i++)
      {
        number[i] = 10000;
      }
    }
    if ((all_good == 0) && (alarm_on == 1))
    {
      noTone(buzzer);
      Serial.println("danger is over");
      alarm_on = 0;
    }
    Serial.println();
    com=true;
    delay(10000);
  }
  else
  {
    com = false;
  }
}
/*how can SERIAL_8N1 be undefined
 struct explenation: https://www.w3schools.com/cpp/cpp_structs.asp
 code from: https://dronebotworkshop.com/air-quality/ note that its for 5003 not 7003
 https://www.4project.co.il/arduinoreference/unsigned-int unsigned int fuck my life
https://how2electronics.com/interfacing-pms5003-air-quality-sensor-arduino/ המקור של הקוד
*/