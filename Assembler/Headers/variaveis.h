#define NVARMAX 999999

extern int  v_val [NVARMAX];

int  var_find  (char *val);
void  var_add  (char *var, int val);
void  inc_vcont(int   val);
void  var_reset();