#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SET_TAB_DIM1 100
#define SET_TAB_DIM2 5
#define SET_TAB_DIM3 100
#define PART_TAB_DIM1 100
#define PART_TAB_DIM2 6
#define PART_TAB_DIM3 100
int* find_set(char* set_name,char (*look_up)[SET_TAB_DIM2][SET_TAB_DIM3],int len){
  int* ind;
  int counter;
  char* help;
  int cu_len = 4;
  int i,j;
  int hit;
  hit = 0;
  counter =0;
  ind = calloc(cu_len,sizeof(int));
  if(ind == NULL){ 
    printf("Not enough virtual RAM can be allocated\n");
    return EXIT_FAILURE;
  }
  for(i=0;i<len;i++){
    help = &(look_up[i][0][0]);
    hit = 0;
    for(j=0;help[j]!='\0' && set_name[j]!='\0';j++){
      if(help[j]!=set_name[j]) break;
      if(help[j]==set_name[j] && help[j+1]=='\0' && set_name[j+1]=='\0') hit =1;
    }
    if(hit){
      if((1+counter)<cu_len)
        ind[1+counter++]=i;
      else{
        cu_len*=2;
        ind = realloc(ind,cu_len*sizeof(int));
        if(ind == NULL) {
          printf("Not enough virtual RAM can be allocated\n");
          return EXIT_FAILURE;
        }
      }
    }
  }
  ind[0]=counter;
  return ind;
}



int charint(char* temp){
  int i;
  int n;
  n = 0;
  for(i=0;temp[i]!='\0';i++){

    n = n * 10+(temp[i]-'0');
  }
  return n;
}

void intchar(char* temp,int fn,int sn, int st){
  int n;
  int i;
  int copy;
  i =0;
  n = (sn-fn)/st+1;
  for(copy=n;copy!=0;copy/=10) i++;
  temp[i]='\0';
  i--;
  for(;i>=0;i--){
    temp[i]=(n%10)+('0');
    n/=10;
  }
}
int grepName(FILE* datei,char* vname){
  int c;
  c = getc(datei);
  while(isspace(c)) c = getc(datei);
  while(1){
    if (c == '='){
      *vname='\0';
      return 0;
    }
    else if (isspace(c)) c = getc(datei);
    else{
      if(c == ';') {
        printf("The character ';' is not allowed in the name of a variable\n");
        return -1;
      }
      *vname=c;
      vname++;
      c = getc(datei);
    }
  }
  return 2;
}

