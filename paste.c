#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<unistd.h>
#define INIT_ALLOC 3
#define ADD_ALLOC 10

int dopt = 0;
int sopt = 0;
char *dparam = NULL;

bool memory_allocate_error = false; //動的配列でのメモリ割り当てが失敗した場合にtrueを返す
/*
オプションが指定されていない場合
引数 
file_num: ファイルの個数 
is_file_end_list: それぞれのファイルについて、ファイルの終端までたどりついている場合にtruwを返す配列	
file_list_pointer: それぞれのファイルへのポインタを格納した配列  
*/
void paste(int file_num, bool *is_file_end_list, FILE **file_list_pointer) {
  char delim = '\t'; //デフォルトではタブ区切り	
  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  int now_index = 0;//現在見ている文字が行頭から何番目か
  char *tmp_row = malloc(INIT_ALLOC*sizeof(char)); //読み込んだ文字を一時的に保管しておく
  if(tmp_row==NULL) {
    memory_allocate_error = true;
    return;
  }
  int now_alloc_num = INIT_ALLOC;//現在割り当てられているメモリの要素数
  
  //すべてのファイルで終端までたどり着くまでループする
  while(1) {
    for(int i = 0; i < file_num; i++) {
      //メモリが不足した場合は再割り当て
      if(now_index >= now_alloc_num) {
	char *tmp = realloc(tmp_row, (i+ADD_ALLOC)*sizeof(char));
	if(tmp == NULL) {
	  memory_allocate_error = true;
	  free(tmp);
	  free(tmp_row);
	  return;
	}
	else {
	  tmp_row = tmp;
	  now_alloc_num += ADD_ALLOC;
	}
      }
      //現在見ているファイルがすでに終端までたどり着いている場合は単に区切り文字を出力	
      if(is_file_end_list[i]) {
        tmp_row[now_index] = delim;
	now_index += 1;
	continue;
      }
      FILE *file = file_list_pointer[i]; //現在見ているファイルへのポインタ	
      int c;
      while(1) {
	c = fgetc(file);
	if(c == '\n') {
	  //各行の右端以外は区切り文字で区切る
	  if(i != file_num-1) {
	    tmp_row[now_index] = delim;
	    now_index += 1;
          }
	  //is_all_file_end = false;
	  file_list_pointer[i] = file; //中断した時点でのポインタを格納
	  break;
	}
	if(c == EOF) {
	  is_file_end_list[i] = true;
	  if(i != file_num - 1) {
	    tmp_row[now_index] = delim;
	    now_index += 1;
	  }
	  break;
	}
        is_all_file_end = false;
	tmp_row[now_index] = c;
	now_index += 1;
      }
    }
    //すべてのファイルで終端までたどり着いたら終了
    if(is_all_file_end) {
      return;
    }
    //実際に出力をするのはそれぞれの行について、すべてのファイルの読み込みが終わった時点
    for(int i = 0; i < now_index; i++) {
      putchar(tmp_row[i]);
    }
    free(tmp_row);
    tmp_row = malloc(INIT_ALLOC*sizeof(char));
    if(tmp_row == NULL) {
      memory_allocate_error = true;
      return;
    }
    now_index = 0;
    now_alloc_num = INIT_ALLOC;
    putchar('\n');
    is_all_file_end = true;
  }
}

//d_param_num: -dオプションの引数として渡される区切り文字の個数
void paste_option_d(int file_num, bool *is_file_end_list, FILE **file_list_pointer, int d_param_num) {
  bool is_all_file_end = true;//すべてのファイルが終端に達したかどうかを管理
  int now_index = 0;
  int dparam_index = 0; //出力する区切り文字のインデックス 
  char *tmp_row = malloc(INIT_ALLOC*sizeof(char));
  if(tmp_row == NULL) {
    memory_allocate_error = true;
    return;
  }

  int now_alloc_num = INIT_ALLOC;
  while(1) {
    for(int i = 0; i < file_num; i++) {
      if(now_index >= now_alloc_num) {
	char *tmp = realloc(tmp_row, (i+ADD_ALLOC)*sizeof(char));
	if(tmp == NULL) {
	  memory_allocate_error = true;
	  free(tmp);
	  free(tmp_row);
	  return;
	}
	else {
	  tmp_row = tmp;
          now_alloc_num += ADD_ALLOC;
	}
      }

      if(is_file_end_list[i]) {
	dparam_index %= d_param_num;
        tmp_row[now_index] = dparam[dparam_index];
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
	    dparam_index %= d_param_num;
	    tmp_row[now_index] = dparam[dparam_index];
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
	    dparam_index %= d_param_num;
	    tmp_row[now_index] = dparam[dparam_index];
	    dparam_index += 1;
	    now_index += 1;
	  }
	  break;
	}
        is_all_file_end = false;
	tmp_row[now_index] = c;
	now_index += 1;
      }
    }
    if(is_all_file_end) {
      return;
    }
    for(int i = 0; i < now_index; i++) {
      putchar(tmp_row[i]);
    }
    free(tmp_row);
    tmp_row = malloc(INIT_ALLOC*sizeof(char));
    if(tmp_row == NULL) {
      memory_allocate_error = true;
      return;
    }
    now_index = 0;
    dparam_index = 0;
    now_alloc_num = INIT_ALLOC;
    putchar('\n');
    is_all_file_end = true;
  }

}


