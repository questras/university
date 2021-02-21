#include <pthread.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "cacti.h"

#define UNUSED(x) (void)(x)


/*
 * Declarations
 */
struct queue;
typedef struct queue queue_t;

struct thread_pool;
typedef struct thread_pool thread_pool_t;
struct pool_job;
typedef struct pool_job pool_job_t;

struct actor_data;
typedef struct actor_data actor_data_t;

struct actor_system;
typedef struct actor_system actor_system_t;


queue_t *init_queue();

void destroy_queue(queue_t *q);

void enqueue(queue_t *q, void *data);

void *dequeue(queue_t *q);

size_t get_size(queue_t *q);

bool jobs_ready(thread_pool_t *tpool);

void *run_thread_in_pool(void *arg);

void thread_pool_join(thread_pool_t *tpool);

thread_pool_t *create_thread_pool(int n_threads);

void destroy_thread_pool(thread_pool_t *tpool);

actor_system_t *init_actor_system();

actor_id_t add_actor(role_t *role);

void kill_actor(actor_id_t actor);

void destroy_actor_system(actor_system_t *actor_system);

void go_die_handler(void **stateptr, size_t nbytes, void *data);

void spawn_handler(void **stateptr, size_t nbytes, void *data);

// Global data structures.
actor_system_t *ac_sys;
thread_pool_t *glob_pool;
act_t *global_functions;

typedef void (*f_ptr)(void **stateptr, size_t nbytes, void *data);

static pthread_key_t currentThreadActorJob;
/*
 * End Declarations
 */

/*
 * Structures
 */
struct queue {
    void **data;
    int head, tail;
    int size;
};

struct pool_job {
    act_t *f;
    void **stateptr;
    size_t nbytes;
    void *data;
};

struct thread_pool {
    int how_many_ready_jobs;
    queue_t *waiting_actors_queue;

    pthread_mutex_t mutex;
    pthread_cond_t job_ready;
    pthread_cond_t all_threads_stopped;

    int n_living;
    bool is_stopped;
};

struct actor_data {
    bool is_dead;

    role_t *role;
    queue_t *queue;
    void **stateptr;

    pthread_mutex_t mutex;
};

struct actor_system {
    int how_many_actors;
    int how_many_godie;
    int current_actors_size;
    int last_actor_index;

    actor_data_t **actors;

    pthread_mutex_t mutex;
};
/*
 * End structures
 */

/*
 * Queue
 */
queue_t *init_queue() {
    queue_t *q = malloc(sizeof(queue_t));
    q->size = 100;
    q->data = malloc(q->size * sizeof(void *));
    q->head = 0;
    q->tail = -1;

    return q;
}


void destroy_queue(queue_t *q) {
    free(q->data);
    free(q);
}


void enqueue(queue_t *q, void *data) {
    if (q == NULL) {
        return;
    }

    if (q->tail + 1 == q->size) {
        // Size overflow, need to realloc.
        q->size *= 2;
        q->data = realloc(q, q->size * sizeof(void *));
    }
    q->tail++;
    q->data[q->tail] = data;
}


void *dequeue(queue_t *q) {
    if (q == NULL || q->head > q->tail) {
        return NULL;
    }

    void *data = q->data[q->head];
    q->head++;

    return data;
}


size_t get_size(queue_t *q) {
    return q->tail - q->head + 1;
}
/*
 * End Queue
 */

/*
 * Thread Pool.
 */
bool jobs_ready(thread_pool_t *tpool) {
    return tpool->how_many_ready_jobs > 0;
}


void *run_thread_in_pool(void *arg) {
    pool_job_t *job_to_do;
    thread_pool_t *tpool = arg;

    while (true) {
        pthread_mutex_lock(&(tpool->mutex));

        while (!jobs_ready(tpool) && !(tpool->is_stopped)) {
            // Wait for job.
            pthread_cond_wait(&(tpool->job_ready), &(tpool->mutex));
        }

        if (jobs_ready(tpool)) {
            // Get next actor in queue.
            actor_id_t next_actor = dequeue(glob_pool->waiting_actors_queue);

            pthread_mutex_lock(&(ac_sys->actors[next_actor]->mutex));

            pthread_setspecific(currentThreadActorJob, &next_actor);
            job_to_do = dequeue(ac_sys->actors[next_actor]->queue);

            pthread_mutex_unlock(&(ac_sys->actors[next_actor]->mutex));
            tpool->how_many_ready_jobs--;
        }
        else {
            if (tpool->is_stopped) {
                break;
            }
            job_to_do = NULL;
        }
        pthread_mutex_unlock(&(tpool->mutex));

        if (job_to_do != NULL) {
            // Call function.
            (*(job_to_do->f))(
                    job_to_do->stateptr,
                    job_to_do->nbytes,
                    job_to_do->data);
            free(job_to_do);
        }

        pthread_mutex_lock(&(tpool->mutex));

        if (!jobs_ready(tpool) && tpool->is_stopped) {
            break;
        }

        pthread_mutex_unlock(&(tpool->mutex));
    }

    // Critical Section inheritance.
    tpool->n_living--;
    if (tpool->n_living == 0) {
        // Last thread has died.
        pthread_mutex_unlock(&(tpool->mutex));
        pthread_cond_broadcast(&(tpool->all_threads_stopped));
    }
    else {
        // Wake up the rest that might be alive and waiting.
        pthread_mutex_unlock(&(tpool->mutex));
        pthread_cond_broadcast(&(tpool->job_ready));
    }

    return NULL;
}


