//Aidan Martin, Eliot Shea, Dylan McClure COP4610 Project 2
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
//linux libraries for kernel modules

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Elevator simulator for scheduling algorithm");

#define MODULE_NAME "elevator"
#define PERMS 0644
#define PARENT NULL
//elevator's movement states
#define OFFLINE 0
#define IDLE 1
#define LOADING 2
#define UP 3
#define DOWN 4

#define num_floors 10
#define maxload 10
//variables to hold state and stats
int current_state, direction, elevator_destination_floor = -1;   //in case of loading or offline
int elevator_status, passengers_waiting, passengers_serviced;
int current_floor, current_load;
int ind;
static char *message;
static int read_p;

static struct file_operations fops;

struct mutex passenger_mutex;   //mutex for passengers waiting
struct mutex elevator_mutex;    //mutex for passengers on elevator

struct list_head floors[num_floors];
struct list_head elevator_passengers;

typedef struct passenger {    //passenger struct
  struct list_head floor;
  int start_floor;
  int destination_floor;
  int type;
} Passenger;

struct thread_parameter {
  struct task_struct *kthread;
};

struct thread_parameter thread1;

int elevator_run(void *data);

int absolute(int v)
{
  return v * ((v>0) - (v<0));
}

void thread_init_parameter(struct thread_parameter *param) {
  param->kthread = kthread_run(elevator_run, param, "elevator_thread");
}

extern long (*STUB_start_elevator)(void);
long start_elevator(void) {
  thread_init_parameter(&thread1);
  current_state = IDLE;
  printk(KERN_NOTICE "Elevator started!\n");
  return 0;
}

extern long(*STUB_issue_request)(int, int, int);
long issue_request(int start_floor, int destination_floor, int type) {
  printk(KERN_NOTICE "Request issued!\n");
  /*if(start_floor < 1 || start_floor > 10)
    return 1;
  if(destination_floor < 1 || destination_floor > 10)
    return 1;
  if(type > 0 || type < 1)
    return 1;
  */
  Passenger *pass = kmalloc(sizeof(Passenger) * 1, __GFP_RECLAIM);
  pass->start_floor = start_floor;
  pass->destination_floor = destination_floor;
  pass->type = type;

  mutex_lock_interruptible(&passenger_mutex);

  passengers_waiting++;
  list_add_tail(&pass->floor, &floors[start_floor-1]);
  if(elevator_destination_floor == -1){
    elevator_destination_floor = start_floor;
    if (current_floor < elevator_destination_floor) {
      direction = UP;
    } else {
      direction = DOWN;
    }
  }

  mutex_unlock(&passenger_mutex);
  printk(KERN_NOTICE "Passenger added: start=%d, dest=%d, type=%d\n", start_floor, destination_floor, type);
  return 0;
}

extern long (*STUB_stop_elevator)(void);
long stop_elevator(void) {
  kthread_stop(thread1.kthread);
  printk(KERN_NOTICE "Elevator stopped!\n");
  current_state = OFFLINE;
  return 0;
}

/**********************************************************/
//run thread

int load_check(void){
  Passenger* pass;
  Passenger* pass1;
  struct list_head *pos, *q;
  int passengers_added = 0;
  int local_direction, global_direction;
  if(current_load != 10){
    mutex_lock(&passenger_mutex);
    list_for_each_safe(pos, q, &floors[current_floor-1]) {
      pass = list_entry(pos, Passenger, floor);

      if(current_load < 10 && pass != NULL) {// add passengers to the elevator whose destination floor aligns with the direction of travel
        if(direction == UP){
          global_direction = 0;
        }
        else{
          global_direction = 1;
        }
        if(pass->start_floor < pass->destination_floor){
          local_direction = 0;
        }
        else{
          local_direction = 1;
        }
        if(local_direction == global_direction || current_load == 0){
          if (pass->type == 1 || (pass->type == 0 && elevator_status == 0)) {
            // passengers can only get on the elevator if they are infected or the elevator is clean
            pass1 = kmalloc(sizeof(Passenger) * 1, __GFP_RECLAIM);
            pass1->start_floor = pass->start_floor;
            pass1->destination_floor = pass->destination_floor;
            pass1->type = pass->type;

            list_add_tail(&pass1->floor, &elevator_passengers);
            passengers_added++;
            current_load++;
            passengers_waiting--;
            printk(KERN_NOTICE "Loading Passenger: %d %d %d", pass->start_floor, pass->destination_floor, pass->type);
            if (pass->type == 1)
              elevator_status = 1;
            list_del(pos);
            kfree(pass);
          }
        }
      }
    }
    mutex_unlock(&passenger_mutex);
  }
  return passengers_added;
}

