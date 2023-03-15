#include <unistd.h>

#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

bool vflag = false;  // verbose
bool tflag = false;
bool eflag = false;
bool pflag = false;
bool iflag = false;

typedef enum { CREATED, READY, RUNNG, PREEMPT, BLOCKED, DONE } process_state;
const static char* state_str[] = {"CREATED", "READY", "RUNNG",
                                  "PREEMPT", "BLOCK", "DONE"};
const static char* no_yes_str[] = {"NO", "YES"};

const static unordered_map<char, string> shed_name = {
    {'F', "FCFS"}, {'L', "LCFS"}, {'S', "SRTF"},
    {'R', "RR"},   {'P', "PRIO"}, {'E', "PREPRIO"}};

int CURRENT_TIME = 0;
bool CALL_SCHEDULER = false;
int maxprio = 4;
int quantum = 10000;
char sched_type = 'F';

// --- begin --- iobusy ---

int time_iobusy = 0;
int global_io_end_t = -1;
void update_global_io(int start_t, int end_t) {
    if (start_t >= global_io_end_t) {
        time_iobusy += end_t - start_t;
        global_io_end_t = end_t;
    } else if (end_t > global_io_end_t) {
        time_iobusy += end_t - global_io_end_t;
        global_io_end_t = end_t;
    }
};

// --- end --- iobusy ---

// --- begin --- randvals ---

int ofs = 0;
int ofs_size = 0;

vector<int> randvals;

void init_randvals(string& rfile_name) {
    ifstream rfile;
    rfile.open(rfile_name);
    if (!rfile.is_open()) {
        cout << "Unable to open rand_file." << endl;
        exit(1);
    }
    int num;
    int pos = 0;
    bool is_first = true;
    while (rfile >> num) {
        if (is_first == true) {
            is_first = false;
            ofs_size = num;
            randvals.resize(ofs_size);
        } else {
            randvals[pos++] = num;
        }
    }
    rfile.close();
};

int myrandom(int burst) {
    if (ofs == ofs_size) {
        ofs = 0;
    }
    return 1 + (randvals[ofs++] % burst);
}

// --- end --- randvals ---

// --- begin --- process ---

int pid_num = 0;

class Process {
   public:
    Process(int pid, int at, int tc, int cb, int io)
        : pid(pid), at(at), tc(tc), cb(cb), io(io) {
        static_prio = myrandom(maxprio);
        dynamic_prio = static_prio - 1;
        rem_time = tc;
        state_ts = at;
    }

    int pid;
    int at;
    int tc;
    int cb;
    int io;

    int static_prio;
    int dynamic_prio;

    int cpu_burst = 0;
    int io_burst = 0;
    int rem_time;
    int state_ts;

    int ft = 0;
    int tt = 0;
    int it = 0;
    int cw = 0;

    void get_cpu_burst() {
        if (cpu_burst == 0) {
            cpu_burst = myrandom(cb);
            if (cpu_burst > rem_time) {
                cpu_burst = rem_time;
            }
        }
    }

    void get_io_burst() {
        io_burst = myrandom(io);
        it += io_burst;
    }

    void update_rem(int val) { rem_time -= val; }

    void update_cpu_burst(int val) { cpu_burst -= val; }

    void update_cw(int val) { cw += val; }

    void update_state_ts(int val) { state_ts = val; }

    void set_ft_tt(int val) {
        ft = val;
        tt = ft - at;
    }

    void update_dynamic_prio() { dynamic_prio--; }

    void reset_dynamic_prio() { dynamic_prio = static_prio - 1; }

    bool is_next_done() { return rem_time == cpu_burst; }
};

Process* CURRENT_RUNNING_PROCESS = nullptr;

vector<Process*> process_order;

// --- end --- process ---

// --- begin --- DES ---

class Event {
   public:
    Event(int evtTimeStamp, Process* evtProcess)
        : evtTimeStamp(evtTimeStamp), evtProcess(evtProcess) {}

    Event(int evtTimeStamp, Process* evtProcess, process_state oldState,
          process_state newState)
        : evtTimeStamp(evtTimeStamp),
          evtProcess(evtProcess),
          oldState(oldState),
          newState(newState) {}

    int evtTimeStamp;
    Process* evtProcess;
    process_state oldState = CREATED;
    process_state newState = READY;
};

list<Event*> eventsQ;
list<Event*>::iterator it;