// Wait for all threads to finish their work.
void thread_pool_join(thread_pool_t *tpool) {
    if (tpool == NULL) {
        return;
    }

    pthread_mutex_lock(&(tpool->mutex));
    while (!(tpool->is_stopped) || tpool->n_living > 0) {
        pthread_cond_wait(&(tpool->all_threads_stopped), &(tpool->mutex));
    }
    pthread_mutex_unlock(&(tpool->mutex));
}


thread_pool_t *create_thread_pool(int n_threads) {
    // Create key for threads that will hold id of current job's actor.
    pthread_key_create(&currentThreadActorJob, NULL);

    thread_pool_t *tpool = malloc(sizeof(thread_pool_t));

    tpool->n_living = n_threads;
    tpool->how_many_ready_jobs = 0;
    tpool->is_stopped = false;
    tpool->waiting_actors_queue = init_queue();

    pthread_mutex_init(&(tpool->mutex), NULL);
    pthread_cond_init(&(tpool->job_ready), NULL);
    pthread_cond_init(&(tpool->all_threads_stopped), NULL);


    for (int i = 0; i < n_threads; ++i) {
        pthread_t thread;
        pthread_create(&thread, NULL, run_thread_in_pool, tpool);
        pthread_detach(thread);
    }

    return tpool;
}


void destroy_thread_pool(thread_pool_t *tpool) {
    destroy_queue(tpool->waiting_actors_queue);
    free(tpool);
}
/*
 * End Thread Pool.
 */


/*
 * Actor System Structure
 */
actor_system_t *init_actor_system() {
    actor_system_t *sys = malloc(sizeof(actor_system_t));
    sys->how_many_actors = 0;
    sys->how_many_godie = 0;
    sys->current_actors_size = 1000;
    sys->last_actor_index = -1;

    sys->actors = malloc(sys->current_actors_size * sizeof(actor_data_t));

    pthread_mutex_init(&(sys->mutex), NULL);

    f_ptr *_global_functions = malloc(2 * sizeof(f_ptr));
    _global_functions[0] = go_die_handler;
    _global_functions[1] = spawn_handler;
    global_functions = (act_t *) _global_functions;

    return sys;
}


// Create actor and return its id.
actor_id_t add_actor(role_t *role) {
    if (ac_sys == NULL) {
        return -1;
    }

    pthread_mutex_lock(&(ac_sys->mutex));

    if (ac_sys->last_actor_index + 1 == CAST_LIMIT) {
        // To many actors.
        exit(1);
    }

    // Check if there is enough place for new actor.
    if (ac_sys->last_actor_index + 1 == ac_sys->current_actors_size) {
        // Need to realloc.
        ac_sys->current_actors_size *= 2;
        ac_sys->actors = realloc(ac_sys->actors, ac_sys->current_actors_size * sizeof(actor_data_t *));
    }

    ac_sys->last_actor_index++;
    actor_id_t new_id = ac_sys->last_actor_index;

    ac_sys->actors[ac_sys->last_actor_index] = malloc(sizeof(actor_data_t));
    ac_sys->actors[ac_sys->last_actor_index]->role = role;
    ac_sys->actors[ac_sys->last_actor_index]->is_dead = false;
    ac_sys->actors[ac_sys->last_actor_index]->stateptr = NULL;
    ac_sys->actors[ac_sys->last_actor_index]->queue = init_queue();
    pthread_mutex_init(&(ac_sys->actors[ac_sys->last_actor_index]->mutex), NULL);

    ac_sys->how_many_actors++;

    pthread_mutex_lock(&(glob_pool->mutex));
    glob_pool->is_stopped = ac_sys->how_many_actors == ac_sys->how_many_godie;
    pthread_mutex_unlock(&(glob_pool->mutex));
    pthread_mutex_unlock(&(ac_sys->mutex));

    return new_id;
}