void unload(void){
  Passenger* pass;
  struct list_head *pos, *q;
  if(current_load != 0){      //ensuring passengers on elevator
    mutex_lock(&passenger_mutex); //locking passenger mutex
    list_for_each_safe(pos, q, &elevator_passengers) {  //checking each passenger on elevator
      pass = list_entry(pos, Passenger, floor);
      if(pass->destination_floor == current_floor){   //is elevator at passenger's destination
        current_load--;                           //modify counts
        passengers_serviced++;
        printk(KERN_NOTICE "Passenger removed from elevator: start=%d, dest=%d, type=%d\n", pass->start_floor, pass->destination_floor, pass->type);
        list_del(pos);                    //delete from position and free passenger
        kfree(pass);
      }
    }
    mutex_unlock(&passenger_mutex);     //unlock mutex
  }else{
    elevator_status = 0;  //changing to not infected when empty
  }
}

void set_destination(void) {
  Passenger* pass;
  struct list_head *pos, *q;
  int max_dist = 0;
  mutex_lock(&elevator_mutex);
  list_for_each_safe(pos, q, &elevator_passengers) {
    pass = list_entry(pos, Passenger, floor);
    if(absolute(pass->destination_floor) > max_dist && pass != NULL){
      max_dist = absolute(pass->destination_floor);
      elevator_destination_floor = pass->destination_floor;
      printk(KERN_NOTICE "New destination floor set: %d", elevator_destination_floor);
    }
  }
  if (current_floor < elevator_destination_floor){
     printk(KERN_NOTICE "Going UP!");
    direction = UP;
  } else {
    printk(KERN_NOTICE "Going DOWN!");
    direction = DOWN;
  }
  mutex_unlock(&elevator_mutex);
}

int elevator_run(void *data){
  printk(KERN_NOTICE "Inside elevator_run");
  while(!kthread_should_stop()){      //running until stop
    if (passengers_waiting > 0) {
      current_state = LOADING;
      printk(KERN_NOTICE "Loaded %d passengers on floor %d \n", load_check(), current_floor);
      ssleep(1);
      unload();


      if (current_floor == elevator_destination_floor){
        printk(KERN_NOTICE "going into set_destination");
        set_destination();
        printk(KERN_NOTICE "returned from set_destination");
      }

      //increment floor
      printk(KERN_NOTICE "Moving in direction: %d towards destination floor: %d \n", direction, elevator_destination_floor);
      if(direction == UP) {
        current_state = UP;
        if (current_floor < 10){
          printk(KERN_NOTICE "incrementing floor");
          current_floor++;
        } else {
          direction = DOWN;
        }
        ssleep(2);
      } else {
        current_state = DOWN;
        if (current_floor > 1){
           printk(KERN_NOTICE "decrementing floor");
          current_floor--;
        } else {
          direction = UP;
        }
        ssleep(2);
      }
    } else {
      ssleep(5);
      current_state = IDLE;
      elevator_destination_floor = -1;
    }
  }
  return 0;
}