void init_events(string& ifile_name) {
    ifstream ifile;
    ifile.open(ifile_name);
    if (!ifile.is_open()) {
        cout << "Unable to open input_file." << endl;
        exit(1);
    }
    int at, tc, cb, io;
    while (ifile >> at >> tc >> cb >> io) {
        Process* process = new Process(pid_num++, at, tc, cb, io);
        process_order.emplace_back(process);
        Event* event = new Event(at, process);
        eventsQ.emplace_back(event);
    }
    ifile.close();
};

void showEventQ() {
    cout << "ShowEventQ: ";
    for (it = eventsQ.begin(); it != eventsQ.end(); it++) {
        cout << (*it)->evtTimeStamp << ":" << (*it)->evtProcess->pid << " ";
    }
    cout << endl;
};

Event* get_event() {
    if (eventsQ.empty()) {
        return nullptr;
    }
    Event* evt = eventsQ.front();
    eventsQ.pop_front();
    return evt;
};

void put_event(Event* evt) {
    it = eventsQ.begin();
    while (it != eventsQ.end() && evt->evtTimeStamp >= (*it)->evtTimeStamp) {
        it++;
    }
    eventsQ.insert(it, evt);
};

int get_next_event_time() {
    if (eventsQ.empty()) {
        return -1;
    }
    return eventsQ.front()->evtTimeStamp;
};

int is_pending_event_in_future(int curr_time, int pid) {
    it = eventsQ.begin();
    while (it != eventsQ.end() && (*it)->evtProcess->pid != pid) {
        it++;
    }
    return (*it)->evtTimeStamp - curr_time;
}

void rm_event() { eventsQ.erase(it); };

// --- end --- DES ---

// --- begin --- scheduler ---

class Scheduler {
   public:
    virtual void add_process(Process* p) = 0;

    virtual Process* get_next_process() = 0;

    bool does_preempt() { return sched_type == 'E'; };
};

class FCFS : public Scheduler {
   public:
    void add_process(Process* p) { runqueue.emplace(p); };

    Process* get_next_process() {
        if (runqueue.empty()) {
            return nullptr;
        }
        Process* proc = runqueue.front();
        runqueue.pop();
        return proc;
    };

   private:
    queue<Process*> runqueue;
};

class LCFS : public Scheduler {
   public:
    void add_process(Process* p) { runqueue.emplace(p); };

    Process* get_next_process() {
        if (runqueue.empty()) {
            return nullptr;
        }
        Process* proc = runqueue.top();
        runqueue.pop();
        return proc;
    };

   private:
    stack<Process*> runqueue;
};

class SRTF : public Scheduler {
   public:
    void add_process(Process* p) {
        it = runqueue.begin();
        while (it != runqueue.end() && p->rem_time >= (*it)->rem_time) {
            it++;
        }
        runqueue.insert(it, p);
    };

    Process* get_next_process() {
        if (runqueue.empty()) {
            return nullptr;
        }
        Process* proc = runqueue.front();
        runqueue.pop_front();
        return proc;
    };

   private:
    list<Process*> runqueue;
    list<Process*>::iterator it;
};

class PRIO : public Scheduler {
   public:
    PRIO() {
        activeQ = new queue<Process*>[maxprio];
        expiredQ = new queue<Process*>[maxprio];
        AQbitmap = 0;
        EQbitmap = 0;
    }

    ~PRIO() {
        delete activeQ;
        activeQ = nullptr;
        delete expiredQ;
        expiredQ = nullptr;
    }

    void add_process(Process* p) {
        if (p->dynamic_prio < 0) {
            p->reset_dynamic_prio();
            expiredQ[p->dynamic_prio].emplace(p);
            EQbitmap |= (1ull << p->dynamic_prio);
        } else {
            activeQ[p->dynamic_prio].emplace(p);
            AQbitmap |= (1ull << p->dynamic_prio);
        }
    };

    Process* get_next_process() {
        if (AQbitmap == 0) {
            swap(activeQ, expiredQ);
            swap(AQbitmap, EQbitmap);
            if (AQbitmap == 0) {
                return nullptr;
            }
        }
        int pos = 63 - __builtin_clzll(AQbitmap);
        Process* proc = activeQ[pos].front();
        activeQ[pos].pop();
        if (activeQ[pos].empty()) {
            AQbitmap &= ~(1ull << pos);
        }
        return proc;
    };

   private:
    queue<Process*>* activeQ;
    queue<Process*>* expiredQ;
    unsigned long long AQbitmap;  // support max 64 prio level
    unsigned long long EQbitmap;
};

Scheduler* THE_SCHEDULER = nullptr;

