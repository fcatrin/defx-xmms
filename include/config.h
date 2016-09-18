
/* Valid config operations */
#define CFG_READ 0
#define CFG_WRITE 1

/* Intiialize configuration module */
int cfg_init(int mode,gchar *section);

/* Configuration module cleanup */
void cfg_done();

/* Write a configuration value */
void cfg_write(const gchar *key, gint value);

/* Read a configuration value */
gboolean cfg_read(const gchar *key, gint def);

