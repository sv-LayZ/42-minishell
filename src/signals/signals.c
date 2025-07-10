#include "minishell.h" // <signal.h>
volatile sig_atomic_t g_interrupted = 0;

//ctrl-c : nouvelle ligne ok
void handle_sigint(int sig)
{
    (void)sig;
    
    ft_putchar_fd('\n', 1);// Print a newline to the terminal
    rl_on_new_line();
    rl_replace_line("", 0); 
    rl_redisplay();
    g_interrupted = 1; // Set the flag to indicate interruption
}
//selon le signal ok
void setup_sigint(void)
{
    struct sigaction sa_int;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART; // = 0 // Restart system calls if interrupted
    sigaction(SIGINT, &sa_int, NULL);
}

void setup_sigquit(void)
{
   struct sigaction sa_quit;

    sa_quit.sa_handler = SIG_IGN; // Ignore SIGQUIT
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0; // No special flags
    sigaction(SIGQUIT, &sa_quit, NULL); //ctrl-\: ignoré
}

void handle_signals(void) //ok
{
    setup_sigint(); // Set up handler for SIGINT (Ctrl-C)
    setup_sigquit(); // Set up handler for SIGQUIT (Ctrl-\)
}