#include<stdio.h>
#include<stdbool.h>
int main(int argc, char *argv[]) {
  char *file_list[argc];
  bool is_file_end_list[argc];
  FILE *file_list_pointer[argc];
  for(int i = 1; i < argc; i++) {
    file_list[i-1] = argv[i];
    is_file_end_list[i-1] = false;
    file_list_pointer[i-1] = fopen(file_list[i-1], "r"); 
  }

  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  while(1) {
    for(int i = 0; i < argc-1; i++) {
      if(is_file_end_list[i]) {
	putchar('\t');
	continue;
      }
      is_all_file_end = false;
      FILE *file = file_list_pointer[i];
      int c;
      while(1) {
	c = fgetc(file);
	if(c == '\n') {
	  putchar('\t');
	  file_list_pointer[i] = file; //中断した時点でのポインタを格納
	  break;
	}
	if(c == EOF) {
	  is_file_end_list[i] = true;
	  putchar('\t');
	  break;
	}

	putchar(c);
      }
    }
    putchar('\n');
    if(is_all_file_end) {
      return 0;
    }
    is_all_file_end = true;
  }

}
