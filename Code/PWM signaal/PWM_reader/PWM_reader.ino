const int PWM_PIN = 3;
double pwm_value;
 
void setup() {
  pinMode(PWM_PIN, INPUT);  // delcare as intput
  Serial.begin(9600);       // serial baundrate
}

void loop() {
  pwm_value = pulseIn(PWM_PIN, HIGH); // read the pulse ont the pin
  double pwm_valueFloat = pwm_value / 1000; // pwm in milliseconds
  Serial.print("pulse of ");
  Serial.print(pwm_valueFloat,3);
  Serial.println(" milliseconds");
}
