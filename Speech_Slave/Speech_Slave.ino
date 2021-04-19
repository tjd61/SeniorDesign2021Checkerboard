#include <uspeech.h>
#include <string.h>
#include <Wire.h>
#define pi 3.1415926535897932384626433832795
signal voice(A0);
char option = 0;
String possibleMoves[16] = {"","","","","","","","","","","","","","","",""};
int num_moves = 16;
int num_bytes = 0;
int num_strings = 0;
//int samples = 0;
void setup(){
  voice.calibrate();
  voice.f_enabled = true;
  voice.minVolume = 100;
  voice.micPowerThreshold = 0;
  voice.fconstant = 200;  /*!< This is the threshold for /f/, configure it yourself */
  voice.econstant = 3;  /*!< This is the threshold for /ee/, /i/, configure it yourself */
  voice.aconstant = 5;  /*!< This is the threshold for /a/ /o/ /r/ /l/, configure it yourself */
  voice.zconstant = 8;  /*!< This is the threshold for /z/ /v/ /w/, configure it yourself */
  voice.shconstant = 16; /*!< This is the threshold for /sh/ /ch/, above this everything else is regarded as /s/ */
  voice.sconstant = 12;
  Wire.begin(8);                    //join i2c bus with address #4
  Wire.onReceive(receiveEvent);     //register event
  Wire.onRequest(requestEvent);     //request event
  Serial.begin(9600);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
  Serial.println("Master writes data to Slave");
  int c, x;
  //int n=0,i;
  char x_c;
  int i;
  //num_strings = 0;
  String moveStr;
  while(1 < Wire.available()){    //loop through all but the last
    i = 0;
    moveStr = "";
    while(i < 4){
      x = Wire.read();            //receive byte as an integer
      if(i%2 == 0){
        x_c = getCharLetter(x);
      }else{
        x_c = getCharNumber(x);
      }
      moveStr += x_c;
      i++;
    }
    possibleMoves[num_strings] = moveStr;
    //Serial.println(num_strings);
    //Serial.println(moveStr);
    //Serial.println(possibleMoves[num_strings]);
    num_strings++;
    //possibleMoves[num_bytes/4] = moveStr;
    //n++;
    num_bytes += 4;
  }
  if(num_bytes == 64){
      num_bytes = 0;
      num_strings = 0;
      int x=0;
      while(x < 16){
        Serial.print(possibleMoves[x]);
        Serial.print(", ");
        x++;
      }
  }
}

char getCharLetter(int x){
  if(x == 0){
    return 'A';
  }else if(x == 1){
    return 'B';
  }else if(x == 2){
    return 'C';
  }else if(x == 3){
    return 'D';
  }else if(x == 4){
    return 'E';
  }else if(x == 5){
    return 'F';
  }else if(x == 6){
    return 'G';
  }else{
    return 'H';
  }
}

char getCharNumber(int x){
  if(x == 0){
    return '1';
  }else if(x == 1){
    return '2';
  }else if(x == 2){
    return '3';
  }else if(x == 3){
    return '4';
  }else if(x == 4){
    return '5';
  }else if(x == 5){
    return '6';
  }else if(x == 6){
    return '7';
  }else{
    return '8';
  }
}

// function that executes whenever data is requested from master
void requestEvent(){
    Serial.println("Master requests data from Slave");
    int i=0;
    char c;
    String phonemes;
    String guesses[4];
    char moveChar;
    String moveStr = "";
    int mode = 0;
    while(i < 4){
      if(i%2 == 0){
        mode = 0;
      }else{
        mode = 1;
      }
      voice.sample();
      c = guessPhoneme(mode);
      phonemes = "";
      if(c == 'm'){
        //Serial.print(c);
      }else{
        while(c != 'm'){
          phonemes += c;
          voice.sample();
          c = guessPhoneme(mode);
        }
        Serial.println(phonemes);
        guesses[i] = phonemes;
        i++;
      }
    }
    option = 0;
    i=0;
    Serial.println("Move Guesses: ");
    while(i<4){
      Serial.println(guesses[i]);
      i++;
    }
    //moveStr = guessMove(guesses);
    int move_index = guessMove(guesses);
    moveStr = possibleMoves[move_index];
    Serial.println("Final guess: ");
    Serial.println(moveStr);
    Serial.println("Slave writes data to Master");
    Serial.println();
    Wire.write(move_index + 1);           //respond with final move index
    //byte x = 0;
    //Wire.write(0);
}

