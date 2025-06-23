# ifndef SIGNAL_H

# define SIGNAL_H

void setup_sigint(void);
void setup_sigquit(void);
void handle_sigint(int sig);

# endif