#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/mutex.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Elevator simulator for scheduling algorithm");

#define MODULE_NAME "elevator"
#define PARENT NULL
//elevator's movement states
#define OFFLINE 0
#define IDLE 1
#define LOADING 2
#define UP 3
#define DOWN 4

#define num_floors 10
#define maxload 10

struct list_head floors[num_floors];

typedef struct passenger {
  struct list_head floor;
  int start_floor;
  int destination_floor;
  int type;
} Passenger;


extern long (*STUB_start_elevator)(void);
long start_elevator(void) {
  printk(KERN_NOTICE "Elevator started!\n");
  return 0;
}

extern long(*STUB_issue_request)(int, int, int);
long issue_request(int start_floor, int destination_floor, int type) {
  printk(KERN_NOTICE "Request issued!\n");
  Passenger* pass = kmalloc(sizeof(Passenger) * 1, __GFP_RECLAIM);
  pass->start_floor = start_floor;
  pass->destination_floor = destination_floor;
  pass->type = type;

  list_add_tail(&pass->floor, &floors[start_floor]);
  printk(KERN_NOTICE "Passenger added: start=%d, dest=%d, type=%d\n", start_floor, destination_floor, type);
  return 0;
}

extern long (*STUB_stop_elevator)(void);
long stop_elevator(void) {
  printk(KERN_NOTICE "Elevator stopped!\n");
  return 0;
}

static int elevator_init(void) {
  STUB_start_elevator = start_elevator;
  STUB_issue_request = issue_request;
  STUB_stop_elevator = stop_elevator;

  int i;
  for (i = 0; i < num_floors; i++){
    INIT_LIST_HEAD(&floors[i]);
  }

  return 0;
}

static void elevator_exit(void) {
  STUB_start_elevator = NULL;
  STUB_issue_request = NULL;
  STUB_stop_elevator = NULL;

  struct list_head *pos, *q;
  Passenger* pass;

  int i;
  for (i = 0; i < num_floors; i++){
    list_for_each_safe(pos, q, &floors[i]) {
      pass = list_entry(pos, Passenger, floor);
      printk(KERN_NOTICE "Deleting Passenger: %d %d %d", pass->start_floor, pass->destination_floor, pass->type);
      list_del(pos);
      kfree(pass);
    }
  }
}

module_init(elevator_init);
module_exit(elevator_exit);
/*

int current_state, next_state;   //in case of loading or offline
int status;
int current_floor, current_load;
int passengers_waiting, passengers_serviced;
//int passengers_waiting[10];
//list of 10 lists of waiting passengers, (1 or 0.. infected/not), desired floor?

int ind;
static char *message;
static int read_p;

struct task_struct* elevator_thread;

static struct file_operations fops;

//struct mutex elevatorPassengers;
//struct mutex passengersList;
/*
struct passengers{
  int start_floor;
  int final_floor;
  int status;
};


int elevator_run(void *data){
  while(!kthread_should_stop()){
  //run module with rules:
    switch (current_state){
      case OFFLINE:
        break;
      case IDLE:
        //IDLE: check for load, set current_state, set next_state to up
        break;
      case LOADING:
        ssleep(1);
        //function to unload passengers from elevator passengers when destination==current_floor
        //function to load elevatorPassengers from next in passengersList at current_floor
          //check space available, elevator status (no humans if infected), right direction..

        mutex_lock_interruptible(&elevatorPassengers);
        //add to elevator
        mutex_unlock(&elevatorPassengers);

        mutex_lock_interruptible(&passengersList);
        //add to list
        mutex_unlock(&passengersList);
        break;
      case UP:
        if(current_floor == 10){
          current_state = DOWN;
          next_state = DOWN;
          ssleep(2);
          current_floor = current_floor - 1;
        }else{
          ssleep(2);
          current_floor = current_floor + 1;
        }
        //load/unload check?
        break;
      case DOWN:
        if(current_floor == 1){
          current_state = UP;
          next_state = UP;
          ssleep(2);
          current_floor = current_floor + 1;
        }else{
          ssleep(2);
          current_floor = current_floor - 1;
        }
        ssleep(2);
        //load/unload/idle check?
        break;
    }
  }

  return 0;
}

/**********************************************************/

