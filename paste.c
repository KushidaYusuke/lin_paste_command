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
  char *tmp_colomn = malloc(INIT_ALLOC*sizeof(char));
  int now_alloc_num = INIT_ALLOC;
  while(1) {
    for(int i = 0; i < file_num; i++) {
      if(now_index >= now_alloc_num) tmp_colomn = realloc(tmp_colomn, (i+ADD_ALLOC)*sizeof(char));
      if(is_file_end_list[i]) {
        tmp_colomn[now_index] = delim;
	now_index += 1;
	continue;
      }
      FILE *file = file_list_pointer[i];
      int c;
      while(1) {
	c = fgetc(file);
	if(c == '\n') {
	  if(i != file_num-1) {
	    tmp_colomn[now_index] = delim;
	    now_index += 1;
          }
	  //is_all_file_end = false;
	  file_list_pointer[i] = file; //中断した時点でのポインタを格納
	  break;
	}
	if(c == EOF) {
	  is_file_end_list[i] = true;
	  if(i != file_num - 1) {
	    tmp_colomn[now_index] = delim;
	    now_index += 1;
	  }
	  break;
	}
        is_all_file_end = false;
	tmp_colomn[now_index] = c;
	now_index += 1;
      }
    }
    if(is_all_file_end) {
      return;
    }
    for(int i = 0; i < now_index; i++) {
      putchar(tmp_colomn[i]);
    }
    free(tmp_colomn);
    tmp_colomn = malloc(INIT_ALLOC*sizeof(char));
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
  char *tmp_colomn = malloc(INIT_ALLOC*sizeof(char));
  int now_alloc_num = INIT_ALLOC;
  while(1) {
    for(int i = 0; i < file_num; i++) {
      if(now_index >= now_alloc_num) tmp_colomn = realloc(tmp_colomn, (i+ADD_ALLOC)*sizeof(char));
      if(is_file_end_list[i]) {
        tmp_colomn[now_index] = dparam[dparam_index%d_param_num];
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
	    tmp_colomn[now_index] = dparam[dparam_index%d_param_num];
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
	    tmp_colomn[now_index] = dparam[dparam_index%d_param_num];
	    dparam_index += 1;
	    now_index += 1;
	  }
	  break;
	}
        is_all_file_end = false;
	tmp_colomn[now_index] = c;
	now_index += 1;
      }
    }
    if(is_all_file_end) {
      return;
    }
    for(int i = 0; i < now_index; i++) {
      putchar(tmp_colomn[i]);
    }
    free(tmp_colomn);
    tmp_colomn = malloc(INIT_ALLOC*sizeof(char));
    now_index = 0;
    dparam_index = 0;
    now_alloc_num = INIT_ALLOC;
    putchar('\n');
    is_all_file_end = true;
  }

}

void paste_option_s(FILE *file) {
  int now_index = 0;
  char *tmp_column = malloc(INIT_ALLOC*sizeof(int));
  int now_alloc_size = INIT_ALLOC;
  int c;
  while(1) {
    if(now_index >= now_alloc_size) tmp_column = realloc(tmp_column, (now_index + ADD_ALLOC)*sizeof(char));
    now_alloc_size += ADD_ALLOC; 
    c = fgetc(file);
    if(c == '\n') {
      tmp_column[now_index] = '\t';
      now_index += 1;
      continue;
    }
    if(c == EOF) {
      for(int i = 0; i < now_index-1; i++) {
        putchar(tmp_column[i]);
      }
      putchar('\n');
      free(tmp_column);
      return;
    }
    tmp_column[now_index] = c;
    now_index += 1;
  }
}


void paste_option_s_with_d(FILE *file, int d_param_num) {
  int delim_count = 0;
  int c;
  bool is_before_n = false;
  while(1) {
    c = fgetc(file);
    if(c == EOF) {
      putchar('\n');
      return;
    }
    if(c == '\n') {
      is_before_n = true;
      putchar(dparam[delim_count%d_param_num]);
      delim_count += 1;
      continue;
    }
    putchar(c);
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


  int d_param_num = 0;
  if(dopt) { 
    while(dparam[d_param_num] != '\0') {
      d_param_num += 1;
    }
  }



  if(sopt) {
      //-dオプションが指定されない場合の処理
    if(!dopt) {
      for(int i = optind; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if(file == NULL) {
          fprintf(stderr, "ファイルを開くことができませんでした\n");
          exit(1);
        }
	paste_option_s(file);
	fclose(file);
      }
    }
    //-dオプションが指定された場合の処理
    else {
      for(int i = optind; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
	if(file == NULL) {
	  fprintf(stderr, "ファイルを開くことができませんでした\n");
	  exit(1);
	}
        paste_option_s_with_d(file, d_param_num);
	putchar('\b');
	fclose(file);
      }
    }
  }
  else{
    for(int i = optind; i < argc; i++) {
    file_list[i-optind] = argv[i];
    is_file_end_list[i-optind] = false;
    file_list_pointer[i-optind] = fopen(file_list[i-optind], "r"); 
    }
 
    if(dopt) paste_option_d(file_num, file_list, is_file_end_list, file_list_pointer, d_param_num);
    else paste(file_num, file_list, is_file_end_list, file_list_pointer);
 
  }

 // for(int i = 0; i < file_num; i++) {
   // printf("%s", file_list[i]);
  //}
  
//  int i = 0;
  //while(dparam[i] != '\0') {
    //printf("%c\n", dparam[i]);
    //i++;
  //}
}
