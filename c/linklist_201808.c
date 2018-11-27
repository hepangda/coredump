#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define ll_type(x) linklist_##x

typedef struct file {
  int fid;
  char filename[128];
  int mode;
  int size;
  char data[256];
} file;

typedef struct file_node {
  file *data_;
  struct file_node *next_;
} * ll_type(file);

typedef void ll_function(ll_type(file));
typedef void ll_function_witharg(ll_type(file), void *);
#define ll_declare_function(name, paramname) void name(ll_type(file) paramname)
#define ll_declare_function_witharg(name, paramname1, paramname2) \
  void name(ll_type(file) paramname1, void *paramname2)

ll_type(file) ll_create() {
  linklist_file res = malloc(sizeof(struct file_node));
  res->data_ = NULL;
  res->next_ = NULL;

  return res;
}

ll_type(file) ll_next(ll_type(file) p) { return p->next_; }

file ll_data(ll_type(file) p) { return *(p->data_); }
file *ll_datap(ll_type(file) p) { return p->data_; }

void ll_foreach(ll_type(file) head, ll_function func) {
  while ((head = ll_next(head)) != NULL) {
    func(head);
  }
}

void ll_foreach_witharg(ll_type(file) head, ll_function_witharg func,
                        void *arg) {
  while ((head = ll_next(head)) != NULL) {
    func(head, arg);
  }
}

void ll_destory(ll_type(file) head) {
  ll_type(file) p = head, t;
  while ((t = ll_next(p)) != NULL) {
    ll_type(file) t = p;
    free(t->data_);
    free(t);
    p = t;
  }
}

void ll_append_back(ll_type(file) * tail, int fid, const char *filename,
                    int mode, int size, const char *data) {
  file *new_data = malloc(sizeof(file));
  new_data->fid = fid;
  strcpy(new_data->filename, filename);
  new_data->mode = mode;
  new_data->size = size;
  memcpy(new_data->data, data, 256);

  ll_type(file) new_node = malloc(sizeof(struct file_node));
  new_node->next_ = NULL;
  new_node->data_ = new_data;

  (*tail)->next_ = new_node;
  *tail = new_node;
}

void ll_append_vtype(ll_type(file) * tail, file buffer) {
  file *new_data = malloc(sizeof(file));
  memcpy(new_data, &buffer, sizeof(buffer));

  ll_type(file) new_node = malloc(sizeof(struct file_node));
  new_node->next_ = NULL;
  new_node->data_ = new_data;

  (*tail)->next_ = new_node;
  *tail = new_node;
}

void ll_write_fucntion(ll_type(file) p) {}

ll_declare_function_witharg(ll_write_file_cb, p, pfd) {
  int fd = *(int *)pfd;
  write(fd, ll_datap(p), sizeof(file));
}

void ll_write_file(const char *filename, ll_type(file) head) {
  int fd = open(filename, O_WRONLY | O_CREAT);
  assert(fd >= 0);
  ll_foreach_witharg(head, ll_write_file_cb, &fd);
  close(fd);
}

ll_type(file) ll_create_from_file(const char *filename) {
  int fd = open(filename, O_RDONLY);
  assert(fd >= 0);
  int readed = -1;

  ll_type(file) head = ll_create(), tail = head;
  file buffer;
  while ((readed = read(fd, &buffer, sizeof(buffer))) > 0) {
    ll_append_vtype(&tail, buffer);
  }
  close(fd);

  return head;
}

ll_declare_function(print, x) {
  printf("%d %s\n", ll_data(x).fid, ll_data(x).filename);
}

int main(int argc, char const *argv[]) {
  DIR *dir_lib = opendir("/lib");
  assert(dir_lib != NULL);

  ll_type(file) head = ll_create(), tail = head;
  char name_buffer[281], data_buffer[256];
  int last_fid = 0;
  for (struct dirent *p = NULL; p = readdir(dir_lib), p != NULL;) {
    if (p->d_type & DT_REG) {
      sprintf(name_buffer, "/lib/%s", p->d_name);

      int fd = open(name_buffer, O_RDONLY);
      assert(fd >= 0);

      struct stat file_stat;
      int stat_code = fstat(fd, &file_stat);
      assert(stat_code == 0);

      if (S_ISDIR(file_stat.st_mode)) {
        close(fd);
        continue;
      }

      int read_code = read(fd, data_buffer, 256);
      assert(read_code >= 0);

      ll_append_back(&tail, ++last_fid, p->d_name, file_stat.st_mode,
                     file_stat.st_size, data_buffer);
      close(fd);
    }
  }
  ll_write_file("alldata", head);
  // ll_foreach(head, &print);
  closedir(dir_lib);
  return 0;
}
