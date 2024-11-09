#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  
  u32 remaining_burst_time;
  u32 start_time;
  u32 end_time;
  u32 waiting_time;
  u32 response_time;
  
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */

  printf("my code starting");

  struct process *p;
  
  int time = 0;
  int quantum_remaining = quantum_length;
  int num_processes_done = 0;

  while (true) {

    // check to see if any processes to add at current time
    for (int i = 0; i < size; i++) {
      if (data[i].arrival_time == time) {
	
	p = &data[i];
	// init start time to -1
	// makes it easy to check if process has been scheduled
	p-> start_time = -1;
	p->remaining_burst_time = p->burst_time;

	TAILQ_INSERT_TAIL(&list, p, pointers);
      }
    }
    printf("time: %d", time);
    // check to make sure queue is not empty
    if (!TAILQ_EMPTY(&list)) {
      // if quantum_remaining is 0, context switch: remove head from queue
      if (quantum_remaining == 0) {
	quantum_remaining = quantum_length;
	p = TAILQ_FIRST(&list);
	TAILQ_REMOVE(&list, p, pointers);

	// if head's time remaining > 0, move it to tail
	if (p->remaining_burst_time > 0) {
	  TAILQ_INSERT_TAIL(&list, p, pointers);
	}

	//if head is done, remove it
	else if (p->remaining_burst_time == 0) {
	  num_processes_done++;
	}
      }

    }

    // check to make sure queue is not empty from removing head
    if (!TAILQ_EMPTY(&list)) {
      
      // set p to process at front of queue
      p = TAILQ_FIRST(&list);

      // if first time process is scheduled
      if (p-> start_time == -1) {
	p-> start_time = time;
	p-> response_time = time - p->arrival_time;
      }

      p->remaining_burst_time = p->remaining_burst_time - 1;
      quantum_remaining = quantum_remaining - 1;

      // if current process is done
      if (p->remaining_burst_time == 0) {

	// process removed on next iteration
	p->end_time = time + 1;
	// subtract burst_time so that we don't count time executing
	p-> waiting_time = p->end_time - p->arrival_time - p->burst_time;
	// set to 0 so that we can do a context switch
	quantum_remaining = 0;
      }

      // if we're done with all processes
      if (num_processes_done == size) {
	break;
      }
      
      time++;
    }

    // calculate total waiting and response times
      
    
  }
  
  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