void kill_actor(actor_id_t actor) {
    pthread_mutex_lock(&(glob_pool->mutex));
    pthread_mutex_lock(&(ac_sys->mutex));

    ac_sys->actors[actor]->is_dead = true;
    ac_sys->how_many_godie++;
    glob_pool->is_stopped = ac_sys->how_many_actors == ac_sys->how_many_godie;

    pthread_mutex_unlock(&(ac_sys->mutex));
    pthread_mutex_unlock(&(glob_pool->mutex));
}


void destroy_actor_system(actor_system_t *actor_system) {
    if (actor_system == NULL) {
        return;
    }

    for (int i = 0; i < actor_system->last_actor_index + 1; ++i) {
        destroy_queue(actor_system->actors[i]->queue);
        free(actor_system->actors[i]);
    }

    free(actor_system);
}
/*
 * End Actor System Structure
 */


/*
 * Actor system.
 */
void go_die_handler(void **stateptr, size_t nbytes, void *data) {
    UNUSED(stateptr);
    UNUSED(nbytes);
    UNUSED(data);

    actor_id_t curr_ac_id = actor_id_self();
    kill_actor(curr_ac_id);
}


void spawn_handler(void **stateptr, size_t nbytes, void *data) {
    UNUSED(stateptr);
    UNUSED(nbytes);

    actor_id_t new_ac_id = add_actor((role_t *) data);
    actor_id_t parent_id = actor_id_self();

    message_t *hello = malloc(sizeof(message_t));
    hello->data = (void *) parent_id;
    hello->nbytes = sizeof(parent_id);
    hello->message_type = MSG_HELLO;

    send_message(new_ac_id, *hello);
}


int send_message(actor_id_t actor, message_t message) {
    if (ac_sys == NULL) {
        exit(1);
    }

    pthread_mutex_lock(&(ac_sys->mutex));
    if (actor > ac_sys->last_actor_index) {
        pthread_mutex_unlock(&(ac_sys->mutex));
        return -2;
    }
    else if (ac_sys->actors[actor]->is_dead) {
        pthread_mutex_unlock(&(ac_sys->mutex));
        return -1;
    }

    pool_job_t *job = malloc(sizeof(pool_job_t));

    pthread_mutex_lock(&(ac_sys->actors[actor]->mutex));
    if (get_size(ac_sys->actors[actor]->queue) == ACTOR_QUEUE_LIMIT) {
        exit(1);
    }

    if (message.message_type == MSG_GODIE) {
        job->f = &(global_functions[0]);
        job->stateptr = ac_sys->actors[actor]->stateptr;
        job->nbytes = message.nbytes;
        job->data = message.data;
    }
    else if (message.message_type == MSG_SPAWN) {
        job->f = &(global_functions[1]);
        job->stateptr = ac_sys->actors[actor]->stateptr;
        job->nbytes = message.nbytes;
        job->data = message.data;
    }
    else if (message.message_type == MSG_HELLO) {
        job->f = &(ac_sys->actors[actor]->role->prompts[message.message_type]);
        job->stateptr = ac_sys->actors[actor]->stateptr;
        job->nbytes = sizeof(actor);
        job->data = (void *) actor;
    }
    else {
        job->f = &(ac_sys->actors[actor]->role->prompts[message.message_type]);
        job->stateptr = ac_sys->actors[actor]->stateptr;
        job->nbytes = message.nbytes;
        job->data = message.data;
    }
    enqueue(ac_sys->actors[actor]->queue, job);

    pthread_mutex_unlock(&(ac_sys->actors[actor]->mutex));
    pthread_mutex_unlock(&(ac_sys->mutex));

    pthread_mutex_lock(&(glob_pool->mutex));

    enqueue(glob_pool->waiting_actors_queue, (void *) actor);
    glob_pool->how_many_ready_jobs++;

    pthread_mutex_unlock(&(glob_pool->mutex));

    // Tell threads there is new job to process.
    pthread_cond_broadcast(&(glob_pool->job_ready));

    return 0;
};


actor_id_t actor_id_self() {
    actor_id_t *curr_actor = pthread_getspecific(currentThreadActorJob);

    if (curr_actor == NULL) {
        return -1;
    }

    return *curr_actor;
}


int actor_system_create(actor_id_t *actor, role_t *const role) {
    glob_pool = create_thread_pool(POOL_SIZE);
    ac_sys = init_actor_system();
    *actor = add_actor(role);

    return 0;
}


void actor_system_join(actor_id_t actor) {
    if (actor > ac_sys->last_actor_index) {
        // No such actor.
        exit(1);
    }

    thread_pool_join(glob_pool);
    destroy_thread_pool(glob_pool);
    destroy_actor_system(ac_sys);
}
/*
 * Actor system end.
 */




