#ifndef DEFS_H
#define DEFS_H

#include <pthread.h>

/** Debugging messages off by default */
#define DEBUGGING   0
/** Wrapper for stderr print */
#define debug_mode(x) { if (DEBUGGING) x; }

/** Number of global file locks (glocks). */
#define N_GLOCKS    2
/** Index of read.txt glock */
#define READ_GLOCK  0
/** Index of empty.txt glock */
#define EMPTY_GLOCK 1

/** Maximum size of action */
#define MAX_ACTION_SIZE 6
/** Maximum size of path and input */
#define MAX_INPUT_SIZE  51

/** Macro for empty.txt */
#define EMPTY_TARGET    "empty.txt"
/** Access mode for empty.txt */
#define EMPTY_MODE      "a"
/** Macro for read.txt */
#define READ_TARGET     "read.txt"
/** Access mode for read.txt */
#define READ_MODE       "a"
/** Macro for commands.txt */
#define CMD_TARGET      "commands.txt"
/** Access mode for commands.txt */
#define CMD_MODE        "a"

#define FMT_2LOG        "%s %s: "
#define FMT_READ_MISS   "%s %s: FILE DNE\n"
#define FMT_EMPTY_MISS  "%s %s: FILE ALREADY EMPTY\n"
#define FMT_2CMD        "[%s] %s %s\n"
#define FMT_3CMD        "[%s] %s %s %s\n"


/** @struct __command
 * Used to store input in a consistent format.
 * 
 * @var __command::action
 * User action. Only read, write, empty.
 * @var __command::path
 * Target path.
 * @var __command::input
 * User input.
 */
typedef struct __command {
    char action[MAX_ACTION_SIZE];
    char path[MAX_INPUT_SIZE];
    char input[MAX_INPUT_SIZE];
} command;

/** @struct __args_t
 * @brief Arguemnts passed to worker threads.
 * 
 * @var __args_t::in_lock
 * Entry lock for hand-over-hand locking.
 * @var __args_t::out_lock
 * Exit lock for hand-over-hand locking.
 * Unlocked when thread ends.
 * @var __args_t::cmd
 * Pointer to command to execute.
 * Stored separately from command used to read
 * user input.
 */
typedef struct __args_t {
    pthread_mutex_t *in_lock;
    pthread_mutex_t *out_lock;

    command *cmd;
} args_t;

/** @struct __fmeta
 * @brief File metadata. Key component of hand-over-hand 
 * locking and building worker threads. This keeps track 
 * of the most recent `out_lock` in `fmeta.recent_lock`.
 * 
 * @var __fmeta::recent_lock
 * Most recent out_lock. Used as the next in_lock.
 * @var __fmeta::path
 * File path used to identify file.
 */
typedef struct __fmeta {
    pthread_mutex_t *recent_lock;

    char path[MAX_INPUT_SIZE];
} fmeta;

/** @struct __lnode_t
 * @brief List node. Used to track file metadata 
 * with key, value pair.
 * 
 * @var __lnode_t::key
 * File path used as key.
 * @var __lnode_t::value
 * File metadata used as value.
 * @var __lnode_t::next
 * Next node in linked list.
 */
typedef struct __lnode_t {
    char *key;
    fmeta *value;
    struct __lnode_t *next;
} lnode_t;

/** @struct __list_t
 * @brief Non-blocking list struct. Based-off OSTEP.
 * 
 * @var __list_t::head
 * Pointer to head of linked list.
 */
typedef struct __list_t {
    lnode_t *head;
} list_t;


#endif
