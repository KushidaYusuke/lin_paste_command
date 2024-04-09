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
  for(int i = 2; i < argc; i++) {
    file_list[i-2] = argv[i];
    is_file_end_list[i-2] = false;
    file_list_pointer[i-2] = fopen(file_list[i-2], "r"); 
  }

  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  int now_index = 0;
  //char *colom_list = malloc(INIT_ALLOC*sizeof(char));
  //int now_alloc_num = INIT_ALLOC;
  for(int i = 0; i < argc-2; i++) {
    FILE *file = file_list_pointer[i];
    while(1) {
      int c;
      c = fgetc(file);
      if(c == EOF) {
        putchar('\n');
	break;
      }
      if(c == '\n') {
        putchar('\t');
        continue;
      }
      putchar(c);
    }
  }
}