/*******************************************************************/
//proc file functions
int proc_open(struct inode *sp_inode, struct file *sp_file){
   read_p = 1;
   message = kmalloc(sizeof(char) * 2048, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
   if(message == NULL){
     printk(KERN_WARNING "Error: proc_open");
     return -ENOMEM;
   }
   return 0;
}

ssize_t proc_read(struct file *sp_file, char __user *buff, size_t size, loff_t *offset){
   Passenger* pass;     //for checking waiting passengers
   struct list_head *pos, *q;
   char *sub = kmalloc(sizeof(char)*200, __GFP_RECLAIM);  //holds substring, concatenated to msg
   char *waitlist = kmalloc(sizeof(char)*200, __GFP_RECLAIM);  //holds substring, concatenated to msg
   int count;
   char *isFloor;
   sprintf(message, "Elevator state: ");
   switch (current_state){    //convert state to string
     case OFFLINE:
       sprintf(sub, "OFFLINE\n");
       break;
     case IDLE:
       sprintf(sub, "IDLE\n");
       break;
     case LOADING:
       sprintf(sub, "LOADING\n");
       break;
     case UP:
       sprintf(sub, "UP\n");
       break;
     case DOWN:
       sprintf(sub, "DOWN\n");
       break;
     default:
       sprintf(sub, "ERROR\n");
       break;
     }
   strcat(message, sub);          //concatenate message to buf
   if(elevator_status == 1){     //status to string
     sprintf(sub, "Eleavtor status: Infected\n");
   }
   else{
     sprintf(sub, "Eleavtor status: Not infected\n");
   }
   strcat(message, sub);
   sprintf(sub, "Current Floor: %d\nNumber of passengers: %d\n", current_floor, current_load);
   strcat(message, sub);
   sprintf(sub, "Number of passengers waiting: %d\nNumber of Passengers serviced: %d\n", passengers_waiting, passengers_serviced);
   strcat(message, sub);

   for(ind = 10; ind > 0; ind--){ //loop floors
     count=0;             //passengers waiting at floor
     strcpy(waitlist, "");
     if(current_floor == ind){
       isFloor = "*";
     }else{
       isFloor = " ";
     }
     mutex_lock(&passenger_mutex);                //locking passenger waiting mutex
     list_for_each_safe(pos, q, &floors[ind-1]) {   //looping passengers at each floor
      pass = list_entry(pos, Passenger, floor);     //getting list entry
      if (pass != NULL) {                           //ensuring passenger exists
        if(pass->type == 0){            //checking passenger type
          strcat(waitlist, "| ");
        }
        else{
          strcat(waitlist, "X ");
        }
      }
      count = count + 1;
     }
     mutex_unlock(&passenger_mutex);        //unlocking mutex
     sprintf(sub, "[%s] Floor %d:\t%d\t%s\n", isFloor, ind, count, waitlist);
     strcat(message,sub);
   }
   sprintf(sub, "\n\n( '|' for human, 'X' for zombie )\n");
   strcat(message,sub);

   read_p = !read_p;      //ensuring it has been read
   if(read_p){
     return 0;
   }
   copy_to_user(buff, message, strlen(message));  //copy message
   return strlen(message);
}

int proc_release(struct inode *sp_inode, struct file *sp_file){
  kfree(message);       //releasing proc file/ freeing message
  return 0;
}



static int elevator_init(void) {
  STUB_start_elevator = &(start_elevator);  //setting system calls
  STUB_issue_request = &(issue_request);
  STUB_stop_elevator = &(stop_elevator);

  fops.open = proc_open;        //setting gile operations
  fops.read = proc_read;
  fops.release = proc_release;

  elevator_status = 0;     //for not infected
  current_state = OFFLINE;  //initialize initial
  direction = UP;
  current_floor = 1;
  current_load = 0;
  passengers_waiting = 0;
  passengers_serviced = 0;

  for (ind = 0; ind < num_floors; ind++){
    INIT_LIST_HEAD(&floors[ind]);
  }
  INIT_LIST_HEAD(&elevator_passengers);
  mutex_init(&passenger_mutex);           //initializing mutexes
  mutex_init(&elevator_mutex);
  if(!proc_create(MODULE_NAME, PERMS, NULL, &fops)){  //creating proc file if necessary
       printk(KERN_WARNING "Error: proc_create \n");
       remove_proc_entry(MODULE_NAME, NULL);
       return -ENOMEM;
  }
  return 0;
}

static void elevator_exit(void) {
  struct list_head *pos, *q;
  Passenger* pass;

  for (ind = 0; ind < num_floors; ind++){       //removing waiting passengers
    list_for_each_safe(pos, q, &floors[ind]) {
      pass = list_entry(pos, Passenger, floor);
      printk(KERN_NOTICE "Deleting Passenger: %d %d %d", pass->start_floor, pass->destination_floor, pass->type);
      list_del(pos);
      kfree(pass);
    }
  }
  if (current_state != OFFLINE)         //ensuring elevator is offline
    stop_elevator();

  mutex_destroy(&passenger_mutex);      //destroying mutexes
  mutex_destroy(&elevator_mutex);
  remove_proc_entry(MODULE_NAME, NULL); //removing proc entry
  STUB_start_elevator = NULL;           //setting system call STUBs to NULL
  STUB_issue_request = NULL;
  STUB_stop_elevator = NULL;
}

module_init(elevator_init);
module_exit(elevator_exit);