void loop(){
  char c=voice.getPhoneme();
}

int guessMove(String guesses[4]){
  int N=num_moves;
  //String possibleMoves[N] = {"A5B6", "B2A3", "C1D2", "E1D2", "F2E3", "F2G3", "H2G3"}; //Scenario 1
  String moveGuess = "";
  String finalMove;
  //int correctChars[N] = {0,0,0,0,0,0,0};
  int correctChars[num_moves];
  int n=0;
  while(n < num_moves){
    correctChars[n] = 0;
    n++;
  }
  //First need to turn guesses into letters/numbers using best logical approximation
  int i=0;
  while(i<4){
    if(containsA(guesses[i])){
      moveGuess += 'A';
    }else if(containsE(guesses[i])){
      moveGuess += 'E';
    }else if(containsS(guesses[i])){
      moveGuess += 'S';
    }else if(containsI(guesses[i])){
      moveGuess += 'I';
    }else{
      moveGuess += '_';
    }
    i++;
  }
  //Second need to compare guesses with the list of possible moves to determine the move
  int j;
  i=0;
  Serial.println("Comparing guesses");
  while(i<N){
    j=0;
    int x=0;
    /*while(x < 16){
      Serial.print(possibleMoves[x]);
      Serial.print(", ");
      x++;
    }*/
    //Serial.println();
    //Serial.println(possibleMoves[i]);
    if(possibleMoves[i].charAt(0) == possibleMoves[i].charAt(2) && possibleMoves[i].charAt(1) == possibleMoves[i].charAt(3)){
      i++;
      //Serial.println("Bad move");
      continue;
    }
    while(j<4){
      if(possibleMoves[i].charAt(j) == moveGuess[j]){
        correctChars[i] += 1;
      }else if(letterMatches(moveGuess[j], possibleMoves[i].charAt(j))){
        correctChars[i] += 1;
      }else if(numberMatches(moveGuess[j], possibleMoves[i].charAt(j))){
        correctChars[i] += 1;
      }
      j++;
    }
    i++;
  }
  Serial.println("Before move: ");
  Serial.println(moveGuess);
  //Third need to pick the move with the most correct characters
  i=0;
  int most_correct = 0;
  int final_move_index = 0;
  while(i<N){
    if(correctChars[i] > most_correct){
      most_correct = correctChars[i];
      finalMove = possibleMoves[i];
      final_move_index = i;
    }
    i++;
  }
  //return finalMove;
  return final_move_index;
}

boolean letterMatches(char c1, char c2){
  if(c1 == 'A'){
    if(c2 == 'A' || c2 == 'H'){
      return true;
    }
  }else if(c1 == 'E'){
    if(c2 == 'E' || c2 == 'B' || c2 == 'D' || c2 == 'G'){
      return true;
    }
  }else if(c1 == 'S'){
    if(c2 == 'C'){
      return true;
    }
  }
  return false;
}

boolean numberMatches(char c1, char c2){
  if(c1 == 'A'){
    if(c2 == '1' || c2 == '4' || c2 == '5' || c2 == '8'){
      return true;
    }
  }else if(c1 == 'E'){
    if(c2 == '3' || c2 == '7'){
      return true;
    }
  }else if(c1 == 'S'){
    if(c2 == '6' || c2 == '7' || c2 == '2'){
      return true;
    }
  }else if(c1 == 'I'){
    if(c2 == '5' || c2 == '6'){
      return true;
    }
  }
  return false;
}

boolean containsA(String guess){
  int i=0;
  int num_chars = guess.length();
  if(guess == "a"){
    return true;
  }
  while(i < num_chars - 1){
    if(guess[i] == 'a' && guess[i + 1] == 'e'){
      return true;
    }
    i++;
  }
  return guess[0] == 'a';
}

