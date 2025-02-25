// MQ-135 and MQ-136 Gas Sensors for Air Quality Project -- Arduino UNO

//Include the library
#include <MQUnifiedsensor.h>
#include <math.h>

//Definitions
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0                      // Analog input 0 of your arduino
#define type "MQ-135"               // MQ135
#define ADC_Bit_Resolution 10       // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6      // RS / R0 = 3.6 ppm  
#define calibration_button 13       // Pin to calibrate your sensor

// Define constants
int sensorPin = A1;                 // Analog input pin connected to A1UT on the sensor
int digitalPin = 2;                 // Digital input pin connected to DOUT on the sensor
int sensorValue = 0;                // Variable to store the sensor value
const float VCC = 5.0;               // Supply voltage (in volts)
const float RL = 10000.0; // Load resistor value (assumed to be 10kΩ)
float R0 = 0.0; // Baseline resistance (to be calibrated)



//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);


void setup() {

  pinMode(digitalPin, INPUT);       // Set the digital pin as input
  Serial.begin(9600);               // Start serial communication at 9600 baud

  //Set math model to calculate the PPM concentration and the value of constants
  MQ135.setRegressionMethod(1);     //_PPM =  a*ratio^b
  
  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 

  MQ135.init(); 

  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:   MQ135.setRL(10);
  */

  /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
  // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
  // and on clean air (Calibration conditions), setting up R0 value.
  // We recomend executing this routine only on setup in laboratory conditions.
  // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor

  Serial.print("Calibrating please wait.");

  float calcR0 = 0;

  for(int i = 1; i<=10; i ++){
    MQ135.update();                                   // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }

  MQ135.setR0(calcR0/10);

  Serial.println("  done!.");
  
  if(isinf(calcR0)) {
    Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); 
    while(1);
  }

  if(calcR0 == 0){
    Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); 
    while(1);
  }
  
  /*****************************  MQ CAlibration ********************************************/ 
  Serial.println("** Values from MQ-135 ****");
  Serial.println("|    CO   |  Alcohol |   CO2  |  Toluen  |  NH4  |  Aceton  |"); 

  // Calibrate R0 in clean air
  Serial.println("Calibrating R0...");
  delay(3000); // Wait for the sensor to stabilize in clean air
  R0 = calculateR0();
  Serial.print("R0: ");
  Serial.println(R0);

}

void loop() {
  readH2S();
  readOthers();
}


void readOthers(){
  Serial.println("** Values from MQ-135 ****");
  Serial.println("|    CO   |  Alcohol |   CO2  |  Toluen  |  NH4  |  Aceton  |"); 
   MQ135.update(); // Update data, the arduino will read the voltage from the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configure the equation to calculate CO concentration value
  float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); //Configure the equation to calculate Alcohol concentration value
  float Alcohol = MQ135.readSensor(); // SSensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configure the equation to calculate CO2 concentration value
  float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(44.947); MQ135.setB(-3.445); // Configure the equation to calculate Toluen concentration value
  float Toluen = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  
  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configure the equation to calculate NH4 concentration value
  float NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup

  MQ135.setA(34.668); MQ135.setB(-3.369); // Configure the equation to calculate Aceton concentration value
  float Aceton = MQ135.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  Serial.print("|   "); Serial.print(CO); 
  Serial.print("   |   "); Serial.print(Alcohol);
  // Note: 400 Offset for CO2 source: https://github.com/miguel5612/MQSensorsLib/issues/29
  /*
  Motivation:
  We have added 400 PPM because when the library is calibrated it assumes the current state of the
  air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
  https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */
 
  Serial.print("   |   "); Serial.print(CO2 + 400); 
  Serial.print("   |   "); Serial.print(Toluen); 
  Serial.print("   |   "); Serial.print(NH4); 
  Serial.print("   |   "); Serial.print(Aceton);
  Serial.println("   |"); 
  /*
    Exponential regression:
  GAS      | a      | b
  CO       | 605.18 | -3.937  
  Alcohol  | 77.255 | -3.18 
  CO2      | 110.47 | -2.862
  Toluen  | 44.947 | -3.445
  NH4      | 102.2  | -2.473
  Aceton  | 34.668 | -3.369
  */

  delay(1000); //Sampling frequency
}

void readH2S(){
  // Step 1: Read the raw analog value
  int sensorValue = analogRead(sensorPin);

  // Step 2: Convert the raw analog value to voltage
  float voltage = (sensorValue / 1024.0) * VCC;

  // Step 3: Calculate the sensor's resistance (Rs)
  float Rs = calculateRs(voltage);

  // Step 4: Calculate the ratio Rs/R0
  float ratio = Rs / R0;

  // Step 5: Map Rs/R0 to H2S concentration using the sensitivity curve
  // Example formula derived from the datasheet (adjust coefficients as needed):
  float h2sConcentration = pow(ratio, -1.17); // Adjust exponent based on calibration

  // Step 6: Print the results
  Serial.println("** Values from MQ-136 ****");
  Serial.println("|  Raw Value   |  Voltage |   Rs  |  Ohms, H2S Concentration "); 

  Serial.print("   |   ");Serial.print(sensorValue);
  Serial.print("   |   ");Serial.print(voltage);
  Serial.print("   |   ");Serial.print(Rs);
  Serial.print("   |   ");Serial.print(h2sConcentration);Serial.println(" ppm");

  delay(1000); // Wait for 2 seconds before taking the next reading
}


// Function to calculate R0 in clean air
float calculateR0() {
  int sensorValue = analogRead(sensorPin);
  float voltage = (sensorValue / 1024.0) * VCC;
  return (RL / ((VCC / voltage) - 1));
}

// Function to calculate Rs
float calculateRs(float voltage) {
  return (RL / ((VCC / voltage) - 1));
}