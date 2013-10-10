#include <stdio.h>
#include <stdlib.h>

int grepName(FILE* datei,char* vname){
  int c = getc(datei);
  while(c==' ') c = getc(datei);
  while(1){
    if (c == '='){
      *vname='\0';
      return 0;
    }
    else if (c == ' ') c = getc(datei);
    else{
      *vname=c;
      vname++;
      c = getc(datei);
    }
  }
  return 2;
}

int grepFirstNumber(FILE* datei,char* vname){
  int c = getc(datei);
  while(c == ' ') c = getc(datei);
  while(1){
    if(c == '-') {
      *vname='\0';
      return 0;
    }
    else if (c == ' ') c = getc(datei);
    else{
      if (c == '0' || c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9'){
        *vname=c;
        vname++;
        c = getc(datei);
      }
      else return 1;
    }      
  }
  return 2;
}

int grepSecondNumber(FILE* datei, char* vname){
  int c = getc(datei);
  int onlyone = 0;
  int spaces = 0;
  while(c == ' ') c = getc(datei);
  while(1){
    if(c == '\\' || c == ';') {
      *vname='\0';
      return 0;
    }
    else if (c == ' '){ 
      if(onlyone){ 
        spaces =1;
        c = getc(datei);
      }
      else c= getc(datei);
    }
    else{
      if (c == '0' || c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9'){
        if(spaces){
          *vname='\0';
          return 77;
        }
        *vname=c;
        vname++;
        onlyone = 1;
        c = getc(datei);
      }
      else return 1;
    }      
  }
  return 2;
}
int grepSteps(FILE* datei, char* vname){
  fseek(datei, -1L, SEEK_CUR);
  int c = getc(datei);
  int nothing = 1;
  int onlyone = 0;
  int spaces = 0;
  while(c == ' ') c = getc(datei);
  if(c == '\\') c = getc(datei);
  else if(c == ';'){
    if(nothing==1){ 
      *vname='1';
      vname++;
    }
    *vname = '\0';
    return 0;
  }
  else if (c == '0' || c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9'){
    *vname='1';
    vname++;
    *vname='\0';
    return 77;
  }
  else
    return 1;
  while(1){
    if(c == ';') {
      if(nothing==1){ 
        *vname='1';
        vname++;
      }
      *vname = '\0';
      return 0;
    }
    else if (c == ' '){
      if(onlyone){ 
        spaces =1;
        c = getc(datei);
      }
      else c= getc(datei);
    }
    else{
      if (c == '0' || c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9'){
        if(spaces){
          *vname= '\0';
          return 77;
        }
        else{
          *vname=c;
          vname++;
          nothing = 0;
          onlyone =1;
          c = getc(datei);
        }
      }
      else return 1;
    }      
  }
  return 2;
}



