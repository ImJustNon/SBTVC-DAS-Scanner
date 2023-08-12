const int trigPin = D7; //ประกาศขา trig
const int echoPin = D6; //ประกาศขา echo
const int relayPin = D5; //ประกาศขา led
const int DISTANCE = 25;
long duration, distance; //ประกาศตัวแปรเก็บค่าระยะ

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT); //สั่งให้ขา echo ใช้งานเป็น input
  pinMode(trigPin, OUTPUT); //สั่งให้ขา trig ใช้งานเป็น output
  pinMode(relayPin, OUTPUT);
}
void loop() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(trigPin, LOW); //ใช้งานขา trig
  
  duration = pulseIn(echoPin, HIGH); //อ่านค่าของ echo
  distance = (duration / 2) / 29.1; //คำนวณเป็น centimeters
  Serial.print(distance); 
  Serial.print(" cm\n");
  if(distance <= DISTANCE){
    digitalWrite(relayPin, HIGH);
  } 
  else{
    digitalWrite(relayPin, LOW);
  }
  delay(500);
}