boolean containsE(String guess){
  int i=0;
  int num_chars = guess.length();
  if(guess == "e"){
    return true;
  }
  while(i < num_chars - 1){
    if(guess[i] == 'e' && guess[i + 1] == 'e'){
      return true;
    }
    i++;
  }
  return guess[0] == 'e';
}

boolean containsS(String guess){
  int i=0;
  int num_chars = guess.length();
  while(i < num_chars - 1){
    if(guess[i] == 's'){
      return true;
    }
    i++;
  }
}

boolean containsI(String guess){
  int i=0;
  int num_chars = guess.length();
  if(guess == "i"){
    return true;
  }
  while(i < num_chars - 1){
    if(guess[i] == 'i'){
      return true;
    }
    i++;
  }
  return false;
}

void printFilter(float out[32]){
  int M = 32;
  int i=0;
  while(i < M){
      Serial.print(out[i]);
      Serial.print(", ");
      i++;
    }
    Serial.print("\n");
}

char guessPhoneme(int mode){
    // y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] - b1*y[n-1] - b2*y[n-2];
    int i=0, j=0, k=0;
    int N=1, M=32;
    float out[N][M];
    float Q = 2;
    int fc[N] = {5500};
    int fs = 16000;
    String outStr[N];
    char guess = 'm';
    while(k < N){
      String outputString = "";
      float a0, a1, a2, b1, b2;
      float w = 2.0*pi * fc[k] / fs;
      float b = 0.5*((1.0 - tan(w / (2.0*Q))) / (1.0 + tan(w / (2.0*Q))));
      float g = (0.5 + b)*cos(w);
      float lastOut1 = 0, lastOut2 = 0;
      float lastIn1 = 0, lastIn2 = 0;
      float minPeak = 0, maxPeak = 0;
      float localMax = 0, localMax2 = 0;
      int tmin, tmax;
      int zero1 = 0, zero2 = 0;
      float amplitude;
      int period;
      float error = -1, e_error, a_error, b_error, o_error, s_error, i_error;
      a0 = 0.5 - b;
      a1 = 0.0;
      a2 = -(0.5 - b);
      b1 = 4.0 * g;
      b2 = 10.0 / 3.0 * b;
      i=0;
      while(i < M){
        out[k][i] = a0*voice.arr[i] + a1*lastIn1 + a2*lastIn2 - b1*lastOut1 - b2*lastOut2;
        lastIn2 = lastIn1;
        lastIn1 = voice.arr[i];
        lastOut2 = lastOut1;
        lastOut1 = out[k][i];
        if((lastOut2 < 0 && lastOut1 > 0)||(lastOut2 > 0 && lastOut1 < 0)){
          if(zero1 == 0){
            zero1 = i; 
          }else if(zero2 == 0){
            zero2 = i;
          }
        }
        if(lastOut1 < minPeak){
          minPeak = lastOut1;
          tmin = i;
        }else if(lastOut1 > maxPeak){
          maxPeak = lastOut1;
          tmax = i;
          if(i < M/2){
            localMax = lastOut1;
          }else{
            localMax2 = lastOut1;
          }
        }
        i++;
      }
      amplitude = abs(maxPeak - minPeak);
      period = abs(tmax - tmin);
      if(amplitude < 15){
        return 'm';
      }else{
        e_error = checkE(amplitude, zero1, zero2);
        a_error = checkA(amplitude, zero1, zero2);
        b_error = checkB(amplitude, zero1, zero2);
        o_error = checkO(amplitude, zero1, zero2);
        s_error = checkS(amplitude, zero1, zero2);
        i_error = checkI(minPeak, localMax, localMax2, zero1, zero2);
        if(mode == 0){
          error = min(min(e_error, a_error), s_error);
        }else{
          error = min(min(e_error, a_error), min(s_error, i_error));
        }
        if(error == e_error){
          guess = 'e';
        }else if(error == a_error){
          guess = 'a';
        }else if(error == b_error){
          guess = 'b';
        }else if(error == o_error){
          guess = 'o';
        }else if(error == s_error){
          guess = 's';
        }else if(error == i_error){
          guess = 'i';
        }
        Serial.print("Amplitude Error: ");
        Serial.print(error);
        Serial.print("\n");
        Serial.print("E Error: ");
        Serial.print(e_error);
        Serial.print("\n");
        Serial.print("A Error: ");
        Serial.print(a_error);
        Serial.print("\n");
        Serial.print("B Error: ");
        Serial.print(b_error);
        Serial.print("\n");
        Serial.print("O Error: ");
        Serial.print(o_error);
        Serial.print("\n");
        Serial.print("S Error: ");
        Serial.print(s_error);
        Serial.print("\n");
        Serial.print("I Error: ");
        Serial.print(i_error);
        Serial.print("\n");
      }
      k++;
      if(error <= 0.5 && error != -1){
        Serial.print("Guess: ");
        Serial.print(guess);
        Serial.print("\n");
        return guess;
      }
    }
    return 'm';
}

