#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<unistd.h>
#define INIT_ALLOC 3
#define ADD_ALLOC 10

int dopt = 0;
int sopt = 0;
char *dparam = NULL;



void paste(int file_num, char **file_list, bool *is_file_end_list, FILE **file_list_pointer) {
  char delim = '\t'; //まずは区切り文字が一つのみの場合を考える
  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  int now_index = 0;
  char *colom_list = malloc(INIT_ALLOC*sizeof(char));
  int now_alloc_num = INIT_ALLOC;
  while(1) {
    for(int i = 0; i < file_num; i++) {
      if(now_index >= now_alloc_num) colom_list = realloc(colom_list, (i+ADD_ALLOC)*sizeof(char));
      if(is_file_end_list[i]) {
        colom_list[now_index] = delim;
	now_index += 1;
	continue;
      }
      FILE *file = file_list_pointer[i];
      int c;
      while(1) {
	c = fgetc(file);
	if(c == '\n') {
	  if(i != file_num-1) {
	    colom_list[now_index] = delim;
	    now_index += 1;
          }
	  //is_all_file_end = false;
	  file_list_pointer[i] = file; //中断した時点でのポインタを格納
	  break;
	}
	if(c == EOF) {
	  is_file_end_list[i] = true;
	  if(i != file_num - 1) {
	    colom_list[now_index] = delim;
	    now_index += 1;
	  }
	  break;
	}
        is_all_file_end = false;
	colom_list[now_index] = c;
	now_index += 1;
      }
    }
    if(is_all_file_end) {
      return;
    }
    for(int i = 0; i < now_index; i++) {
      putchar(colom_list[i]);
    }
    free(colom_list);
    colom_list = malloc(INIT_ALLOC*sizeof(char));
    now_index = 0;
    now_alloc_num = INIT_ALLOC;
    putchar('\n');
    is_all_file_end = true;
  }
}

void paste_option_d(int file_num, char **file_list, bool *is_file_end_list, FILE **file_list_pointer, int d_param_num) {
  //char delim = dparam[0]; //まずは区切り文字が一つのみの場合を考える
  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  int now_index = 0;
  int dparam_index = 0;
  char *colom_list = malloc(INIT_ALLOC*sizeof(char));
  int now_alloc_num = INIT_ALLOC;
  while(1) {
    for(int i = 0; i < file_num; i++) {
      if(now_index >= now_alloc_num) colom_list = realloc(colom_list, (i+ADD_ALLOC)*sizeof(char));
      if(is_file_end_list[i]) {
        colom_list[now_index] = dparam[dparam_index%d_param_num];
	dparam_index += 1;
	now_index += 1;
	continue;
      }
      FILE *file = file_list_pointer[i];
      int c;
      while(1) {
	c = fgetc(file);
	if(c == '\n') {
	  if(i != file_num-1) {
	    colom_list[now_index] = dparam[dparam_index%d_param_num];
	    dparam_index += 1;
	    now_index += 1;
          }
	  //is_all_file_end = false;
	  file_list_pointer[i] = file; //中断した時点でのポインタを格納
	  break;
	}
	if(c == EOF) {
	  is_file_end_list[i] = true;
	  if(i != file_num - 1) {
	    colom_list[now_index] = dparam[dparam_index%d_param_num];
	    dparam_index += 1;
	    now_index += 1;
	  }
	  break;
	}
        is_all_file_end = false;
	colom_list[now_index] = c;
	now_index += 1;
      }
    }
    if(is_all_file_end) {
      return;
    }
    for(int i = 0; i < now_index; i++) {
      putchar(colom_list[i]);
    }
    free(colom_list);
    colom_list = malloc(INIT_ALLOC*sizeof(char));
    now_index = 0;
    dparam_index = 0;
    now_alloc_num = INIT_ALLOC;
    putchar('\n');
    is_all_file_end = true;
  }

}

int main(int argc, char *argv[]) {
  //オプションの処理
  int opt;
  while((opt = getopt(argc, argv, "d:s")) != -1) {
    switch(opt) {
      case 'd':
        dopt = 1;
	dparam = optarg;
	break;
      case 's':
        sopt = 1;
        break;
    }
  }

  int file_num = argc - optind;
  char *file_list[file_num];
  bool is_file_end_list[file_num];
  FILE *file_list_pointer[file_num];
  for(int i = optind; i < argc; i++) {
    file_list[i-optind] = argv[i];
    is_file_end_list[i-optind] = false;
    file_list_pointer[i-optind] = fopen(file_list[i-optind], "r"); 
  }
 
  if(dopt) {
    int d_param_num = 0;
    while(dparam[d_param_num] != '\0') {
      d_param_num += 1;
    }
    paste_option_d(file_num, file_list, is_file_end_list, file_list_pointer, d_param_num);
  }
  else paste(file_num, file_list, is_file_end_list, file_list_pointer);
 

 // for(int i = 0; i < file_num; i++) {
   // printf("%s", file_list[i]);
  //}
  
//  int i = 0;
  //while(dparam[i] != '\0') {
    //printf("%c\n", dparam[i]);
    //i++;
  //}
}
