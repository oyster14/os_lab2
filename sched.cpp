#include <unistd.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

typedef enum { STATE_RUNNING, STATE_BLOCKED } process_state_t;

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
    string line;
    int pos = 0;
    bool is_first = true;
    while (getline(rfile, line)) {
        if (is_first == true) {
            is_first = false;
            ofs_size = stoi(line);
            randvals.resize(ofs_size);
        } else {
            randvals[pos++] = stoi(line);
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

class Process {
   public:
    Process(int AT, int TC, int CB, int IO, int PRIO)
        : AT(AT), TC(TC), CB(CB), IO(IO), PRIO(PRIO) {}

   private:
    int AT;
    int TC;
    int CB;
    int IO;
    int PRIO;
    int quantum = 10000;
};

class Event {
   public:
    Event() {}

    Process* evtProcess;
};

// class Scheduler {
//    public:
//     Scheduler() {}
//     void add_process(Process* p);
//     Process* get_next_process();
//     bool does_preempt();
// };

class DES {};

// void Simulation() {
//     EVENT* evt;
//     while ((evt = get_event())) {
//         Process* proc =
//             evt->evtProcess;  // this is the process the event works on
//         CURRENT_TIME = evt->evtTimeStamp;
//         int transition = evt->transition;
//         int timeInPrevState = CURRENT_TIME – proc->state_ts;  // for
//         accounting delete evt; evt = nullptr;         // remove cur event obj
//         and don’t touch anymore switch (transition) {  // encodes where we
//         come from and where we go
//             case TRANS_TO_READY:
//                 // must come from BLOCKED or CREATED
//                 // add to run queue, no event created
//                 CALL_SCHEDULER = true;
//                 break;
//             case TRANS_TO_PREEMPT:  // similar to TRANS_TO_READY
//                 // must come from RUNNING (preemption)
//                 // add to runqueue (no event is generated)
//                 CALL_SCHEDULER = true;
//                 break;
//             case TRANS_TO_RUN:
//                 // create event for either preemption or blocking
//                 break;
//             case TRANS_TO_BLOCK:
//                 // create an event for when process becomes READY again
//                 CALL_SCHEDULER = true;
//                 break;
//         }
//         if (CALL_SCHEDULER) {
//             if (get_next_event_time() == CURRENT_TIME) {
//                 continue;  // process next event from Event queue
//             }
//             CALL_SCHEDULER = false;  // reset global flag
//             if (CURRENT_RUNNING_PROCESS == nullptr) {
//                 CURRENT_RUNNING_PROCESS = THE_SCHEDULER->get_next_process();
//                 if (CURRENT_RUNNING_PROCESS == nullptr) continue;
//                 // create event to make this process runnable for same time.
//             }
//         }
//     }
// }

int main(int argc, char** argv) {
    bool vflag = false;
    bool tflag = false;
    bool eflag = false;
    bool pflag = false;
    bool iflag = false;
    string schedsped;
    int c;

    opterr = 0;

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
                schedsped = optarg;
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

    init_randvals(rfile_name);

    return 0;
}