int grepFirstNumber(FILE* datei,char* vname){
  int c;
  c = getc(datei);
  while(isspace(c)) c = getc(datei);
  while(1){
    if(c == '-') {
      *vname='\0';
      return 0;
    }
    else if (isspace(c)) c = getc(datei);
    else{
      if (isdigit(c)){
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
  int c;
  int onlyone;
  int spaces;
  c = getc(datei);
  onlyone = 0;
  spaces = 0;
  while(isspace(c)) c = getc(datei);
  while(1){
    if(c == '\\' || c == ';') {
      *vname='\0';
      return 0;
    }
    else if (isspace(c)){ 
      if(onlyone){ 
        spaces =1;
        c = getc(datei);
      }
      else c= getc(datei);
    }
    else{
      if (isdigit(c)){
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
  int c;
  int nothing;
  int onlyone;
  int spaces;
  fseek(datei, -1L, SEEK_CUR);
  c = getc(datei);
  nothing  = 1;
  onlyone = 0;
  spaces = 0;
  while(isspace(c)) c = getc(datei);
  if(c == '\\') c = getc(datei);
  else if(c == ';'){
    if(nothing==1){ 
      *vname='1';
      vname++;
    }
    *vname = '\0';
    return 0;
  }
  else if (isdigit(c)){
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
    else if (isspace(c)){
      if(onlyone){ 
        spaces =1;
        c = getc(datei);
      }
      else c= getc(datei);
    }
    else{
      if (isdigit(c)){
        if(spaces){
          *vname= '\0';
          return 77;
        }
        else{
          *vname=c;
          vname++;
          nothing = 0;
          onlyone = 1;
          c = getc(datei);
        }
      }
      else return 1;
    }      
  }
  return 2;
}

int grepModName(FILE* datei, char* vname, char* pname){
  int c;
  int empt;
  c = getc(datei);
  empt = 1;
  while(isspace(c)) c = getc(datei);
  while(1){
    if(isspace(c)) c = getc(datei);
    if(c == ';'){
      printf("The character ';' is not allowed in the name of a variable\n");
      return -1;
    }
    if(c == ':'){
      if(empt){
        printf("No Model has been found\n");
        return -1;
      }
      *pname = '{';
      pname++;
      *pname = '}';
      pname++;
      *pname = '\0';
      *vname= '\0';
      return 0;
    }
    if(c == '+'){
      *pname = '{';
      pname++;
      *pname = '}';
      pname++;
      *pname = c;
      pname++;
      *vname = c;
      vname++;
      c = getc(datei);
    }
    if(c == '{'){
      *pname= c;
      pname++;
      while((c = getc(datei)) != '}'){
        if(!(isspace(c))){
          *pname = c;
          pname++;
        }
      }
      *pname = '}';
      pname++;
      c = getc(datei);
      while(isspace(c)) c = getc(datei);
      if(c == '+'){
        *pname = '+';
        pname++;
        *vname = '+';
        vname++;
        c = getc(datei);
      }
      else {
        *pname = '\0';
        *vname= '\0';
        return 0;
      }
    }
    else{
      *vname=c,
      vname++;
      empt = 0;
      c = getc(datei);
    }
  }
  return 0;
}

int grepVName(FILE* datei, char* vname){
  int c;
  int empt;
  c = getc(datei);
  empt = 1;
  while(isspace(c)) c = getc(datei);
  while(1){
    if(c == ';') {
      if(empt){
        printf("No text after ':' has been found");
        return -1;
      }
      else {
        *vname = '\0';
        return 0;
      }
    }
    if(isspace(c)) {
      while(isspace(c)) c = getc(datei);
      if(c == ';'){
        if(empt){
          printf("No text after ':' has been found");
          return -1;
        }
        else{
          *vname = '\0';        
          return 0;
        }
      }
      else if(c == ','){
        if(empt){
          printf("No text after ':' has been found");
          return -1;
        }
        c = getc(datei);
        *vname = '\0';        
        return 77;
      }
      else{
        *vname= '\0';
        return 66;
      }
    }
    if(c == ','){
      if(empt){
        printf("No text after ':' has been found");
        return -1;
      }
      c = getc(datei);
      *vname = '\0';        
      return 77;
    }
    else{
      *vname=c;
      vname++;
      empt = 0;
      c = getc(datei);
    }
  }
return 0;
}

int main(int argc, char **argv) {
  FILE *datei; 
  int c;
  int temp_summe;
  char *test;
  int charset_flag;
  int counter;
  int charpart_flag;
  char vname[30];
  char first_number[30];
  char second_number[30];
  char step_number[30];
  char lookup[SET_TAB_DIM1][SET_TAB_DIM2][SET_TAB_DIM3];
  char mod_name[30];
  char mod_v_name[30];
  char pname[100];
  char partition_lookup[PART_TAB_DIM1][PART_TAB_DIM2][PART_TAB_DIM3];
  char set_len[30];
  int set_look_len;
  int i;
  int j;
  int k;
  int current_pos;
  int fn,sn,st; /* Calculation of length */
  int true_nexus;
  int set_start;
  int end_counter;
  int partition_counter;
  int name_right;
  int more_numbone;
  int more_numbtwo;
  int more_mv_names;
  int temp;
  int* ind;
  current_pos=0;
  temp = 0;
  if(argc<2){
    printf("Please set a nexus file as first parameter\n E.g: \" rathian Filename \"\n");
    return 1;
  }
  test  = 0;
  if(argc==3) 
    test = argv[2];
  datei=fopen(argv[1],"rb");
  charset_flag = 0;  
  counter = 0;
  partition_counter = 0;
  charpart_flag = 0;  
  end_counter = 0;
  set_start = 0;
  i = 0;
  j = 0;
  true_nexus = 0;
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
    if(charset_flag == 0 && c=='e' && end_counter==0) end_counter++;
    else if(charset_flag == 0 && c=='n' && end_counter==1) end_counter++;
    else if(charset_flag == 0 && c=='d' && end_counter==2) end_counter++;
    else if(charset_flag == 0 && c==';' && end_counter==3) end_counter++;
    else end_counter=0;
    if(end_counter==4) break;
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
      name_right = grepName(datei,vname);
      if( name_right == -1) return -1;
      grepFirstNumber(datei,first_number);
      more_numbone = grepSecondNumber(datei,second_number);
      more_numbtwo = grepSteps(datei,step_number);
      for(temp=0;vname[temp]!='\0';temp++) lookup[i][0][temp]=vname[temp];
      lookup[i][0][temp]='\0';
      for(temp=0;first_number[temp]!='\0';temp++) lookup[i][1][temp]=first_number[temp];
      lookup[i][1][temp]='\0';
      for(temp=0;second_number[temp]!='\0';temp++) lookup[i][2][temp]=second_number[temp];
      lookup[i][2][temp]='\0';
      for(temp=0;step_number[temp]!='\0';temp++) lookup[i][3][temp]=step_number[temp];
      lookup[i][3][temp]='\0';
      fn = charint(first_number);
      sn = charint(second_number);
      st = charint(step_number);
      intchar(set_len,fn,sn,st);
      for(temp=0;set_len[temp]!='\0';temp++) lookup[i][4][temp] = set_len[temp];
      lookup[i][4][temp]='\0';
      i++;
      while(more_numbone == 77 || more_numbtwo == 77){
        fseek(datei, -1L, SEEK_CUR);
        grepFirstNumber(datei,first_number);
        more_numbone = grepSecondNumber(datei,second_number);
        more_numbtwo = grepSteps(datei,step_number);
        temp = 0;
        for(temp=0;vname[temp]!='\0';temp++) lookup[i][0][temp]=vname[temp];
        lookup[i][0][temp]='\0';
        for(temp=0;first_number[temp]!='\0';temp++) lookup[i][1][temp]=first_number[temp];
        lookup[i][1][temp]='\0';
        for(temp=0;second_number[temp]!='\0';temp++) lookup[i][2][temp]=second_number[temp];
        lookup[i][2][temp]='\0';
        for(temp=0;step_number[temp]!='\0';temp++) lookup[i][3][temp]=step_number[temp];
        lookup[i][3][temp]='\0';
        fn = charint(first_number);
        sn = charint(second_number);
        st = charint(step_number);
        intchar(set_len,fn,sn,st);
        for(temp=0;set_len[temp]!='\0';temp++) lookup[i][4][temp] = set_len[temp];
        lookup[i][4][temp]='\0';
        i++;
        counter++;
      }
      counter++;
      charset_flag = 0;
    }
    else 
      charset_flag = 0;
    if(charpart_flag == 0 && c=='c') charpart_flag++;
    else if(charpart_flag == 1 && c=='h') charpart_flag++;
    else if(charpart_flag == 2 && c=='a') charpart_flag++;
    else if(charpart_flag == 3 && c=='r') charpart_flag++;
    else if(charpart_flag == 4 && c=='p') charpart_flag++;
    else if(charpart_flag == 5 && c=='a') charpart_flag++;
    else if(charpart_flag == 6 && c=='r') charpart_flag++;
    else if(charpart_flag == 7 && c=='t') charpart_flag++;
    else if(charpart_flag == 8 && c=='i') charpart_flag++;
    else if(charpart_flag == 9 && c=='t') charpart_flag++;
    else if(charpart_flag == 10 && c=='i') charpart_flag++;
    else if(charpart_flag == 11 && c=='o') charpart_flag++;
    else if(charpart_flag == 12 && c=='n') charpart_flag++;
    else if(charpart_flag == 13 && c==' '){
      temp = 0;
      fseek(datei, -1L, SEEK_CUR);
      name_right = grepName(datei,vname);
      /* check if 'all' is set */
      if(argc==3){
        if(test[0]=='a' && test[1]=='l' && test[2]=='l'){
          vname[0] = 'a';
          vname[1] = 'l';
          vname[2] = 'l';
          vname[3] = '\0';
        }
      }
      if( name_right == -1) return -1;
      grepModName(datei,mod_name, pname);
      more_mv_names = grepVName(datei,mod_v_name);
      while (more_mv_names != 0){
        fseek(datei, -1L, SEEK_CUR);
        for(temp=0;vname[temp]!='\0';temp++)partition_lookup[j][0][temp] = vname[temp];
        partition_lookup[j][0][temp]='\0';
        for(temp=0;mod_name[temp]!='\0';temp++)partition_lookup[j][2][temp] = mod_name[temp];
        partition_lookup[j][2][temp]='\0';
        for(temp=0;pname[temp]!='\0';temp++)partition_lookup[j][3][temp] = pname[temp];
        partition_lookup[j][3][temp]='\0';
        if( more_mv_names == -1 ) return -1;
        else if( more_mv_names == 66){
          for(temp=0;mod_v_name[temp]!='\0';temp++)partition_lookup[j][1][temp] = mod_v_name[temp];
          partition_lookup[j][1][temp]='\0';
          ind = find_set(mod_v_name,lookup,SET_TAB_DIM1);
          temp_summe =0;
          for(temp=1;temp<ind[0]+1;temp++)temp_summe+= charint(&(lookup[ind[temp]][4][0]));
          free(ind);
          current_pos += temp_summe;
          j++;
          partition_counter++;
          more_mv_names = grepVName(datei,mod_v_name);  
        }
        else if( more_mv_names == 77) {
          for(temp=0;mod_v_name[temp]!='\0';temp++)partition_lookup[j][1][temp] = mod_v_name[temp];
          partition_lookup[j][1][temp]='\0';
          ind = find_set(mod_v_name,lookup,SET_TAB_DIM1);
          temp_summe =0;
          for(temp=1;temp<ind[0]+1;temp++)temp_summe+= charint(&(lookup[ind[temp]][4][0]));
          free(ind);
          current_pos += temp_summe;
          j++;
          partition_counter++; 
          grepModName(datei,mod_name, pname); 
          more_mv_names = grepVName(datei,mod_v_name);  
        }
      }
      for(temp=0;vname[temp]!='\0';temp++)partition_lookup[j][0][temp] = vname[temp];
      partition_lookup[j][0][temp]='\0';
      for(temp=0;mod_v_name[temp]!='\0';temp++)partition_lookup[j][1][temp] = mod_v_name[temp];
      partition_lookup[j][1][temp]='\0';
      for(temp=0;mod_name[temp]!='\0';temp++)partition_lookup[j][2][temp] = mod_name[temp];
      partition_lookup[j][2][temp]='\0';
      for(temp=0;pname[temp]!='\0';temp++)partition_lookup[j][3][temp] = pname[temp];
      partition_lookup[j][3][temp]='\0';
      ind = find_set(mod_v_name,lookup,SET_TAB_DIM1);
      temp_summe =0;
      for(temp=1;temp<ind[0]+1;temp++)temp_summe+= charint(&(lookup[ind[temp]][4][0]));
      free(ind);
      current_pos += temp_summe;

      j++;
      partition_counter++;   
      charpart_flag = 0;
    }
    else
      charpart_flag = 0; 
  }
  if(end_counter != 4) {
    printf("No 'end;' for 'begin sets;' was found.\n");
    return 6;
  }
  k = 0;
  for(i=0;i<counter;i++){
    for(k=0;k<5;k++){
      printf("%s  ",lookup[i][k]);
     }
     printf("\n");
  }
  for(i=0;i<partition_counter;i++){
    for(k=0;k<4;k++){
      printf("%s  ",partition_lookup[i][k]);
     }
     printf("\n");
  }
  return 0;
}