void calculateFilter(){
    // y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] - b1*y[n-1] - b2*y[n-2];
    int i=0, j=0, k=0;
    int N=1, M=32;
    float out[1][32];
    float Q = 2;
    int fc[N] = {5500};
    int fs = 16000;
    while(k < N){
      String outputString = "";
      float a0, a1, a2, b1, b2;
      float w = 2.0*pi * fc[k] / fs;
      float b = 0.5*((1.0 - tan(w / (2.0*Q))) / (1.0 + tan(w / (2.0*Q))));
      float g = (0.5 + b)*cos(w);
      float lastOut1 = 0, lastOut2 = 0;
      float lastIn1 = 0, lastIn2 = 0;
      a0 = 0.5 - b;
      a1 = 0.0;
      a2 = -(0.5 - b);
      b1 = 4.0 * g;
      b2 = 10.0 / 3.0 * b;
      i=0;
      outputString += "filter ";
      outputString += fc[k];
      outputString += ", ";
      while(i < M){
        out[k][i] = a0*voice.arr[i] + a1*lastIn1 + a2*lastIn2 - b1*lastOut1 - b2*lastOut2;
        lastIn2 = lastIn1;
        lastIn1 = voice.arr[i];
        lastOut2 = lastOut1;
        lastOut1 = out[k][i];
        outputString += out[k][i];
        outputString += ", ";
        i++;
      }
      Serial.println(outputString);
      k++;
    }
    Serial.print("unfiltered, ");
    while(j < M){
      Serial.print(voice.arr[j]);
      Serial.print(", ");
      j++;
    }
    Serial.print("\n");
}

float checkE(float amplitude, int zero1, int zero2){
  int k=0;
  float s_amp_quiet[8] = {74.25, 71.47, 73.79, 72.09, 72.29, 69.16, 70.17, 67.58};
  float s_t_quiet[8] = {40, 40, 35, 50, 50, 35, 35, 50};
  float s_amp_loud[3] = {108.32, 87.03, 93.09};
  float s_t_loud[3] = {35, 50, 35};
  float s_amp_average = 70.26;
  float s_t_average = 42.5;
  float period = abs(zero2 - zero1)*2;
  float amp_error = -1;
  float t_error = -1;
  while(k < 8){
    //Check if amplitude within error margin
    float temp_amp_error = abs((amplitude - s_amp_quiet[k])/s_amp_quiet[k]);
    float temp_period_error = abs((period - s_t_quiet[k])/s_t_quiet[k]);
    if(amp_error < 0){
      amp_error = temp_amp_error;
      t_error = temp_period_error;
    }else if(temp_amp_error < amp_error){
      amp_error = temp_amp_error;
      t_error = temp_period_error;
    }
    k++;
  }
  //Check amplitude and period against sample averages
  float avg_amp_error = abs((amplitude - s_amp_average)/s_amp_average);
  float avg_period_error = abs((period - s_t_average)/s_t_average);
  float avg_mean_error = (avg_amp_error + avg_period_error)/2;
  float mean_error = (amp_error + t_error)/2;
  if(avg_mean_error < mean_error){
    amp_error = avg_amp_error;
    t_error = avg_period_error;
  }
  Serial.print("e amp_error: ");
  Serial.print(amp_error);
  Serial.print(", t_error: ");
  Serial.print(t_error);
  Serial.print("\n");
  float avg_error = (amp_error + t_error)/2;
  return avg_error;
}

