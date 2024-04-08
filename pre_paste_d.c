#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#define INIT_ALLOC 3
#define ADD_ALLOC 10


int main(int argc, char *argv[]) {
  char delim = ',';
  char *file_list[argc];
  bool is_file_end_list[argc];
  FILE *file_list_pointer[argc];
  for(int i = 1; i < argc; i++) {
    file_list[i-1] = argv[i];
    is_file_end_list[i-1] = false;
    file_list_pointer[i-1] = fopen(file_list[i-1], "r"); 
  }

  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  int now_index = 0;
  char *colom_list = malloc(INIT_ALLOC*sizeof(char));
  int now_alloc_num = INIT_ALLOC;
  while(1) {
    for(int i = 0; i < argc-1; i++) {
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
	  if(i != argc-2) {
	    colom_list[now_index] = delim;
	    now_index += 1;
          }
	  //is_all_file_end = false;
	  file_list_pointer[i] = file; //中断した時点でのポインタを格納
	  break;
	}
	if(c == EOF) {
	  is_file_end_list[i] = true;
	  if(i !=argc-2) {
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
      return 0;
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
