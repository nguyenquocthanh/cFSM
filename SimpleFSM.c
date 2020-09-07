
#include <stdio.h>
int entry_state(void);
int foo_state(void);
int bar_state(void);
int exit_state(void);

/* array and enum below must be in sync! */
int (* state[])(void) = { entry_state, foo_state, bar_state, exit_state};
enum state_codes { entry, foo, bar, end};

enum ret_codes { ok, fail, repeat};
struct transition {
    enum state_codes src_state;
    enum ret_codes   ret_code;
    enum state_codes dst_state;
};
/* transitions from end state aren't needed */
struct transition state_transitions[] = {
    {entry, ok,     foo},
    {entry, fail,   end},
    {foo,   ok,     bar},
    {foo,   fail,   end},
    {foo,   repeat, foo},
    {bar,   ok,     end},
    {bar,   fail,   end},
    {bar,   repeat, foo}
};

#define EXIT_STATE end
#define ENTRY_STATE entry
#define EXIT_SUCCESS 0
int entry_state(void) {
    printf("entry_state\r\n");
    return 0;
}
int foo_state(void) {
    printf("foo_state\r\n");
    static int count = 0;
    count++;
    if(count < 5) {
        return 2;
    }
    else {
        return 0;
    }
}
int bar_state(void) {
    printf("bar_state\r\n");
    return 0;
}
int exit_state(void) {
    printf("exit_state\r\n");
    return 0;
}


int lookup_transitions(enum state_codes cur_state,enum ret_codes rc) {
    enum state_codes ret = EXIT_STATE;
    
    int transition_len = sizeof(state_transitions)/sizeof(state_transitions[0]);
    
    for(int i = 0; i < transition_len; i++) {
        if(cur_state == state_transitions[i].src_state && rc == state_transitions[i].ret_code) {
            ret = state_transitions[i].dst_state;
        }
    }
    
    return ret;
}
int main(int argc, char *argv[]) {
    enum state_codes cur_state = ENTRY_STATE;
    enum ret_codes rc;

    int (* state_fun)(void);

    for (;;) {
        state_fun = state[cur_state];
        rc = state_fun();
        if (EXIT_STATE == cur_state)
        {
            break;
        }
        cur_state = lookup_transitions(cur_state, rc);
    }

    return EXIT_SUCCESS;
}