void init_scheduler() {
    switch (sched_type) {
        case 'F':
        case 'R':
            THE_SCHEDULER = new FCFS();
            break;
        case 'L':
            THE_SCHEDULER = new LCFS();
            break;
        case 'S':
            THE_SCHEDULER = new SRTF();
            break;
        case 'P':
        case 'E':
            THE_SCHEDULER = new PRIO();
            break;
        default:
            cout << "Unknown Scheduler spec : -v { FLSRPE }" << endl;
            exit(1);
    }
}

void rm_scheduler() {
    delete THE_SCHEDULER;
    THE_SCHEDULER = nullptr;
}

// --- end --- scheduler ---

void simulation() {
    Event* evt;
    while ((evt = get_event())) {
        Process* proc = evt->evtProcess;
        CURRENT_TIME = evt->evtTimeStamp;
        process_state transition = evt->newState;
        int timeInPrevState = CURRENT_TIME - proc->state_ts;
        switch (transition) {
            case READY: {
                proc->update_state_ts(CURRENT_TIME);
                THE_SCHEDULER->add_process(proc);
                CALL_SCHEDULER = true;
                if (vflag == true) {
                    printf("%d %d %d: %s -> %s\n", CURRENT_TIME, proc->pid,
                           timeInPrevState, state_str[evt->oldState],
                           state_str[evt->newState]);
                    // 0 0 0 : CREATED->READY
                }
                if (THE_SCHEDULER->does_preempt() &&
                    CURRENT_RUNNING_PROCESS != nullptr) {
                    int time_diff = is_pending_event_in_future(
                        CURRENT_TIME, CURRENT_RUNNING_PROCESS->pid);
                    bool cond1 = proc->dynamic_prio >
                                 CURRENT_RUNNING_PROCESS->dynamic_prio;
                    bool cond2 = time_diff > 0;
                    if (cond1 && cond2) {
                        rm_event();
                        Event* new_event =
                            new Event(CURRENT_TIME, CURRENT_RUNNING_PROCESS,
                                      RUNNG, PREEMPT);
                        put_event(new_event);
                    }
                    if (pflag == true) {
                        printf(
                            "    --> PrioPreempt Cond1=%d Cond2=%d (%d) --> "
                            "%s\n",
                            cond1, cond2, time_diff, no_yes_str[cond1 & cond2]);
                    }
                }
                break;
            }
            case PREEMPT: {
                CURRENT_RUNNING_PROCESS = nullptr;
                proc->update_state_ts(CURRENT_TIME);
                proc->update_cpu_burst(timeInPrevState);
                proc->update_rem(timeInPrevState);
                if (vflag == true) {
                    printf("%d %d %d: %s -> %s  cb=%d rem=%d prio=%d\n",
                           CURRENT_TIME, proc->pid, timeInPrevState,
                           state_str[evt->oldState], state_str[READY],
                           proc->cpu_burst, proc->rem_time, proc->dynamic_prio);
                    // 2 0 2: RUNNG -> READY  cb=6 rem=98 prio=1
                }
                proc->update_dynamic_prio();
                if (sched_type == 'R') {
                    proc->reset_dynamic_prio();
                }
                THE_SCHEDULER->add_process(proc);
                CALL_SCHEDULER = true;
                break;
            }
            case RUNNG: {
                proc->update_state_ts(CURRENT_TIME);
                CURRENT_RUNNING_PROCESS = proc;
                proc->get_cpu_burst();
                Event* new_evt;
                if (proc->cpu_burst <= quantum) {
                    if (proc->is_next_done()) {
                        new_evt = new Event(CURRENT_TIME + proc->cpu_burst,
                                            proc, RUNNG, DONE);
                    } else {
                        new_evt = new Event(CURRENT_TIME + proc->cpu_burst,
                                            proc, RUNNG, BLOCKED);
                    }
                } else {
                    new_evt =
                        new Event(CURRENT_TIME + quantum, proc, RUNNG, PREEMPT);
                }
                put_event(new_evt);
                if (vflag == true) {
                    printf("%d %d %d: %s -> %s cb=%d rem=%d prio=%d\n",
                           CURRENT_TIME, proc->pid, timeInPrevState,
                           state_str[evt->oldState], state_str[evt->newState],
                           proc->cpu_burst, proc->rem_time, proc->dynamic_prio);
                    // 0 0 0: READY -> RUNNG cb=8 rem=100 prio=1
                }
                break;
            }
            case BLOCKED: {
                proc->get_io_burst();
                proc->update_cpu_burst(timeInPrevState);
                proc->update_rem(timeInPrevState);
                proc->update_state_ts(CURRENT_TIME);
                proc->reset_dynamic_prio();
                update_global_io(CURRENT_TIME, CURRENT_TIME + proc->io_burst);
                CURRENT_RUNNING_PROCESS = nullptr;
                Event* new_evt = new Event(CURRENT_TIME + proc->io_burst, proc,
                                           BLOCKED, READY);
                put_event(new_evt);
                CALL_SCHEDULER = true;
                if (vflag == true) {
                    printf("%d %d %d: %s -> %s  ib=%d rem=%d\n", CURRENT_TIME,
                           proc->pid, timeInPrevState, state_str[evt->oldState],
                           state_str[evt->newState], proc->io_burst,
                           proc->rem_time);
                    // 8 0 8: RUNNG -> BLOCK  ib=2 rem=92
                }
                break;
            }
            case DONE: {
                proc->update_rem(timeInPrevState);
                proc->update_cpu_burst(timeInPrevState);
                proc->update_state_ts(CURRENT_TIME);
                proc->set_ft_tt(CURRENT_TIME);
                CURRENT_RUNNING_PROCESS = nullptr;
                CALL_SCHEDULER = true;
                if (vflag == true) {
                    printf("%d %d %d: Done\n", CURRENT_TIME, proc->pid,
                           timeInPrevState);
                    // 201 0 5: Done
                }
                break;
            }
        }
        delete evt;
        evt = nullptr;

        if (CALL_SCHEDULER) {
            if (get_next_event_time() == CURRENT_TIME) {
                continue;  // process next event from Event queue
            }
            CALL_SCHEDULER = false;  // reset global flag
            if (CURRENT_RUNNING_PROCESS == nullptr) {
                CURRENT_RUNNING_PROCESS = THE_SCHEDULER->get_next_process();
                if (CURRENT_RUNNING_PROCESS == nullptr) {
                    continue;
                }
                // create event to make this process runnable for same time.
                CURRENT_RUNNING_PROCESS->update_cw(
                    CURRENT_TIME - CURRENT_RUNNING_PROCESS->state_ts);
                Event* new_event = new Event(
                    CURRENT_TIME, CURRENT_RUNNING_PROCESS, READY, RUNNG);
                put_event(new_event);
            }
        }
    }
}

