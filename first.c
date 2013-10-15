#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SET_LENG 10
#define SET_TAB_DIM3 20
#define PART_TAB_DIM2 4
#define PART_TAB_DIM3 200
#define PART_LENG 10

struct parsed_part {
  char*** lookup;
  size_t* start;
  size_t* end;
};
/*copy from 1 to 2 */
void copy_str(char* str1,char* str2){
  int i;
  for(i=0;str1[i]!='\0';i++) 
    str2[i]=str1[i];
  str2[i]='\0';
}
/* Finde indexes of the right sets. Returning an int*. Int[0] tells the number of found indexes */
int* find_set(char* set_name,char** look_up,int len){
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
    exit(EXIT_FAILURE);
  }
  for(i=0;i<len;i++){
    help = &(look_up[i][0]);
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
          exit(EXIT_FAILURE);
        }
      }
    }
  }
  ind[0]=counter;
  if(counter==0){
    printf("Partition could not be build. Is the right charset existent?");
    exit(EXIT_FAILURE);
  }
  return ind;
}


/* Converts a char[] to an int */
int charint(char* temp){
  int i;
  int n;
  n = 0;
  for(i=0;temp[i]!='\0';i++){

    n = n * 10+(temp[i]-'0');
  }
  return n;
}
/* Converts an int to a char[] */
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
/* Greps the name of the variable */
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
/* Greps the starting number */
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
/* Greps the end number and tells if there are more start/end points */
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
/* Greps the Stepnumber and tells if there are more start/end point */ 
int grepSteps(FILE* datei, char* vname){
  int c;
  int nothing;
  int onlyone;
  int spaces;
  fseek(datei, -1L, SEEK_CUR);
  c = getc(datei);
  nothing = 1;
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
  *pname = '\0';
  *vname = '\0';
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

struct parsed_part* do_something(FILE* datei,int all){
  struct parsed_part ret;
  int c;
  int temp_summe;
  int charset_flag;
  int counter;
  int charpart_flag;
  char vname[100];
  char first_number[100];
  char second_number[100];
  char step_number[100];
  char **lookup,**lookup2;
  char mod_name[100];
  char mod_v_name[100];
  char pname[100];
  char ***partition_lookup,***partition_lookup2;
  char new_pos[100];
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
  size_t* pset_fn,*pset_fn2;
  size_t* pset_sn,*pset_sn2;
  size_t* pset_st,*pset_st2;
  size_t* pset_leng,*pset_leng2;
  size_t* ppart_fn,* ppart_fn2;
  size_t* ppart_sn,* ppart_sn2;
  size_t set_leng;
  size_t part_leng;
  struct parsed_part *pret;
  pret=NULL;
  set_leng = SET_LENG;
  part_leng=PART_LENG;
  pset_fn = (size_t*) malloc(sizeof(size_t)*set_leng);
  pset_sn = (size_t*) malloc(sizeof(size_t)*set_leng);
  pset_st = (size_t*) malloc(sizeof(size_t)*set_leng);
  pset_leng = (size_t*) malloc(sizeof(size_t)*set_leng);
  ppart_fn = (size_t*) malloc(sizeof(size_t)*part_leng);
  ppart_sn = (size_t*) malloc(sizeof(size_t)*part_leng);
  partition_lookup = (char ***) malloc(sizeof(char**)*part_leng);
  for(i=0;i<part_leng;i++)
    partition_lookup[i]= (char **) malloc(sizeof(char*)*PART_TAB_DIM2);
  for(i=0;i<part_leng;i++){
    for(j=0;j<PART_TAB_DIM2;j++){
      partition_lookup[i][j]= (char *) malloc(sizeof(char)*PART_TAB_DIM3);
    }
  }
  lookup = (char **) malloc(sizeof(char*)*set_leng);
  for(i=0;i<set_leng;i++)
    lookup[i]= (char*) malloc(sizeof(char*)*SET_TAB_DIM3);
  current_pos=0;
  temp = 0;
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
    exit(EXIT_FAILURE);
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
    exit(EXIT_FAILURE);
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
      if( name_right == -1) exit(EXIT_FAILURE);
      grepFirstNumber(datei,first_number);
      more_numbone = grepSecondNumber(datei,second_number);
      more_numbtwo = grepSteps(datei,step_number);
      for(temp=0;vname[temp]!='\0';temp++) lookup[i][temp]=vname[temp];
      lookup[i][temp]='\0';
      fn = charint(first_number);
      sn = charint(second_number);
      st = charint(step_number);
      pset_fn[i]=charint(first_number);
      pset_sn[i]=charint(second_number);
      pset_st[i]=charint(step_number);
      pset_leng[i]=(sn-fn)/st+1;
      i++;
      if(i==set_leng){
        set_leng=set_leng *2;
        lookup2 = (char **) malloc(sizeof(char*)*set_leng);
        if(lookup2 == NULL) {
          fprintf(stderr, "Not enought virtual RAM could have been allocated.");
          exit(EXIT_FAILURE);
        }
        for(k=0;k<set_leng;k++){
          lookup2[k]= (char*) malloc(sizeof(char*)*SET_TAB_DIM3);
          if(lookup2[k]==NULL){
            fprintf(stderr, "Not enought virtual RAM could have been allocated.");
            exit(EXIT_FAILURE);
          }
          if(k<(set_leng/2)) copy_str(&lookup[k][0],&lookup2[k][0]);
        }
        for(k=0;k<(set_leng/2);k++) free(lookup[k]);
        free(lookup);
        lookup=lookup2;
        pset_leng2 = malloc(sizeof(size_t)*set_leng);
        pset_fn2 = malloc(sizeof(size_t)*set_leng);
        pset_sn2 = malloc(sizeof(size_t)*set_leng);
        pset_st2 = malloc(sizeof(size_t)*set_leng);
        if(pset_leng2 == NULL || pset_fn2 == NULL || pset_sn2 == NULL || pset_st2 == NULL){
          fprintf(stderr, "Not enought virtual RAM could have been allocated.");
          exit(EXIT_FAILURE);
        }
        for(k=0;k<(set_leng/2);k++){
          pset_leng2[k]=pset_leng[k];
          pset_fn2[k]=pset_fn[k];
          pset_sn2[k]=pset_sn[k];
          pset_st2[k]=pset_st[k];
        }
        free(pset_st);
        free(pset_sn);
        free(pset_fn);
        free(pset_leng);
        pset_sn=pset_sn2;
        pset_fn=pset_fn2;
        pset_leng=pset_leng2;
        pset_st=pset_st2;
      }
      while(more_numbone == 77 || more_numbtwo == 77){
        fseek(datei, -1L, SEEK_CUR);
        grepFirstNumber(datei,first_number);
        more_numbone = grepSecondNumber(datei,second_number);
        more_numbtwo = grepSteps(datei,step_number);
        temp = 0;
        for(temp=0;vname[temp]!='\0';temp++) lookup[i][temp]=vname[temp];
        lookup[i][temp]='\0';
        pset_fn[i]=charint(first_number);
        pset_sn[i]=charint(second_number);
        pset_st[i]=charint(step_number);
        pset_leng[i]=(pset_sn[i]-pset_fn[i])/pset_st[i]+1;
        i++;
        if(i>=set_leng){
          set_leng=set_leng *2;
          lookup2 = (char **) malloc(sizeof(char*)*set_leng);
          if(lookup2 == NULL) {
            fprintf(stderr, "Not enought virtual RAM could have been allocated.");
            exit(EXIT_FAILURE);
          }
          for(k=0;k<set_leng;k++){
            lookup2[k]= (char*) malloc(sizeof(char)*SET_TAB_DIM3);
            if(lookup2[k]==NULL){
              fprintf(stderr, "Not enought virtual RAM could have been allocated.");
              exit(EXIT_FAILURE);
            }
            if(k<(set_leng/2)) copy_str(&lookup[k][0],&lookup2[k][0]);
          }
          for(k=0;k<(set_leng/2);k++) free(lookup[k]);
          free(lookup);
          lookup=lookup2;
          pset_leng2 = malloc(sizeof(size_t)*set_leng);
          pset_fn2 = malloc(sizeof(size_t)*set_leng);
          pset_sn2 = malloc(sizeof(size_t)*set_leng);
          pset_st2 = malloc(sizeof(size_t)*set_leng);
          if(pset_leng2 == NULL || pset_fn2 == NULL || pset_sn2 == NULL || pset_st2 == NULL){
            fprintf(stderr, "Not enought virtual RAM could have been allocated.");
            exit(EXIT_FAILURE);
          }
          for(k=0;k<(set_leng/2);k++){
            pset_leng2[k]=pset_leng[k];
            pset_fn2[k]=pset_fn[k];
            pset_sn2[k]=pset_sn[k];
            pset_st2[k]=pset_st[k];
          }
          free(pset_st);
          free(pset_sn);
          free(pset_fn);
          free(pset_leng);
          pset_sn=pset_sn2;
          pset_fn=pset_fn2;
          pset_leng=pset_leng2;
          pset_st=pset_st2;
        }
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
      if(all==1){
        vname[0] = 'a';
        vname[1] = 'l';
        vname[2] = 'l';
        vname[3] = '\0';
      }
      if( name_right == -1) exit(EXIT_FAILURE);
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
        if( more_mv_names == -1 ) exit(EXIT_FAILURE);
        else if( more_mv_names == 66){
          for(temp=0;mod_v_name[temp]!='\0';temp++)partition_lookup[j][1][temp] = mod_v_name[temp];
          partition_lookup[j][1][temp]='\0';
          ind = find_set(mod_v_name,lookup,set_leng);
          temp_summe =0;
          for(temp=1;temp<ind[0]+1;temp++)temp_summe+= pset_leng[ind[temp]];
          free(ind);
          intchar(new_pos,1,current_pos+1,1);
          ppart_fn[j]=current_pos+1;
          current_pos += temp_summe;
          ppart_sn[j]=current_pos;
          intchar(new_pos,1,current_pos,1);
          j++;
          if(j==part_leng){
            part_leng *= 2;  
            partition_lookup2 = (char***) malloc(sizeof(char**)*part_leng);
            if(partition_lookup2 == NULL) {
              fprintf(stderr, "Not enought virtual RAM could have been allocated.");
              exit(EXIT_FAILURE);
            }
            for(k=0;k<part_leng;k++){
              partition_lookup2[k] = (char**) malloc(sizeof(char*)*PART_TAB_DIM2);
              if(partition_lookup2[k] == NULL) {
                fprintf(stderr, "Not enought virtual RAM could have been allocated.");
                exit(EXIT_FAILURE);
              }
            }  
/* fprintf(stderr, "HALLO (%s:%d)\n", __FILE__, __LINE__); */
            for(k=0;k<part_leng;k++){
              for(temp=0;temp<PART_TAB_DIM2;temp++){
                partition_lookup2[k][temp]= (char*) malloc(sizeof(char)*PART_TAB_DIM3);
                if(partition_lookup2[k][temp] == NULL) {
                  fprintf(stderr, "Not enought virtual RAM could have been allocated.");
                  exit(EXIT_FAILURE);
                }
                if(k<(part_leng/2))
                  copy_str(&(partition_lookup[k][temp][0]),&(partition_lookup2[k][temp][0]));
              }
            }
            for(k=0;k<(part_leng/2);k++){
              for(temp=0;temp<PART_TAB_DIM2;temp++){
                free(partition_lookup[k][temp]);
              }
            }
            for(k=0;k<(part_leng/2);k++) free(partition_lookup[k]);
            free(partition_lookup);
            partition_lookup = partition_lookup2;
            ppart_fn2 =(size_t*) malloc(sizeof(size_t)*part_leng);
            ppart_sn2 =(size_t*) malloc(sizeof(size_t)*part_leng);
            for(k=0;k<(part_leng/2);k++) ppart_fn2[k]=ppart_fn[k];
            for(k=0;k<(part_leng/2);k++) ppart_sn2[k]=ppart_sn[k];
            free(ppart_fn);
            free(ppart_sn);
            ppart_fn=ppart_fn2;
            ppart_sn=ppart_sn2;
          }
          partition_counter++;
          more_mv_names = grepVName(datei,mod_v_name);
        }
        else if( more_mv_names == 77) {
          for(temp=0;mod_v_name[temp]!='\0';temp++)partition_lookup[j][1][temp] = mod_v_name[temp];
          partition_lookup[j][1][temp]='\0';
          ind = find_set(mod_v_name,lookup,set_leng);
          temp_summe =0;
          for(temp=1;temp<ind[0]+1;temp++)temp_summe+= pset_leng[ind[temp]];
          free(ind);
          intchar(new_pos,1,current_pos+1,1);
          ppart_fn[j]=current_pos+1;
          current_pos += temp_summe;
          ppart_sn[j]=current_pos;
          intchar(new_pos,1,current_pos,1);
          j++;
          if(j==part_leng){
            part_leng *= 2;  
            partition_lookup2 = (char***) malloc(sizeof(char**)*part_leng);
            if(partition_lookup2 == NULL) {
              fprintf(stderr, "Not enought virtual RAM could have been allocated.");
              exit(EXIT_FAILURE);
            }
            for(k=0;k<part_leng;k++){
              partition_lookup2[k] = (char**) malloc(sizeof(char*)*PART_TAB_DIM2);
              if(partition_lookup2[k] == NULL) {
                fprintf(stderr, "Not enought virtual RAM could have been allocated.");
                exit(EXIT_FAILURE);
              }
            }  
            for(k=0;k<part_leng;k++){
              for(temp=0;temp<PART_TAB_DIM2;temp++){
                partition_lookup2[k][temp]= (char*) malloc(sizeof(char)*PART_TAB_DIM3);
                if(partition_lookup2[k][temp] == NULL) {
                  fprintf(stderr, "Not enought virtual RAM could have been allocated.");
                  exit(EXIT_FAILURE);
                }
                if(k<(part_leng/2))copy_str(&(partition_lookup[k][temp][0]),&(partition_lookup2[k][temp][0]));
              }
            }
            for(k=0;k<(part_leng/2);k++){
              for(temp=0;temp<PART_TAB_DIM2;temp++){
                free(partition_lookup[k][temp]);
              }
            }
            for(k=0;k<(part_leng/2);k++) free(partition_lookup[k]);
            free(partition_lookup);
            partition_lookup = partition_lookup2;
            ppart_fn2 =(size_t*) malloc(sizeof(size_t)*part_leng);
            ppart_sn2 =(size_t*) malloc(sizeof(size_t)*part_leng);
            for(k=0;k<(part_leng/2);k++) ppart_fn2[k]=ppart_fn[k];
            for(k=0;k<(part_leng/2);k++) ppart_sn2[k]=ppart_sn[k];
            free(ppart_fn);
            free(ppart_sn);
            ppart_fn=ppart_fn2;
            ppart_sn=ppart_sn2;
          }
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
      
      ind = find_set(mod_v_name,lookup,set_leng);
      temp_summe =0;
      for(temp=1;temp<ind[0]+1;temp++)temp_summe+= pset_leng[ind[temp]];
      free(ind);
      intchar(new_pos,1,current_pos+1,1);
      ppart_fn[j]=current_pos+1;
      current_pos += temp_summe;
      ppart_sn[j]= current_pos;
      intchar(new_pos,1,current_pos,1);
      j++;
      if(j==part_leng){
        part_leng =2;  
        partition_lookup2 = (char***) malloc(sizeof(char**)*part_leng);
        if(partition_lookup2 == NULL) {
          fprintf(stderr, "Not enought virtual RAM could have been allocated.");
          exit(EXIT_FAILURE);
        }
        for(k=0;k<part_leng;k++){
          partition_lookup2[k] = (char**) malloc(sizeof(char*)*PART_TAB_DIM2);
          if(partition_lookup2[k] == NULL) {
            fprintf(stderr, "Not enought virtual RAM could have been allocated.");
            exit(EXIT_FAILURE);
          }
        }  
        for(k=0;k<part_leng;k++){
          for(temp=0;temp<PART_TAB_DIM2;temp++){
            partition_lookup2[k][temp]= (char*) malloc(sizeof(char)*PART_TAB_DIM3);
            if(partition_lookup2[k][temp] == NULL) {
              fprintf(stderr, "Not enought virtual RAM could have been allocated.");
              exit(EXIT_FAILURE);
            }
            if(k<(part_leng/2))copy_str(&(partition_lookup[k][temp][0]),&(partition_lookup2[k][temp][0]));
          }
        }
        for(k=0;k<(part_leng/2);k++){
          for(temp=0;temp<PART_TAB_DIM2;temp++){
            free(partition_lookup[k][temp]);
          }
        }
        for(k=0;k<(part_leng/2);k++) free(partition_lookup[k]);
        free(partition_lookup);
        partition_lookup = partition_lookup2;
        ppart_fn2 =(size_t*) malloc(sizeof(size_t)*part_leng);
        ppart_sn2 =(size_t*) malloc(sizeof(size_t)*part_leng);
        for(k=0;k<(part_leng/2);k++) ppart_fn2[k]=ppart_fn[k];
        for(k=0;k<(part_leng/2);k++) ppart_sn2[k]=ppart_sn[k];
        free(ppart_fn);
        free(ppart_sn);
        ppart_fn=ppart_fn2;
        ppart_sn=ppart_sn2;
      }
      partition_counter++;
      charpart_flag = 0;
    }
    else
      charpart_flag = 0;
  }
  if(end_counter != 4) {
    printf("No 'end;' for 'begin sets;' was found.\n");
    exit(EXIT_FAILURE);
  }
  ret.start = ppart_fn;
  ret.end = ppart_sn;
  ret.lookup = partition_lookup;
  pret=&ret;
  k = 0;
  for(i=0;i<counter;i++){
     printf("%s ",lookup[i]);
     printf("%lu...%lu...%lu...%lu",pset_fn[i],pset_sn[i],pset_st[i],pset_leng[i]);
     printf("\n");
  }
  for(i=0;i<partition_counter;i++){
    for(k=0;k<4;k++){
      printf("%s ",ret.lookup[i][k]);
     }
     printf("%lu...%lu",ret.start[i],ret.end[i]);
     printf("\n");
  }
  return pret;
}
int main(int argc, char **argv) {
  int all;
  FILE *datei;
  char *test;
  struct parsed_part *pstruct;
  pstruct=NULL;
  if(argc<2){
    printf("Please set a nexus file as first parameter\n E.g: \" rathian Filename \"\n");
    return 1;
  }
  test = 0;
  if(argc==3)
    test = argv[2];
  if(test!=NULL){
    if(test[0]=='a' && test[1]=='l' && test[2]=='l')
      all=1;
    else 
      all=0;
  }
  datei=fopen(argv[1],"rb");
  pstruct = do_something(datei,all);
  return 0;
}