int main(int argc, char **argv) {
  FILE *datei; 
  int c;
  if(argc<2){
    printf("Please set a nexus file as first parameter\n E.g: \" rathian Filename \"\n");
    return 1;
  }
  datei=fopen(argv[1],"rb");
  int charset_flag = 0;  
  int counter = 0;
  char vname[30];
  char first_number[30];
  char second_number[30];
  char step_number[30];
  char lookup[30][4][30];
  int i = 0;
  int true_nexus = 0;
  int set_start = 0;
  while( (c=getc(datei)) != EOF && true_nexus != 6){
    while(c==' ' || c == '\t') c = getc(datei);
    if (c<91 && c>64) c+= 32;
    if(true_nexus == 0 && c=='#') true_nexus++;
    else if(true_nexus == 1 && c=='n') true_nexus++;
    else if(true_nexus == 2 && c=='e') true_nexus++;
    else if(true_nexus == 3 && c=='x') true_nexus++;
    else if(true_nexus == 4 && c=='u') true_nexus++;
    else if(true_nexus == 5 && c=='s') {
      true_nexus++;
      break;
    }
    else true_nexus = 0;
  }
  if(true_nexus != 6) {
    printf("No '#nexus' was found. Is this a nexus file?\n");
    return 4;
  }
  else {
    while( (c=getc(datei)) != EOF){
      if(set_start == 0 && c=='b') set_start++;
      else if(set_start == 1 && c=='e') set_start++;
      else if(set_start == 2 && c=='g') set_start++;
      else if(set_start == 3 && c=='i') set_start++;
      else if(set_start == 4 && c=='n') set_start++;
      else if(set_start == 5 && c==' ') set_start++;
      else if(set_start == 6 && c=='s') set_start++;
      else if(set_start == 7 && c=='e') set_start++;
      else if(set_start == 8 && c=='t') set_start++;
      else if(set_start == 9 && c=='s') set_start++;
      else if(set_start == 10 && c==';') {
        set_start++;
        break;
      }
      else set_start = 0;
    }
  }
  if(set_start != 11) {
    printf("No sets have been found\n");
    return 5;
  }
  while( (c=getc(datei)) != EOF){
    if(charset_flag == 0 && c=='c') charset_flag++;
    else if(charset_flag == 1 && c=='h') charset_flag++;
    else if(charset_flag == 2 && c=='a') charset_flag++;
    else if(charset_flag == 3 && c=='r') charset_flag++;
    else if(charset_flag == 4 && c=='s') charset_flag++;
    else if(charset_flag == 5 && c=='e') charset_flag++;
    else if(charset_flag == 6 && c=='t') charset_flag++;
    else if(charset_flag == 7 && c==' ') charset_flag++;
    else if(charset_flag == 8){
      fseek(datei, -1L, SEEK_CUR);
      grepName(datei,vname);
      grepFirstNumber(datei,first_number);
      int more_numbone = grepSecondNumber(datei,second_number);
      int more_numbtwo = grepSteps(datei,step_number);
//      putc(getc(datei),stdout);
      printf("%s\n",vname);
      printf("%s\n",first_number);
      printf("%s\n",second_number);
      printf("%s\n",step_number);
      int temp = 0;
      for(temp=0;vname[temp]!='\0';temp++) lookup[i][0][temp]=vname[temp];
      lookup[i][0][temp]='\0';
      for(temp=0;first_number[temp]!='\0';temp++) lookup[i][1][temp]=first_number[temp];
      lookup[i][1][temp]='\0';
      for(temp=0;second_number[temp]!='\0';temp++) lookup[i][2][temp]=second_number[temp];
      lookup[i][2][temp]='\0';
      for(temp=0;step_number[temp]!='\0';temp++) lookup[i][3][temp]=step_number[temp];
      lookup[i][3][temp]='\0';
      i++;
      while(more_numbone == 77 || more_numbtwo == 77){
        fseek(datei, -1L, SEEK_CUR);
        grepFirstNumber(datei,first_number);
        more_numbone = grepSecondNumber(datei,second_number);
        more_numbtwo = grepSteps(datei,step_number);
        printf("%s\n",vname);
        printf("%s\n",first_number);
        printf("%s\n",second_number);
        printf("%s\n",step_number);
        int temp = 0;
        for(temp=0;vname[temp]!='\0';temp++) lookup[i][0][temp]=vname[temp];
        lookup[i][0][temp]='\0';
        for(temp=0;first_number[temp]!='\0';temp++) lookup[i][1][temp]=first_number[temp];
        lookup[i][1][temp]='\0';
        for(temp=0;second_number[temp]!='\0';temp++) lookup[i][2][temp]=second_number[temp];
        lookup[i][2][temp]='\0';
        for(temp=0;step_number[temp]!='\0';temp++) lookup[i][3][temp]=step_number[temp];
        lookup[i][3][temp]='\0';
        i++;
        counter++;
      }
      counter++;
      charset_flag = 0;
    }
    else 
      charset_flag = 0;
  }
  int j = 0;
  printf("%i\n",counter);
  for(i=0;i<counter;i++){
    for(j=0;j<4;j++){
      printf("%s  ",lookup[i][j]);
     }
     printf("\n");
  }
  return 0;
}

