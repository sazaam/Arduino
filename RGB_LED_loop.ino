

const byte RGB[3] = {3,5,6};
int rgb[3] = {255, 255, 255};
int ms_cnt = 0;
int amp = 50 ;
int bias = 255 - amp ;

unsigned long prev_ms = 0;
unsigned long diff_ms = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}


void plotter(){
  
  Serial.print("R :"); 
  Serial.print(rgb[0]);
  Serial.print(", G :"); 
  Serial.print(rgb[1]);
  Serial.print(", B :"); 
  Serial.print(rgb[2]);
}

void loop() {
  // put your main code here, to run repeatedly:

  diff_ms = millis() - prev_ms;
  if(diff_ms){
    ms_cnt+= diff_ms;
    float rad1 = PI * (ms_cnt) / 500;
    float rad2 = .5 * rad1;
    rgb[0] = amp * cos(rad1);
    rgb[1] = -rgb[0];
    rgb[2] = amp * cos(rad2);

    for(int i = 0 ; i < 3; i++){
      rgb[i] += bias;
      analogWrite(RGB[i], rgb[i]) ; 

    }
    prev_ms = millis();
    plotter() ;
  }



}
