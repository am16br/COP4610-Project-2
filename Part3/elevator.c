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

int current_state, next_state;   //in case of loading or offline
int status, passengers_waiting, passengers_serviced;
int current_floor, current_load;
int ind;
static char *message;
static int read_p;

static struct file_operations fops;

struct mutex passenger_mutex;

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
  Passenger *pass = kmalloc(sizeof(Passenger) * 1, __GFP_RECLAIM);
  pass->start_floor = start_floor;
  pass->destination_floor = destination_floor;
  pass->type = type;

  mutex_lock_interruptible(&passenger_mutex);

  list_add_tail(&pass->floor, &floors[start_floor-1]);

  mutex_unlock(&passenger_mutex);
  printk(KERN_NOTICE "Passenger added: start=%d, dest=%d, type=%d\n", start_floor, destination_floor, type);
  return 0;
}

extern long (*STUB_stop_elevator)(void);
long stop_elevator(void) {
  printk(KERN_NOTICE "Elevator stopped!\n");
  return 0;
}

/**********************************************************/
//run thread
/*
int load_check(void){
  Passenger* pass;
  struct list_head *pos, *q;

  if(current_load != 10){
    mutex_lock_interruptible(&waiting_mutex);
    list_for_each_safe(pos, q, &floors[current_floor]) {
      pass = list_entry(pos, Passenger, floor);
      if(pass->type == 0){
        if(status == 1){
          return 0;
        }
      }
      mutex_unlock(&waiting_mutex);
      return 1;
      if(pass->destination_floor == current_floor){
        mutex_unlock(&passenger_mutex);
        return 1;
      }
    }
    mutex_unlock(&waiting_mutex);
  }
  return 0;
}

int unload_check(void){
  Passenger* pass;
  struct list_head *pos, *q;
  mutex_lock_interruptible(&passenger_mutex);
  list_for_each_safe(pos, q, &floors) {
    pass = list_entry(pos, Passenger, floor);
    if(pass->destination_floor == current_floor){
      mutex_unlock(&passenger_mutex);
      return 1;
    }
    kfree(pass);
  }
  mutex_unlock(&passenger_mutex);
  return 0;
}

void load(int floor){
  Passenger* wpass;
  struct list_head *pos, *q;
  mutex_unlock(&waiting_mutex);
  list_for_each_safe(pos, q, &floors[floor-1]) {
    wpass = list_entry(pos, Passenger, floor);
    Passenger* pass = kmalloc(sizeof(Passenger) * 1, __GFP_RECLAIM);
    pass->start_floor = wpass->start_floor;
    pass->destination_floor = wpass->destination_floor;
    pass->type = wpass->type;
    if(pass->type == 1){
      status == 1;
    }
    mutex_lock_interruptible(&passenger_mutex);
    list_add_tail(&pass, &floors);
    list_del(pos);
    kfree(wpass);
    mutex_unlock(&passenger_mutex);
    mutex_unlock(&waiting_mutex);
    return;
  }
  mutex_unlock(&waiting_mutex);
}

void unload(void){
  Passenger* pass;
  struct list_head *pos, *q;
  mutex_lock_interruptible(&passenger_mutex);
  list_for_each_safe(pos, q, &floors) {
    pass = list_entry(pos, Passenger, floor);
    if(pass->destination_floor == current_floor){
      passengers_serviced++;
      list_del(pos);
      kfree(pass);
    }
  }
  mutex_unlock(&passenger_mutex);
  if(current_load == 0){
    status = 0;
  }
  return;
}

int elevator_run(void *data){
  while(!kthread_should_stop()){
    switch (current_state){
      case OFFLINE:
        break;
      case IDLE:
        //IDLE: check for load, set current_state, set next_state to up
        if(load_check() == 1){
          current_state = LOADING;
        }
        else{
          current_state = UP;
        }
        next_state = UP;
        break;
      case LOADING:
        ssleep(1);
        unload();
        if(load_check() == 1){
          load(current_floor);
        }
        current_state = next_state;
        if(current_state == UP){
          if(current_floor == 10){
            current_state = DOWN;
            next_state = DOWN;
          }
        }
        else if(current_state == DOWN){
          if(current_floor == 1){
            current_state = UP;
            next_state = UP;
          }
        }
        break;
      case UP:
        if(current_floor == 10){
          current_state = DOWN;
          next_state = DOWN;
        }else{
          ssleep(2);
          current_floor = current_floor + 1;
          if(load_check() == 1 || unload_check() == 1){
            current_state = LOADING;
          }
        }
        break;
      case DOWN:
        if(current_floor == 1){
          current_state = UP;
          next_state = UP;
        }else{
          ssleep(2);
          current_floor = current_floor - 1;
          if(load_check() == 1 || unload_check() == 1){
            current_state = LOADING;
          }
        }
        //idle check?
        break;
    }
  }
  return 0;
}
*/