// char *getState(int current_state){
//   static char str[10];
//   switch (current_state){
//     case OFFLINE:
//       sprintf(str, "OFFLINE");
//       break;
//     case IDLE:
//       sprintf(str, "IDLE");
//       break;
//     case LOADING:
//       sprintf(str, "LOADING");
//       break;
//     case UP:
//       sprintf(str, "UP");
//       break;
//     case DOWN:
//       sprintf(str, "DOWN");
//       break;
//     default:
//       sprintf(str, "ERROR");
//       break;
//   }
//   return str;
// }

// char *printfloors(int current_floor){
//   int i;
//   char f;
//   sprintf("\n\n");
//   for(ind = 10; ind > 0; ind--){
//     f = " ";
//     if(current_floor == ind){
//       f = "*"
//     }
//     sprintf("[%s] Floor %d:\t%d\t", f, ind, passengers_waiting);
//     /*
//     //loop through mutex?
//     for(i = 0; i < passengers_waiting[ind]; i++){
//       if(passengers_waiting[ind][i] == 0){
//         sprintf("X");
//       }
//       else{
//         sprintf("|");
//       }
//     }
//     */
//     sprintf("\n\n");

//   }
//   sprintf("( '|' for human, 'X' for zombie )\n");
// }

// /*******************************************************************/

// int proc_open(struct inode *sp_inode, struct file *sp_file){
//   read_p = 1;
//   printk(KERN_NOTICE "proc_open\n");
//   message = kmalloc(sizeof(char) * 1024, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
//   if(message == NULL){
//     printk(KERN_WARNING "Error: proc_open");
//     return -ENOMEM;
//   }
//   return 0;
// }

// ssize_t proc_read(struct file *sp_file, char __user *buff, size_t size, loff_t *offset){
//   int len = strlen(message);
//   char *stat;
//   if(status == 1){
//     stat="Infected";
//   }else{
//     stat="Not infected";
//   }
//   sprintf(message, "Elevator state: %s \nElevator status: %s \nCurrent floor: %d \nNumber of passengers: %d \nNumber of passengers waiting: %d\nNumber passengers serviced: %d \n%s",
// getState(current_state), stat, current_floor,  current_load, passengers_waiting, passengers_serviced, printfloors());

//   read_p = !read_p;
//   if(read_p){
//     return 0;
//   }
//   printk(KERN_NOTICE "proc_read\n");
//   copy_to_user(buff, message, len);
//   return len;
// }

// int proc_release(struct inode *sp_inode, struct file *sp_file)
//   printk(KERN_NOTICE "proc_release\n");
//   kfree(message);
//   return 0;
// }

// /*******************************************************************/

// static int elevator_init(void) {
//   printk(KERN_NOTICE "Creating /proc/%s.\n", MODULE_NAME);

//   fops.open = proc_open;
//   fops.read = proc_read;
//   fops.release = proc_release;

//   status = 0;     //for not infected
//   current_state = OFFLINE;  //initialize states
//   next_state = UP;
//   current_floor = 1;
//   current_load = 0;
//   passengers_waiting = 0;
//   passengers_serviced = 0;

//   if(!proc_create(MODULE_NAME, 0644, NULL, &fops)){
//     printk(KERN_WARNING "Error: proc_create \n");
//     remove_proc_entry(MODULE_NAME, NULL);
//     return -ENOMEM;
//   }
  
//   elevator_thread = kthread_run(elevator_run, NULL, "Elevator Thread");
//   if(IS_ERR(elevator_thread)){
//     printk(KERN_WARNING "Error: ElevatorRun\n");
//     return PTR_ERR(elevator_thread);
//   }
//   return 0;
// }

// static void elevator_exit(void) {
//     mutex_destroy(&elevatorPassengers);
//     mutex_destroy(&passengersList);  
//     kthread_stop(elevator_thread);
//     remove_proc_entry(MODULE_NAME, NULL);
//     printk(KERN_NOTICE "Removing /proc/%s.\n", MODULE_NAME);
// }

// module_init(elevator_init);
// module_exit(elevator_exit);