void summary() {
    cout << shed_name.at(sched_type);
    if (sched_type == 'R' || sched_type == 'P' || sched_type == 'E') {
        cout << " " << quantum;
    }
    cout << endl;
    int finishtime = 0, time_cpubusy = 0, total_tt = 0, total_cw = 0,
        num_processes = process_order.size();
    for (Process* p : process_order) {
        finishtime = max(finishtime, p->ft);
        time_cpubusy += p->tc;
        total_tt += p->tt;
        total_cw += p->cw;
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n", p->pid, p->at,
               p->tc, p->cb, p->io, p->static_prio, p->ft, p->tt, p->it, p->cw);
    }
    double cpu_util = 100.0 * (time_cpubusy / (double)finishtime);
    double io_util = 100.0 * (time_iobusy / (double)finishtime);
    double throughput = 100.0 * (num_processes / (double)finishtime);
    double avg_tt = total_tt / (double)num_processes;
    double avg_cw = total_cw / (double)num_processes;
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", finishtime, cpu_util,
           io_util, avg_tt, avg_cw, throughput);
}

int main(int argc, char** argv) {
    int c;

    opterr = 0;

    string sched_param;

    while ((c = getopt(argc, argv, "vtepis:")) != -1) {
        switch (c) {
            case 'v':
                vflag = true;
                break;
            case 't':
                tflag = true;
                break;
            case 'e':
                eflag = true;
                break;
            case 'p':
                pflag = true;
                break;
            case 'i':
                iflag = true;
                break;
            case 's':
                sched_param = optarg;
                sched_type = sched_param[0];
                if (sched_type == 'R' || sched_type == 'P' ||
                    sched_type == 'E') {
                    if (sched_param.size() == 1) {
                        cout << "Invalid scheduler param<" << sched_type << ">"
                             << endl;
                        exit(1);
                    }
                    sscanf(optarg, "%c%d:%d", &sched_type, &quantum, &maxprio);
                    // need other checks
                }
                break;
            case '?':
                if (optopt == 's')
                    fprintf(stderr, "Option -%c requires an argument.\n",
                            optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort();
        }
    }

    if (argc - optind == 0) {
        cout << "Not a valid inputfile <(null)>" << endl;
        return 1;
    } else if (argc - optind == 1) {
        cout << "Not a valid random file <(null)>" << endl;
        return 1;
    }
    string ifile_name = argv[optind];
    string rfile_name = argv[optind + 1];

    init_scheduler();
    init_randvals(rfile_name);
    init_events(ifile_name);

    simulation();

    summary();
    rm_scheduler();

    return 0;
}