float checkA(float amplitude, int zero1, int zero2){
  int k=0;
  float s_amp[8] = {165.01, 155.57, 138.6, 126.62, 168.36, 148.08, 148.77, 140.6};
  int s_t = 17;
  float s_amp_average = 150.14;
  float s_t_average = 17;
  float period = abs(zero2 - zero1)*2;
  float t_error = abs((period - s_t)/s_t);
  float amp_error = -1;
  while(k < 8){
    //Check if amplitude within error margin
    float temp_amp_error = abs((amplitude - s_amp[k])/s_amp[k]);
    if(amp_error < 0){
      amp_error = temp_amp_error;
    }else if(temp_amp_error < amp_error){
      amp_error = temp_amp_error;
    }
    k++;
  }
  //Check amplitude and period against sample averages
  float avg_amp_error = abs((amplitude - s_amp_average)/s_amp_average);
  float avg_period_error = abs((period - s_t_average)/s_t_average);
  float avg_mean_error = (avg_amp_error + avg_period_error)/2;
  float mean_error = (amp_error + t_error)/2;
  if(avg_mean_error < mean_error){
    amp_error = avg_amp_error;
    t_error = avg_period_error;
  }
  Serial.print("a amp_error: ");
  Serial.print(amp_error);
  Serial.print(", t_error/2: ");
  Serial.print(t_error/2);
  Serial.print("\n");
  if(t_error < 0.5){
    t_error = t_error/2;
    if(t_error < 0.3){
      return t_error; 
    }
  }
  if(amp_error < 0.8){
    t_error = t_error/2;
  }
  float avg_error = (amp_error + t_error)/2;
  return avg_error;
}

float checkI(float minVal, float localMax, float localMax2, int zero1, int zero2){
  int k=0;
  //float s_amp[4] = {187.29, 158.78, 265.49, 216.02};
  float maxVal = max(localMax, localMax2);
  float s_min[7] = {-38.79, -39.3, -35.09, -36.3, -37.84, -34.33, -36.79};
  float s_max[7] = {61.49, 58.94, 57.45, 53.13, 51.93, 51.93, 55.06};
  float s_local_max = 23;
  float s_peak_ratio[7] = {2.69, 2.56, 2.5, 2.31, 2.26, 2.26, 2.39};
  float s_t= 17;
  float sample_average = 1.2;
  float period = abs(zero2 - zero1)*2;
  float min_error = -1, max_error = -1;
  float average_error = -1;
  float t_error = -1;
  float peak_ratio_error;
  while(k < 7){
    //Check if amplitude within error margin
    float temp_min_error = abs((minVal - s_min[k])/s_min[k]);
    float temp_max_error = abs((maxVal - s_max[k])/s_max[k]);
    float temp_avg_error = (temp_min_error + temp_max_error)/2;
    float temp_period_error = abs((period - s_t)/s_t);
    float peak_ratio = maxVal/min(localMax, localMax2);
    float temp_peak_ratio_error = abs((peak_ratio - s_peak_ratio[k])/s_peak_ratio[k]);
    if(average_error < 0){
      min_error = temp_min_error;
      max_error = temp_max_error;
      t_error = temp_period_error;
      peak_ratio_error = temp_peak_ratio_error;
      average_error = (min_error + max_error)/2;
    }else if(temp_avg_error < average_error){
      min_error = temp_min_error;
      max_error = temp_max_error;
      t_error = temp_period_error;
      peak_ratio_error = temp_peak_ratio_error;
      average_error = (min_error + max_error)/2;
    }
    k++;
  }
  Serial.print("i min_error: ");
  Serial.print(min_error);
  Serial.print(", i max_error: ");
  Serial.print(max_error);
  Serial.print(", t_error: ");
  Serial.print(t_error);
  Serial.print("\n");
  Serial.print(", peak_ratio_error: ");
  Serial.print(peak_ratio_error);
  Serial.print("\n");
  if(peak_ratio_error < 0.9){
    if((min_error < 0.3 && max_error < 0.3) && t_error < 0.9){
       return (min_error + max_error)/4;
    }else if(min_error < 0.4 && max_error < 0.4){
      return (min_error + max_error + peak_ratio_error)/6;
    }
  }
  if((min_error < 0.3 && max_error < 0.3) && t_error < 0.9){
       return (min_error + max_error)/2;
    }else if(min_error < 0.4 && max_error < 0.4){
      return (min_error + max_error + peak_ratio_error)/3;
    }
  
  return (min_error + max_error + t_error);
}

