/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * A class for reliably starting/stopping subprocesses.
 *
 * We want to avoid calling system(), since it uses the shell (and thus has
 * strange parsing weirdness, environment variable changes, and so on).  Plus
 * calling the shell when we need to is just slow.
 * 
 * On the other hand, we want handy features like the ability to wait for our
 * child process to die, and the ability to kill it if it doesn't (without
 * having to use "killall").
 * 
 * By using setsid(), we also deal with strange situations like scripts which
 * launch other programs.  stop() and kill() will kill them all. (If you don't
 * want that, use stop_primary() and kill_primary().)
 */
#ifndef __WVSUBPROC_H
#define __WVSUBPROC_H

#include "wvstringlist.h"

#include <signal.h>
#include <time.h>

class WvSubProc
{
public:
    pid_t pid;
    bool running;
    int estatus;
    WvStringList env;
    
    WvSubProc();

    WvSubProc(const char cmd[], const char * const *argv) {
	startv(cmd, argv);
    }

    virtual ~WvSubProc();
    
    // launch a subprocess, which will be owned by this object.
    int start(const char cmd[], ...);
    int startv(const char cmd[], const char * const *argv);
    
    // stop (kill -TERM or -KILL as necessary) the subprocess and
    // all its children.
    void stop(time_t msec_delay, bool kill_children = true);
    
    // wait for the subprocess (and all its children) to die.
    void wait(time_t msec_delay, bool wait_children = true);
    
    // send a signal to the subprocess and all its children.
    void kill(int sig);
    
    // send a signal only to the main subprocess.
    void kill_primary(int sig);
    
    // suspend the process temporarily, or resume it.
    void suspend()
        { kill(SIGSTOP); }
    void resume()
        { kill(SIGCONT); }
};

DeclareWvList(WvSubProc);

#endif // __WVSUBPROC_H
