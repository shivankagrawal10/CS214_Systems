typedef struct {
  size_t length;
  size_t used;
  char *data;
} strbuf_t;

int len(char*);
int sb_init(strbuf_t *, size_t);
int sb_comp(strbuf_t*,strbuf_t*);
void sb_destroy(strbuf_t *);
int sb_append(strbuf_t *, int);
int sb_remove(strbuf_t *, char *);
int sb_insert(strbuf_t *, int, char);
int sb_concat(strbuf_t *,char *);
strbuf_t* sb_expand(strbuf_t *, int);
strbuf_t* update_null(strbuf_t *);
strbuf_t* remove_null(strbuf_t *);
void sb_print(strbuf_t *);