float checkO(float amplitude, int zero1, int zero2){
  int k=0;
  float s_amp[7] = {115.86, 105.45, 100.53, 97.31, 90.5, 121.6, 101.06};
  float s_t = 33.33;
  float s_amp_average = 112.9;
  float s_t_average = 33.33;
  float period = abs(zero2 - zero1)*2;
  float t_error = abs((period - s_t)/s_t);
  float amp_error = -1;
  while(k < 7){
    //Check if amplitude within error margin
    float temp_amp_error = abs((amplitude - s_amp[k])/s_amp[k]);
    if(amp_error < 0){
      amp_error = temp_amp_error;
    }else if(temp_amp_error < amp_error){
      amp_error = temp_amp_error;
    }
    k++;
  }
  //Check amplitude and period against sample averages
  float avg_amp_error = abs((amplitude - s_amp_average)/s_amp_average);
  float avg_period_error = abs((period - s_t_average)/s_t_average);
  float avg_mean_error = (avg_amp_error + avg_period_error)/2;
  float mean_error = (amp_error + t_error)/2;
  if(avg_mean_error < mean_error){
    amp_error = avg_amp_error;
    t_error = avg_period_error;
  }
  Serial.print("o amp_error: ");
  Serial.print(amp_error);
  Serial.print(", t_error: ");
  Serial.print(t_error);
  Serial.print("\n");
  if(amp_error < 0.5 && t_error < 0.5){
    float avg_error = (amp_error + t_error)/2;
    return avg_error;
  }
  return max(amp_error, t_error);
}

float checkB(float amplitude, int zero1, int zero2){
  int k=0;
  float s_amp[5] = {26.12, 111.88, 113.11, 29.15, 64.05}; //Removed sample 5, 6, 8
  int s_t[5] = {80, 40, 40, 50, 60};
  float period = abs(zero2 - zero1)*2;
  float amp_error = -1;
  float t_error;
  while(k < 5){
    //Check if amplitude within error margin
    float temp_amp_error = abs((amplitude - s_amp[k])/s_amp[k]);
    if(amp_error < 0){
      amp_error = temp_amp_error;
      t_error = abs((period - s_t[k])/s_t[k]);
    }else if(temp_amp_error < amp_error){
      amp_error = temp_amp_error;
      t_error = abs((period - s_t[k])/s_t[k]);
    }
    k++;
  }
  Serial.print("b amp_error: ");
  Serial.print(amp_error);
  Serial.print(", t_error: ");
  Serial.print(t_error);
  Serial.print("\n");
  return max(amp_error, t_error);
}

float checkS(float amplitude, int zero1, int zero2){
  int k=0;
  float s_amp_average = 20;
  float s_t_average = 5.55;
  float period = abs(zero2 - zero1)*2;
  float amp_error = abs((amplitude - s_amp_average)/s_amp_average);
  float t_error = abs((period - s_t_average)/s_t_average);
  float mean_error = (amp_error + t_error)/2;
  Serial.print("s amp_error: ");
  Serial.print(amp_error);
  Serial.print(", t_error/2: ");
  Serial.print(t_error/2);
  Serial.print("\n");
  if(t_error < 0.5){
    t_error = t_error/4;
  }
  if(amp_error < 0.9){
    return t_error/2;
  }
  return (amp_error + t_error)/2;
}
