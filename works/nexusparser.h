#ifndef NEXUSPARSER__H
#define NEXUSPARSER_H

#define SET_LENG 10
#define SET_TAB_DIM3 20
#define PART_TAB_DIM2 4
#define PART_TAB_DIM3 200
#define PART_LENG 10
#define STR_LEN 10

typedef struct {
  char** model_names;
  char** parameter_names;
  char** partition_names;
  char** gene_names;
  size_t part_len;
  size_t* start;
  size_t* end;
  size_t max_part_len;
   
} partition_t;

void show_help(char* arg);
char* intchar(int n);
int mygetc(char* str,int str_len,FILE* file,int* c_read);
void protoss_build_nexus(char* f_name,partition_t* nexus,int pflag,int cflag);
void expand_part_table(char ***ppart_name,char ***ppart_var_name,char ***ppart_model_name,char ***ppart_parameter_name,size_t partition_counter,size_t part_leng,size_t** ppart_fn,size_t** ppart_sn);
void expand_set_table(size_t set_leng,size_t current_set_len,char ***lookup,size_t **pset_leng,size_t **pset_fn,size_t **pset_sn,size_t **pset_st);
int charint(char* temp);
int* find_set(char* set_name,char** look_up,int len);
int grepName(int c,FILE* datei,char* vname,char* str,int str_len,int* c_read);
int grepFirstNumber(int* ret,int c,FILE* datei,char* vname,char* str,int str_len,int* c_read);
int grepSecondNumber(int c,FILE* datei, char* vname,char* str,int str_len,int* c_read);
void grepSteps(int* ret,int c,FILE* datei, char* vname,char* str,int str_len,int* c_read);
void grepModName(int c,FILE* datei, char* vname, char* pname,int* ret,char* str,int str_len,int* c_read);
void grepVName(int c,FILE* datei, char* vname, int* ret,char* str,int str_len,int* c_read);
partition_t parse_partition(int argc,char **argv);
void destroy(partition_t* temp);
void part_print(partition_t* partition_table,int paras_flag);
#endif /* NEXUSPARSER_H */