//-sオプションのみが指定された場合
//各ファイルへのポインタを引数にとって、タブ区切りで順にファイルの内容を出力する
void paste_option_s(FILE *file) {
  int now_index = 0;
  char *tmp_row = malloc(INIT_ALLOC*sizeof(char));
  if(tmp_row == NULL) {
    memory_allocate_error = true;
    return;
  }
  int now_alloc_size = INIT_ALLOC;
  int c;
  while(1) {
    if(now_index >= now_alloc_size) {
      char *tmp = realloc(tmp_row, (now_index + ADD_ALLOC)*sizeof(char));
      if(tmp == NULL) {
        free(tmp);
	free(tmp_row);
	memory_allocate_error = true;
	return;
      }
      else {
	tmp_row = tmp;
        now_alloc_size += ADD_ALLOC;
      }
    }

    c = fgetc(file);
    if(c == '\n') {
      tmp_row[now_index] = '\t';
      now_index += 1;
      continue;
    }
    if(c == EOF) {
      for(int i = 0; i < now_index-1; i++) {
        putchar(tmp_row[i]);
      }
      putchar('\n');
      free(tmp_row);
      return;
    }
    tmp_row[now_index] = c;
    now_index += 1;
  }
}

//-sオプションと-dオプションが共に指定された場合
void paste_option_s_with_d(FILE *file, int d_param_num) {
  int now_index = 0;
  char *tmp_row = malloc(INIT_ALLOC*sizeof(char));
  if(tmp_row == NULL) {
    memory_allocate_error = true;
    return;
  }
  int now_alloc_size = INIT_ALLOC;
  int delim_count = 0;
  int c;
  bool is_before_n = false;
  while(1) {
    if(now_index >= now_alloc_size) {
      char *tmp = realloc(tmp_row, (now_index+ADD_ALLOC)*sizeof(char));
      if(tmp == NULL) {
        free(tmp);
	free(tmp_row);
	memory_allocate_error = true;
	return;
      }
      else {
        tmp_row = tmp;
        now_alloc_size += ADD_ALLOC;
      }
    }

    c = fgetc(file);
    if(c == EOF) {
      for(int i = 0; i < now_index-1; i++) {
        putchar(tmp_row[i]);
      }
      putchar('\n');
      return;
    }
    if(c == '\n') {
      tmp_row[now_index] = dparam[delim_count%d_param_num]; 
      now_index += 1;
      delim_count += 1; 
      continue;
    }

    tmp_row[now_index] = c;
    now_index += 1;
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
      default:
        fprintf(stderr, "paste: invalid option -- %c \nTry 'paste --help' for more information.\n", optarg[0]);
	exit(1);
    }
  }


  int file_num = argc - optind;//ファイルの個数	
  bool is_file_end_list[file_num];//それぞれのファイルについて終端までたどり着いたかを表す
  FILE *file_list_pointer[file_num];//それぞれのファイルへのポインタ

  //-dオプションで指定された区切り文字の個数をカウントする
  int d_param_num = 0;
  if(dopt) { 
    while(dparam[d_param_num] != '\0') {
      d_param_num += 1;
    }
  }



  if(sopt) {
      //-sオプションのみが指定された場合の処理
    if(!dopt) {
      for(int i = optind; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if(file == NULL) {
          fprintf(stderr, "paste: %s: No such file or directory\n", argv[i]);
          exit(1);
        }
	paste_option_s(file);
	fclose(file);
	if(memory_allocate_error) {
	  printf("memory allocation failed\n");
	  exit(1);
	}
      }
    }
    //-s,-dオプションがともに指定された場合の処理
    else {
      for(int i = optind; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
	if(file == NULL) {
          fprintf(stderr, "paste: %s: No such file or directory\n", argv[i]);
	  exit(1);
	}
        paste_option_s_with_d(file, d_param_num);
	fclose(file);
	if(memory_allocate_error) {
	  printf("memory allocation failed\n");
	  exit(1);
	}
      }
    }
  }
  //-sオプションが指定されていない場合
  else{
    for(int i = optind; i < argc; i++) {
      is_file_end_list[i-optind] = false;
      FILE *file = fopen(argv[i], "r");
      if(file == NULL) {
        fprintf(stderr, "paste: %s: No such file or directory\n", argv[i]);
        exit(1);
      }
      file_list_pointer[i-optind] = file; 
    }
    //-dオプションのみが指定された場合の処理
    if(dopt) paste_option_d(file_num, is_file_end_list, file_list_pointer, d_param_num);
    //オプションが指定されていない場合の処理
    else paste(file_num, is_file_end_list, file_list_pointer);
    
    for(int i = 0; i < file_num; i++) {
      FILE *file = file_list_pointer[i];
      fclose(file);
    } 
    if(memory_allocate_error) {
      printf("memory allocation failed\n");
      exit(1);
    }
  }

}
