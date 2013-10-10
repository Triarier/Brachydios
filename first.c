#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
  FILE *datei; 
  int c;
  fpos_t pos;
  char filename[255];
  if(argc<2){
    printf("Please set a nexus file as first parameter\n E.g: \" rathian Filename \"\n");
    return 1;
  }
  datei=fopen(argv[1],"rb");
  fgetpos(datei,&pos);
  while( (c=getc(datei)) != EOF) putc(c, stdout);  
  char charset[] = {"charset"};
  fsetpos(datei,&pos);
  int charset_flag = 0;  
  int counter = 0;
  while( (c=getc(datei)) != EOF){
   if(charset_flag == 0 && c=='c') charset_flag++; 
   if(charset_flag == 1 && c=='h') charset_flag++;
   if(charset_flag == 2 && c=='a') charset_flag++;
   if(charset_flag == 3 && c=='r') charset_flag++;
   if(charset_flag == 4 && c=='s') charset_flag++;
   if(charset_flag == 5 && c=='e') charset_flag++;
   if(charset_flag == 6 && c=='t') charset_flag++;
   if(charset_flag == 7 && c!=' ') return 2;
   
   
   
   
   if(charset_flag == 7) {
     counter++;charset_flag = 0;
   }
    
  }
  printf("%i\n",counter);
return 0;
}

