/* 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    

    
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(2,3);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 2;

// Address of the other node
const uint16_t other_node = 0;

int update_freq = 5000;

long unsigned last_update = 0;

int line = 0;

int display_counter = 0;

void setup(void)
{
  Serial.begin(57600);
  Serial.println("Weather Display v1.0");
  lcd.begin(16, 2);
  lcd.clear();
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

      // Structure of our payload
struct payload_t
{
  //unsigned long ms;
  //unsigned long counter;
  int charge;
  float pressure;
  float outside_temp;
  float garage_temp;
  float humidity;

};

    payload_t payload;


void loop(void)
{
  // Pump the network regularly
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    
  //  lcd.setCursor(0, 0);
    // If so, grab it and print it out
    RF24NetworkHeader header;
    network.read(header,&payload,sizeof(payload));
   /* lcd.print("Outside: ");
    lcd.print(payload.outside_temp);
    lcd.setCursor(0, 1);
    lcd.print("Garage: ");
    lcd.print(payload.garage_temp);*/

}
update_display();

}

void update_display(void)
 {

  
  // See if we should be updating the display
  
  if(millis() - last_update > update_freq)
 { 
  
   // Toggle which line to write to
   
   if(line == 0)
 {
   line = 1;
 }
 else
 {
   line = 0;
 }
   
   // update the last_update value to right now
   
   last_update = millis();
  
  // set cursor position
  
  lcd.setCursor(0, line);

  // clear line to be written
  
  lcd.print ("                ");
  lcd.setCursor(0, line);

// Write to display

  switch (display_counter){
  
    case 0:
    lcd.print ("Outside: ");
    lcd.print (payload.outside_temp);
    lcd.print (" C");
    display_counter++;
    break;

    case 1:
    lcd.print ("Garage: ");
    lcd.print (payload.garage_temp);
    lcd.print (" C");
    display_counter++;    
    break;
    case 2:
    lcd.print ("Atmos: ");
    lcd.print (payload.pressure);
    lcd.print ("mB");
    display_counter++;
    break;
    case 3:
    lcd.print ("Humidity: ");
    lcd.print (payload.humidity);
    lcd.print ("%");
    display_counter++;   
    break;
    case 4:
    lcd.print ("Battery: ");
    lcd.print (payload.charge);
    lcd.print ("");
    display_counter = 0;       
    break;

    
    
  
 
  }
  //lcd.print("In update");
  //    lcd.setCursor(0, 1);
 //lcd.print(millis());
   
 
 }
  
}