/**********************************************************/
//display functions
char *getState(int current_state){
  //convert state to printable string
   static char str[10];
   switch (current_state){
     case OFFLINE:
       sprintf(str, "OFFLINE");
       break;
     case IDLE:
       sprintf(str, "IDLE");
       break;
     case LOADING:
       sprintf(str, "LOADING");
       break;
     case UP:
       sprintf(str, "UP");
       break;
     case DOWN:
       sprintf(str, "DOWN");
       break;
     default:
       sprintf(str, "ERROR");
       break;
   }
   return str;
}

char *printfloors(void){
  //print all floors
   //struct list_head *pos, *q;
   //Passenger* pass;
   int count;
   char *isFloor;//, *waitlist;
   char *str = kmalloc(sizeof(char) * 2048, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
   char *buf = kmalloc(sizeof(char) * 2048, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
   char *ret = kmalloc(sizeof(char) * 2048, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
   for(ind = 10; ind > 0; ind--){ //loop floors
     count=0;   //passengers waiting at floor
     //waitlist = NULL;
     if(current_floor == ind){
       isFloor="*";
     }else{
       isFloor=" ";
     }
     count = count + 1;
     /*
     mutex_lock_interruptible(&passenger_mutex);
     list_for_each_safe(pos, q, &floors[ind]) {
       count = count + 1;
       pass = list_entry(pos, Passenger, floor);
       if(pass->type == 0){
         strcat(waitlist, "|");
       }
       else{
         strcat(waitlist, "X");
       }
       kfree(pass);
     }
     mutex_unlock(&passenger_mutex);
     */
     sprintf(buf, "[%s] Floor %d:\t%d\n", isFloor, ind, count);
     strcat(str,buf);
      //print users at waiting at each floor!!!!
   }
   sprintf(ret, "%s\n\n( '|' for human, 'X' for zombie )\n", str);
   return ret;
}

/*******************************************************************/

//proc file functions

int proc_open(struct inode *sp_inode, struct file *sp_file){
   read_p = 1;
   printk(KERN_NOTICE "proc_open\n");
   message = kmalloc(sizeof(char) * 2048, __GFP_RECLAIM | __GFP_IO | __GFP_FS);
   if(message == NULL){
     printk(KERN_WARNING "Error: proc_open");
     return -ENOMEM;
   }
   return 0;
}

ssize_t proc_read(struct file *sp_file, char __user *buff, size_t size, loff_t *offset){
   printk(KERN_NOTICE "proc_read\n");
   char *stat;
   if(status == 1){     //convert int status to printable string
     stat="Infected";
   }else{
     stat="Not infected";
   }
   //sprintf(message, "Elevator state: %s\nElevator status: %s \nCurrent floor: %d \nNumber of passengers: %d \nNumber of passengers waiting: %d\nNumber passengers serviced: %d \n", getState(current_state),stat, current_floor,  current_load, passengers_waiting, passengers_serviced);

   sprintf(message, "Elevator state: %s\nElevator status: %s \nCurrent floor: %d \nNumber of passengers: %d \nNumber of passengers waiting: %d\nNumber passengers serviced: %d \n%s\n", getState(current_state),stat, current_floor,  current_load, passengers_waiting, passengers_serviced, printfloors());
   read_p = !read_p;
   if(read_p){
     return 0;
   }
   copy_to_user(buff, message, strlen(message));  //copy message
   return strlen(message);
}

int proc_release(struct inode *sp_inode, struct file *sp_file){
   printk(KERN_NOTICE "proc_release\n");
   kfree(message);
   return 0;
}



static int elevator_init(void) {
  STUB_start_elevator = &(start_elevator);
  STUB_issue_request = &(issue_request);
  STUB_stop_elevator = &(stop_elevator);

  fops.open = proc_open;
  fops.read = proc_read;
  fops.release = proc_release;

  status = 0;     //for not infected
  current_state = OFFLINE;  //initialize states
  next_state = UP;
  current_floor = 1;
  current_load = 0;
  passengers_waiting = 0;
  passengers_serviced = 0;

  for (ind = 0; ind < num_floors; ind++){
    INIT_LIST_HEAD(&floors[ind]);
  }
  mutex_init(&passenger_mutex);
  if(!proc_create(MODULE_NAME, PERMS, NULL, &fops)){
       printk(KERN_WARNING "Error: proc_create \n");
       remove_proc_entry(MODULE_NAME, NULL);
       return -ENOMEM;
  }
  return 0;
}

static void elevator_exit(void) {
  struct list_head *pos, *q;
  Passenger* pass;

  for (ind = 0; ind < num_floors; ind++){
    list_for_each_safe(pos, q, &floors[ind]) {
      pass = list_entry(pos, Passenger, floor);
      printk(KERN_NOTICE "Deleting Passenger: %d %d %d", pass->start_floor, pass->destination_floor, pass->type);
      list_del(pos);
      kfree(pass);
    }
  }
  mutex_destroy(&passenger_mutex);
  remove_proc_entry(MODULE_NAME, NULL);
  STUB_start_elevator = NULL;
  STUB_issue_request = NULL;
  STUB_stop_elevator = NULL;
}

module_init(elevator_init);
module_exit(elevator_exit);
