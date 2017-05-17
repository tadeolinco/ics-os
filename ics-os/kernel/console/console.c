/* 
   ==========================================================================
   Console.c
   Author: Joseph Emmanuel Dayo
   Date updated:December 6, 2002
   Description: A kernel mode console that is used for debugging the kernel
   and testing new kernel features.
                
    DEX educational extensible operating system 1.0 Beta
    Copyright (C) 2004  Joseph Emmanuel DL Dayo

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
   ==========================================================================
1*/

#include "console.h"


/*A console mode get string function terminates
upon receving \r */
void getstring(char *buf,DEX32_DDL_INFO *dev)
  {
    memset(buf, 0, 255);
    unsigned int i=0;
    char c;
    const int commands_len = 47;
    const char *commands[commands_len];
    commands[0] = "cal";
    commands[1] = "cat";
    commands[2] = "cd";
    commands[3] = "cls";
    commands[4] = "copy";
    commands[5] = "cpuid";
    commands[6] = "date";
    commands[7] = "del";
    commands[8] = "demo_graphics";
    commands[9] = "dir";
    commands[10] = "echo";
    commands[11] = "exit";
    commands[12] = "fgman";
    commands[13] = "files";
    commands[14] = "find";
    commands[15] = "help";
    commands[16] = "kill";
    commands[17] = "libinfo";
    commands[18] = "loadmod";
    commands[19] = "ls";
    commands[20] = "lsext";
    commands[21] = "lsdev";
    commands[22] = "lsmod";
    commands[23] = "lspcut";
    commands[24] = "mem";
    commands[25] = "mkdir";
    commands[26] = "mount";
    commands[27] = "mouse";
    commands[28] = "meminfo";
    commands[29] = "newconsole";
    commands[30] = "off";
    commands[31] = "path";
    commands[32] = "pause";
    commands[33] = "pcut";
    commands[34] = "procinfo";
    commands[35] = "procs";
    commands[36] = "rempcut";
    commands[37] = "ren";
    commands[38] = "rmdir";
    commands[39] = "run";
    commands[40] = "set";
    commands[41] = "shutdown";
    commands[42] = "time";
    commands[43] = "type";
    commands[44] = "umount";
    commands[45] = "unload";
    commands[46] = "use";
    commands[47] = "ver";
    do
    {
    c=getch();
    if (c=='\t') {
        char temp[256];
        strcpy(temp, buf);
        char *u = strtok(temp, " ");
        char *last_input;
        strcpy(last_input, u);
        int input_len = 0;
        int buf_len = 0;
        while (u != 0) {
            input_len++;
            buf_len = strlen(u);
            strcpy(last_input, u);
            u = strtok(0, " ");
        }

        char sub_buf[buf_len];
        if (input_len == 1) {
            for (int index=0; i<commands_len; ++index) {
                if (strlen(commands[index] > buf_len)) {
                    memcpy(sub_buf, commands[index], buf_len);
                    sub_buf[buf_len] = 0;
                    if (strcmp(last_input, sub_buf) == 0) {
                        for (int j=buf_len; j<strlen(commands[index]); ++j) {
                            Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),commands[index][j],Dex32GetAttb(dev));
                            buf[i] = commands[index][j];
                            i++;
                            Dex32SetX(dev,Dex32GetX(dev)+1);  
                        }
                        Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),' ',Dex32GetAttb(dev));
                        buf[i] = ' ';
                        i++;
                        Dex32SetX(dev,Dex32GetX(dev)+1);  
                    }
                    break;
                }
            }
        } else if (input_len > 1) {
            vfs_node *dptr = current_process->workdir;
            vfs_node *buffer;
            int totalfiles = vfs_listdir(dptr, 0, 0);
            buffer = (vfs_node*) malloc(totalfiles * sizeof(vfs_node));
            totalfiles = vfs_listdir(dptr, buffer, totalfiles*sizeof(vfs_node));
            qsort(buffer, totalfiles, sizeof(vfs_node), console_ls_sortname);

            for (int index=0; index<totalfiles; ++index) {
                if (strlen(buffer[index].name) > buf_len) {
                    char fname[255];
                    strcpy(fname, buffer[index].name);
                    fname[24] = 0;
                    memcpy(sub_buf, fname, buf_len);
                    sub_buf[buf_len] = 0;
                    if (strcmp(last_input, sub_buf) == 0) {
                        while (buf_len < strlen(fname)) { // why wont for loops work
                            Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),fname[buf_len],Dex32GetAttb(dev));
                            buf[i] = fname[buf_len];
                            i++;
                            Dex32SetX(dev,Dex32GetX(dev)+1);   
                            buf_len++;
                        }
                        Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),' ',Dex32GetAttb(dev));
                        buf[i] = ' ';
                        i++;
                        Dex32SetX(dev,Dex32GetX(dev)+1);  
                        break;
                    }
                }
            }
            free(buffer);
        }
        memset(last_input, 0, buf_len);
        memset(sub_buf, 0, buf_len);
    }
    else if (c=='\r'||c=='\n'||c==0xa) break;
    else if (c=='\b' || (unsigned char)c == 145)
       {
       if(i>0)
        {
        buf[i] = '\0';
        i--;

        if (Dex32GetX(dev)==0)
             {
              Dex32SetX(dev,79);
              if (Dex32GetY(dev)>0) Dex32SetY(dev,Dex32GetY(dev)-1);
             }
             else
        Dex32SetX(dev,Dex32GetX(dev)-1);     
        Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),' ',Dex32GetAttb(dev));
        };
       }
       else
       {

        if (i<256)  //maximum command line is only 255 characters
         {
            Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),buf[i]=c,Dex32GetAttb(dev));
            i++;
            Dex32SetX(dev,Dex32GetX(dev)+1);     
            
            if (Dex32GetX(dev)>79) {
                Dex32SetX(dev,0);
                Dex32NextLn(dev);
            };
         };
       };

     Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),' ',Dex32GetAttb(dev));
     update_cursor(Dex32GetY(dev),Dex32GetX(dev));
    } while (c!='\r');
    
    Dex32SetX(dev,0);
    Dex32NextLn(dev);
    buf[i]=0;
  };

/*Show information about memory usage. This function is also useful
  for detecting memory leaks*/
void meminfo()
  {
    DWORD totalbytes = totalpages * 0x1000;
    DWORD freebytes = totalbytes - (totalpages - stackbase[0])* 0x1000;
    printf("=================Memory Information===============\n");
    printf("Pages available     : %10u pages\n",stackbase[0]);
    printf("Total Pages         : %10d pages\n",totalpages);
    printf("Total Memory        : %10u bytes (%10d KB)\n",totalbytes, totalbytes / 1024);
    printf("Free Memory         : %10u bytes (%10d KB)\n",freebytes, freebytes / 1024);
    printf("used pages          : %10d pages (%10d KB)\n",totalpages-stackbase[0],
    (totalpages-stackbase[0])*0x1000);
  };

int delfile(char *fname)
  {
    int sectors;
    file_PCB *f=openfilex(fname,FILE_WRITE);
    return fdelete(f);
  };

int user_fork()
{
    int curval = current_process->processid;
    int childready = 0, retval = 0;
    int hdl;
    int id;
    DWORD flags;
    #ifdef DEBUG_FORK
    printf("user_fork called\n");
    #endif
    
    //enable interrupts since we want the process dispatcher to take control
    storeflags(&flags);
    startints();
   
    hdl = pd_forkmodule(current_process->processid);
    
    taskswitch();  
    id = pd_dispatched(hdl);
    //while (!(id=pd_dispatched(hdl)))
    //  ;
    
    if (curval != current_process->processid) //this is the child
      {
        //If this is the child process, the processid when this function
        //was called is not equal to the current processid.
        retval = 0;
      };
      
    if (curval == current_process->processid) // this is the parent
      {
        pd_ok(hdl);
        retval = id;
      };
      
      restoreflags(flags);
      return retval;
};

int user_execp(char *fname,DWORD mode,char *params)
 {
  DWORD id,size;
  char *buf;
  vfs_stat filestat;
  file_PCB *f=openfilex(fname,0);
  
  if (f!=0)
  {
      fstat(f,&filestat);
      size = filestat.st_size;
      buf=(char*)malloc(filestat.st_size+511);
      //tell the vfs to increase buffersize to speed up reads
      vfs_setbuffer(f,0,filestat.st_size,FILE_IOFBF);
      if (fread(buf,size,1,f)==size)
      {
        char temp[255];
        #ifdef DEBUG_USER_PROCESS
        printf("execp(): adding module..\n");
        #endif
        int hdl= addmodule(fname,buf,userspace,mode,params,showpath(temp),getprocessid());
        
        #ifdef DEBUG_USER_PROCESS
        printf("execp(): done.\n");
        #endif

        taskswitch();
        #ifdef DEBUG-USER_PROCESS
        printf("execp(): parent waiting for child to finish\n");
        #endif
        
        while (!(id=pd_ok(hdl))) ; //process already running?
        fg_setmykeyboard(id);
        dex32_waitpid(id,0);
        //dex32_wait();
        fg_setmykeyboard(getprocessid());
      };
     free(buf);
     fclose(f);
     return id;
  };
  return 0;
 ;};

int exec(char *fname,DWORD mode,char *params)
 {
  DWORD id;
  char *buf;
  file_PCB *f=openfilex(fname,0);

  if (f!=0)
  {
     DWORD size;
     char temp[255];
     vfs_stat fileinfo;
     fstat(f,&fileinfo);
     size = fileinfo.st_size;
     
     buf=(char*)malloc(size+511);
     
     if (fread(buf,size,1,f)==size)
     id=dex32_loader(fname,buf,userspace,mode,params,showpath(temp),getprocessid());


     free(buf);
     fclose(f);
     return id;
  };
  return 0;
 ;};


int user_exec(char *fname,DWORD mode,char *params)
 {
  DWORD id;
  char *buf;
  file_PCB *f=openfilex(fname,FILE_READ);

  if (f!=0)
  {
     int hdl,size;
     char temp[255];
     vfs_stat fileinfo;
     fstat(f,&fileinfo);
     size = fileinfo.st_size;
     buf=(char*)malloc(size+511);
     fread(buf,size,1,f);
     hdl = addmodule(fname,buf,userspace,mode,params,showpath(temp),getprocessid());
     while (!pd_ok(hdl)) ;
     
     free(buf);
     fclose(f);
     return id;
  };
  return 0;
 ;};

int loadDLL(char *name,char *p)
{
 file_PCB *handle;
 int fsize; vfs_stat filestat;
 int hdl,libid;
 char *buf;

 handle=openfilex(name,FILE_READ);
 if (!handle) return -1;
 fstat(handle,&filestat);
 vfs_setbuffer(handle,0,filestat.st_size,FILE_IOFBF);
 //get filesize and allocate memory
 fsize= filestat.st_size;
 buf=(char*)malloc(fsize);
 
 //load the file from the disk 
 fread(buf,fsize,1,handle);
 
 /*Tell the process dispatcher to map the file into memory and
   create data structures necessary for managing dynamic libraries*/
   
 hdl = addmodule(name,buf,lmodeproc,PE_USERDLL,p,0,getprocessid());
 
 //wait until the library has been loaded before we continue since addmodule returns immediately
 while (!(libid = pd_ok(hdl)));
 
 //done!
 free(buf);

 //close the file
 fclose(handle);
 return libid;
};

void loadfile(char *s,int,int);

void loadlib(char *s)
  {
 char *buf;
   DWORD size;
 loadDLL(s,0);
 };


int console_showfile(char *s,int wait)
 {
   char *buf;
   DWORD size;
   file_PCB *handle;
   vfs_stat fileinfo;
   int i;
   DEX32_DDL_INFO *myddl;
   handle=openfilex(s,FILE_READ);
   if (!handle) return -1;
   fstat(handle,&fileinfo);
   size = fileinfo.st_size;
   buf=(char*)malloc(size);
   textbackground(BLUE);
   myddl = Dex32GetProcessDevice();
   printf("Name: %s  size of file: %d\n",s,size);
   textbackground(BLACK);
   fread(buf,size,1,handle);
    for (i=0;i<size;i++)
       {
           if (buf[i]!='\r') 
           printf("%c",buf[i]);

           if (myddl->lines%25==0)  
              { 
                 char c;
                 printf("\nPress any key to continue, 'q' to quit\n");
                 c=getch();
                 if (c=='q') break;
             };
       };
    fclose(handle);
    free(buf);
    return 1;
};
//creates a virtual console for a process
DWORD alloc_console()
{
dex32_commit(0xB8000,1,current_process->pagedirloc,PG_WR)
;};


void console()
 {
   
    console_main();
 };


void prompt_parser(const char *promptstr,char *prompt)
  {
  int i,i2=0,i3=0;
  char command[10],temp[255];
  strcpy(prompt,"");
  for (i=0;promptstr[i]&&i<255;i++)
    {
      if (promptstr[i]!='%')
          //add to the prompt
          {
              prompt[i2]=promptstr[i];
              i2++;
              prompt[i2]=0;
          }
          else
          {
            if (promptstr[i+1]!=0)
              {
                if (promptstr[i+1]=='%')
                   {
                   prompt[i2]='%';
                   i2++;
                   prompt[i2]=0;
                   i+=2;
                   continue;
                   };
              };
            i3=0;
            for (i2=i+1;promptstr[i2]&&i2<255;i2++)
              {

                  if (promptstr[i2]=='%'||i3>=10) break;
                  command[i3]=promptstr[i2];
                  i3++;

              };
              i=i2;
              command[i3]=0;
              if (strcmp(command,"cdir")==0)
                {
                  strcat(prompt,showpath(temp));
                  i2=strlen(prompt);
                };
          };

    };

  };
  


/* ==================================================================
   console_ls(int style):
   
   *list the contents of the current directory to the screen
    style = 1      : list format 
    sty;e = others : wide format  

*/

/*An auxillary function for qsort for comparing two elements*/
int console_ls_sortsize(vfs_node *n1,vfs_node *n2)
{
    if (n1->size > n2->size) return -1;
    if (n2->size > n1->size) return 1;
    return 0;
};

/*An auxillary function for qsort for comparing two elements*/
int console_ls_sortname(vfs_node *n1,vfs_node *n2)
{
    if ( (n1->attb & FILE_DIRECTORY) && !(n2->attb & FILE_DIRECTORY))
        return -1;

    if ( !(n1->attb & FILE_DIRECTORY) && (n2->attb & FILE_DIRECTORY))
        return 1;
        
    return strsort(n1->name,n2->name);
};

/*lists the files in the current directory to the console screen*/
void console_ls(int style, int sortmethod)
{

    vfs_node *dptr=current_process->workdir;
    vfs_node *buffer;
    int totalbytes=0,freebytes=0;
    int totalfiles=0,i;
    char cdatestr[20],mdatestr[20],temp[20];
    
    //obtain total number of files
    totalfiles = vfs_listdir(dptr,0,0);
    
    buffer = (vfs_node*) malloc( totalfiles * sizeof(vfs_node));

    //Place the list of files obtained from the VFS into a buffer
    totalfiles = vfs_listdir(dptr,buffer,totalfiles* sizeof(vfs_node));     
    
    //Sort the list
    if (sortmethod == SORT_NAME)
        qsort(buffer,totalfiles,sizeof(vfs_node),console_ls_sortname);
    else
    if (sortmethod == SORT_SIZE)
        qsort(buffer,totalfiles,sizeof(vfs_node),console_ls_sortsize);
        
    textbackground(BLUE);
    textcolor(WHITE);
    
    if (style==1)
        printf("%-25s %10s %14s %14s\n","Filename","Size(bytes)","Attribute","Date Modified");
        
    textbackground(BLACK);

    for (i=0; i < totalfiles; i++)
    {
        char fname[255];   
        if (style==0) //wide view style
        {
            
            if (buffer[i].attb&FILE_MOUNT)
                textcolor(LIGHTBLUE);
            else
                if (buffer[i].attb&FILE_DIRECTORY)
                    textcolor(GREEN);
                else
                    if (buffer[i].attb&FILE_OEXE)
                        textcolor(YELLOW);
                    else
                    textcolor(WHITE);

            strcpy(fname,buffer[i].name);
            fname[24]=0;
            printf("%-25s ",fname);
            totalbytes+=buffer[i].size;
            
            if ( (i+1)%3==0 && (i+1 < totalfiles) ) printf("\n");

        };

        if (style==1) //list view style
        {
            if (buffer[i].attb&FILE_MOUNT)
                textcolor(LIGHTBLUE);
            else
                if (buffer[i].attb&FILE_DIRECTORY)
                    textcolor(GREEN);
                else
                     if (buffer[i].attb&FILE_OEXE)
                        textcolor(YELLOW);
                    else
                    textcolor(WHITE);
                    
            strcpy(fname,buffer[i].name);
            fname[24]=0;
            printf("%-25s ",fname);
            
            textcolor(WHITE);
            printf("%10d %14s %14s\n",buffer[i].size,
            vfs_attbstr(&buffer[i],temp),datetostr(&buffer[i].date_modified,
                       mdatestr));
                       
            totalbytes+=buffer[i].size;


            //try to make it fit the screen
            if ((i+1) % 23==0) 
            {
                char c;
                printf("Press Q to quit or any other key to continue ...");
                c=getch();
                printf("\n");
                if (c=='q'||c=='Q') break;
            };
        };       
    };
    
    textcolor(WHITE);
    printf("\nTotal Files: %d  Total Size: %d bytes\n", totalfiles, totalbytes);
    free(buffer);
    
};
// http://www.stoimen.com/blog/2012/04/24/computer-algorithms-how-to-determine-the-day-of-the-week/

int get_century_code(int year) {
    int century = 0;
    int code = 6;
    while (1) {
        if (century <= year && year < century+100) {
            return code;
        }
        century += 100;
        if (code == 0) code = 6;
        else code -= 2;
    }
}

int get_month_code(int month, int year) {
    int code[12] = {1,4,4,0,2,5,0,3,6,1,4,6};

    if (year % 4 == 0 && year % 100 != 0 && (month == 1 || month == 2)) {
        return code[month-1]-1;
    } else {
        return code[month-1];
    }
}

int starting_day(int month, int year) {
    int day = (((year % 100)/4)+1+get_month_code(month, year)
    +get_century_code(year)+(year % 100)) % 7;

    return day;
}

void cal(int month, int year) {
    if (month != -1) {
        switch (month) {
            case 1: printf("\tJanuary"); break;
            case 2: printf("\tFebruary"); break;
            case 3: printf("\t\tMarch"); break;
            case 4: printf("\t\tApril"); break;
            case 5: printf("\t\tMay"); break;
            case 6: printf("\t\tJune"); break;
            case 7: printf("\t\tJuly"); break;
            case 8: printf("\tAugust"); break;
            case 9: printf("\tSeptember"); break;
            case 10: printf("\tOctober"); break;
            case 11: printf("\tNovember"); break;
            case 12: printf("\tDecember"); break;
        }
        printf(" %d\n", year);
        printf("Su Mo Tu We Th Fr Sa\n");

        int days_in_months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int start_day = starting_day(month, year);
        int day = 1;
        int days_in_month = days_in_months[month-1];
        if (year % 4 == 0 && year % 100 != 0 && month == 2) {
            days_in_month++;
        }
        for (int i=0; i<start_day-1; ++i) {
            printf("   ");
            day++;
        }
        for (int date=1; date<days_in_month+1; ++date) {
            if (date == time_systime.day && month == time_systime.month && year == time_systime.year) {
                textbackground(WHITE);
                textcolor(BLACK);
            }
            if (date < 10) {
                printf(" ");
            }
            printf("%d", date);
            textbackground(BLACK);
            textcolor(WHITE);
            printf(" ");
            if (day % 7 == 0) {
                printf("\n");
            }
            day++;
        }
        if (day != 7) {
            printf("\n");
        }
    } else {
        printf("                             %d\n", year);
        char *months[12] = {
            "       January", "              February", "               March",
            "       April", "                  May", "                   June",
            "        July", "                 August", "              September",
            "      October", "               November", "              December"
        };
        char *days = "Su Mo Tu We Th Fr Sa  ";
        int days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int start_day[12] = {0}; memset(start_day, 0, 12);
        int day[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
        int spaced[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
        int date[12] = {1,1,1,1,1,1,1,1,1,1,1,1};

        if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) {
            days_in_month[1]++;
        }

        for (int month=0; month<12; ++month) {
            start_day[month] = starting_day(month+1, year);
        }

        for (int row=0; row<4; ++row) {
            for (int col=0; col<3; ++col) {
                int month = row*3+col;
                printf("%s", months[month]);
            }
            printf("\n");
            
            for (int col=0; col<3; ++col) {
                printf("%s", days);
            }
            printf("\n");
            for (int i=0; i<6; ++i) {
                int done = 1;
                for (int col=0; col<3; ++col) {
                    int month = row*3+col;
                    if (date[month] <= days_in_month[month]) {
                        done = 0;
                        break;
                    }
                }
                if (done) {
                    continue;
                }

                for (int col=0; col<3; ++col) {
                    int month = row*3+col;

                    if (!spaced[month]) {
                        for (int i=0; i<start_day[month]-1; ++i) {
                            printf("   ");
                            day[month]++;
                        }
                        spaced[month] = 1;
                    }

                    if (date[month] <= days_in_month[month]) {
                        int first = 1;
                        while (first || day[month] % 7 != 0) {
                            first = 0;
                            if (date[month] == time_systime.day && month+1 == time_systime.month && year == time_systime.year) {
                                textbackground(WHITE);
                                textcolor(BLACK);
                            }
                            if (date[month] < 10) {
                                printf(" ");
                            }

                            printf("%d", date[month]);
                            textbackground(BLACK);
                            textcolor(WHITE);   
                            printf(" ");

                            day[month]++;
                            date[month]++;
                            if (date[month] > days_in_month[month]) {
                                if (day[month]%7 != 0) {
                                    for (int j=0; j<7-(day[month]%7); ++j) {
                                        printf("   ");
                                    }
                                }
                                break;
                            }
                        }
                    } else {
                        printf("                     ");
                    }
                    printf(" ");
                        
                }
                printf("\n");

            }
            printf("\n");
            if (row < 3) {
                getch();
            }
        }
    }
}

/* ==================================================================
   console_execute(const char *str):
   * This command is used to execute a console string.

*/
int console_execute(const char *str)
{
  char temp[512];
  char *u;
  int command_length = 0;
  signed char mouse_x,mouse_y,last_mouse_x=0,last_mouse_y=0;
  
  //make a copy so that strtok wouldn't ruin str
  strcpy(temp,str);
  u=strtok(temp," ");
  
  if (u==0) return;
  
  command_length = strlen(u);    
    
    //check if a pathcut command was executed
    if (u[command_length - 1] == ':') 
                {
                    char temp[512];
                    sprintf(temp,"cd %s",u);            
                    console_execute(temp); 
                }
                else
    if (strcmp(u,"fgman")==0)
                {
                    fg_set_state(1);
                }
                else
    if (strcmp(u,"mouse")==0)
                {
                  while (!kb_ready()){
                    get_mouse_pos(&mouse_x,&mouse_y);
                    printf("Mouse (x,y): %d %d\n",mouse_x, mouse_y);
                    while ((last_mouse_x == mouse_x) && (last_mouse_y==mouse_y))
                    {
                      get_mouse_pos(&mouse_x,&mouse_y);
                    }
                    last_mouse_x=mouse_x;
                    last_mouse_y=mouse_y; 
                  }
                }
                else
    if (strcmp(u,"shutdown")==0)
                {
                    sendmessage(0,MES_SHUTDOWN,0);
                }
                else
    if (strcmp(u,"procinfo")==0)
                {
                   int pid;             
                   u=strtok(0," ");
                   if (u!=0)
                   {
                       pid = atoi(u);
                       show_process_stat(pid);
                   };
                }
                else
    if (strcmp(u,"meminfo")==0)
                mem_interpretmemory(memory_map,map_length);
                else
    if (strcmp(u,"pause")==0)
                {
                printf("press any key to continue or 'q' to quit..\n");
                if (getch()=='q') return -1;
                }
                else
    if (strcmp(u,"lspcut")==0)
                vfs_showpathcuts();
                else
    if (strcmp(u,"pcut")==0)
                {
                   char *u2,*u3;
                   u2 = strtok(0," ");
                   u3 = strtok(0," ");
                   if (u2!=0)
                      {
                          if (vfs_addpathcut(u2,u3)==-1)
                            printf("Invalid Pathcut specified\n");
                              else
                            printf("pathcut added.\n");                                
                      }
                   else
                      printf("Wrong number of parameters specified\n");
                }
                else
    if (strcmp(u,"rmdir")==0)
                {
                char *u2 = strtok(0," ");
                if (u2!=0)
                    {
                        char c;                
                        printf("*Warning!* This will delete all files and subdirectories!\n");
                        printf("Do you wish to continue? (y/n):");
                        c = getch();
                        if (c == 'y')
                           {
                              printf("please wait..\n");                        
                              if (rmdir(u2)!=-1)
                                printf("Remove directory successful!\n");
                              else
                                printf("Error while removing directory.\n");
                           }
                        else
                           printf("Remove directory cancelled.\n");
                    }
                   else
                   printf("Invalid parameter.\n"); 
                }
                else
    if (strcmp(u,"rempcut")==0)
                {
                   char *u2;
                   u2 = strtok(0," ");
                   if (u2!=0)
                     {
                     if (vfs_removepathcut(u2)==-1)
                        printf("Invalid Pathcut or pathcut not found\n");
                           else
                        printf("pathcut removed.\n");   
                     
                     }
                   else
                     printf("Wrong number of parameters specified\n");
                     
                }
                else
    if (strcmp(u,"newconsole")==0)
                {
                //create a new console         
                   console_new();
                   printf("new console thread created.\n");                   
                }
                else  
    if (strcmp(u,"ver")==0) {
		printf("%s",dex32_versionstring);
                }
                else
    if (strcmp(u,"cpuid")==0)
                {
                   hardware_cpuinfo mycpu;
                   hardware_getcpuinfo(&mycpu);
                   hardware_printinfo(&mycpu);
                }
                else            
    if (strcmp(u,"exit")==0)
                {
                  fg_exit();
                  exit(0);              
                }
                else  
    if (strcmp(u,"echo")==0)
                {
                  u=strtok(0,"\n");
                  
                  if (u!=0)              
                      printf("%s\n",u);
                }
                else  
    if (strcmp(u,"use")==0)
                {
                u=strtok(0," ");
                if (extension_override(devmgr_getdevicebyname(u),0)==-1)
                    {
                        printf("Unable to install extension %s.\n",u);                
                    };            
                }
                else        
    if (strcmp(u,"off")==0)
                dex32apm_off();
                else
    if (strcmp(u,"files")==0)
                 file_showopenfiles();
                else
    if (strcmp(u,"find")==0)
                {
                u=strtok(0," ");
                if (u!=0)
                  findfile(u);
                }
              else
    if (strcmp(u,"kill")==0)
                {
                u=strtok(0," ");
                if (u!=0)
                      dex32_killkthread_name(u);
                }
                else
    if (strcmp(u,"procs")==0||strcmp(u,"ps")==0)
                show_process();
                else
    if (strcmp(u,"cls")==0)
               clrscr();
                else
    if (strcmp(u,"help")==0)
               //console_execute("type /icsos/icsoshlp.txt");
               console_execute("type /icsos/icsoshlp.txt");
                else
    if (strcmp(u,"umount")==0)
             {
               char *u =strtok(0," ");
               if (u!=0)
                {
                 if (vfs_unmount_device(u)==-1)
                    printf("umount failed.\n");
                 else
                   printf("%s umounted.\n",u);
                } 
                  else
                printf("missing parameter.\n");                    
              }
              else
   if (strcmp(u,"mount")==0)
               {
               char *fsname,*devname,*location;
               fsname=strtok(0," ");
               devname=strtok(0," ");
               location=strtok(0," ");
               
               if (vfs_mount_device(fsname,devname,location)==-1)
                 printf("mount not successful.\n");
                   else
                 printf("mount successful.\n");  
               //fat12_mount_root(root,floppy_deviceid);
               }
              else
    if (strcmp(u,"path")==0)
               {
                char temp[255];
                printf("%s\n",showpath(temp));
               }
                else
    if (strcmp(u,"lsmod")==0)
                showlibinfo();
                else
    if (strcmp(u,"mem")==0)
                meminfo();
                else
    if (strcmp(u,"mkdir")==0)
               {
                u=strtok(0," ");
                if (u!=0)
                   if (mkdir(u)==-1)
                     printf("mkdir failed.\n");
               }
                else       
    if (strcmp(u,"run")==0)
               {
               u=strtok(0," ");
               if (u!=0)
                  {
                  if (script_load(u)==-1)
                      {
                         printf("console: Error loading script file.\n");
                      };            
                  }
               }
               else    
    if (strcmp(u,"ls")==0||strcmp(u,"dir")==0)
               {
               int style=0, ordering = 0;
               
               u=strtok(0," ");
               
               if (u!=0)
               {
                   do {           
                   if (strcmp(u,"-l")==0) style=1;
                   if (strcmp(u,"-oname")==0) ordering  = 0;
                   if (strcmp(u,"-osize")==0) ordering  = 1;
                   u=strtok(0," ");
                   } while (u!=0);
               };
               
               console_ls(style, ordering);
               }
              else
     if (strcmp(u,"del")==0)
              {
              int res;
               u=strtok(0," ");
               if (u!=0)
                  {
                           char *u3=strtok(0," ");
                           if (u3==0)
                           {
                               delfile(u);
                               printf("file deleted.\n");
                           }
                            else
                           printf("invalid parameter.\n");

                  }
                     else
                  printf("missing parameter.\n");

              }

              else
    if (strcmp(u,"ren")==0)
              {
              char *u2,*u3;
               u2=strtok(0," ");
               u3=strtok(0," ");               
               if (u2!=0&&u3!=0)
                  {
                     if (rename(u2,u3)) printf("file renamed.\n");
                        else
                         printf("error renaming file.\n");
                  }
                    else
                printf("missing parameter.\n");    
              }
              else
    if ( strcmp(u,"type")==0 || strcmp(u,"cat")==0 )
              {
               u=strtok(0," ");
               if (u!=0)
                  {
                   if (console_showfile(u,0)==-1)
                    printf("error opening file.\n");
                  }
                  else
               printf("missing parameter.\n");

              }
              else
   if (strcmp(u,"copy")==0)
              {
              u=strtok(0," ");
              if (u!=0) 
                   {
                      char *u2 = strtok(0," ");
                      if (u2!=0) 
                          {
                            if (fcopy(u,u2) == -1)
                                {
                                    printf("copy failed. Error while copying. Destination\n");
                                    printf("directory might not be present.\n");
                                };
                          };  
                   };
              }
              else              
   if (strcmp(u,"cd")==0)
              {
              u=strtok(0," ");
              if (u!=0)  {
               if (!changedirectory(u))
               printf("cd: Cannot find directory\n");
                         } else
                changedirectory(0); //go to working directory 
              } else
   if (strcmp(u,"loadmod")==0)
              {
               u=strtok(0," ");
               if (u!=0)
                {
               if (loadDLL(u,str)==-1)
                  printf("Unable to load %s.\n",u);
                    else
                  printf("Load module Successful.\n");  
                }
                  else
               printf("missing parameter.\n");

              }
              else
    if (strcmp(u,"lsdev")==0)
             {
              devmgr_showdevices();
             }
             else
    if (strcmp(u,"lsext")==0)
    			 {
              extension_list();
             }
             else
    if (strcmp(u,"libinfo")==0)
             {
              u=strtok(0," ");
              module_listfxn(u);
             }
             else
    if (strcmp(u,"time")==0)
             {
             printf("%d/%d/%d %d:%d.%d (%d total seconds since 1970)\n",time_systime.day,
                      time_systime.month, time_systime.year,
                      time_systime.hour, time_systime.min,
                      time_systime.sec,time());
             }
             else
    if (strcmp(u,"set")==0)
             {
              u=strtok(0," ");
              if (u==0)
                 {
                   env_showenv();
                 }
                  else
                 {
                    char *name  = strtok(u,"=");
                    char *value = strtok(0,"\n");
                    env_setenv(name,value,1);
                 }; 
             }
             else         
    if (strcmp(u,"unload")==0)
    			 {
             u=strtok(0," ");
             if (u!=0)
             	{
	             if (module_unload_library(u)==-1)
                printf("Error unloading library");
   	         };
             }
             else
    if (strcmp(u,"demo_graphics")==0)
              {
               demo_graphics();
              }
    
              else
    if (strcmp(u, "cal") == 0) {
        char* firstParam;
        u = strtok(0, " ");
        firstParam = u;
        u = strtok(0, " ");
        if (firstParam != 0) {
            if (u != 0) {
                // two params
                cal(atoi(firstParam), atoi(u));
            } else {
                // one params
                if (strcmp(firstParam, "-h") == 0) {
                    printf("The command shows a formatted calendar\n");
                    printf("Possible options:\n");
                    printf("\tcal -h - shows this message\n");
                    printf("\tcal <year> - shows the calendar for the whole specific year\n");
                    printf("\tcal <month> <year> - shows the the calendar for that month of that year\n");
                } else {
                    printf("\nYear: %s\n", firstParam);
                    cal(-1, atoi(firstParam));
                }

            }
        } else {
            // no params
            cal(time_systime.month, time_systime.year);
        }
    } else
    if (u[0]=='$')
             {
               int i, devid;
               char devicename[255],*cmd;
               for (i=0;i<20&&u[i+1];i++)
                    {
                      devicename[i] = u[i+1];
                    };
                    
               devicename[i] = 0;
               
               printf("sending command to %s\n",devicename);
               devid = devmgr_finddevice(devicename);
               
               if (devid != -1)
               {
               if (devmgr_sendmessage(devid,DEVMGR_MESSAGESTR,str)==-1)
                   printf("console: send_message() failed or not supported.\n");
               }
                  else
               printf("console: cannot find device.\n");   

             }
             else
             {
               if (u!=0)
                 {
                  
                  if (!user_execp(u,0,str))
                      printf("console32: undefined console command.\n");

                 }

              }
//normal termination
return 1;
};

int console_new()
{
         //create a new console         
         char consolename[255];
         sprintf(consolename,"dex32_console(%d)",console_first);    
         return createkthread((void*)console,consolename,200000);
};

void console_main()
  {
    DEX32_DDL_INFO *myddl=0;
    fg_processinfo *myfg;
    char s[256]="";
    char temp[256]="";
    char last[256]="";
    char console_fmt[256]="%cdir% %% ";
    char console_prompt[256]="cmd >";
    
    DWORD ptr;
    
    myddl =Dex32CreateDDL();    

    
    Dex32SetProcessDDL(myddl,getprocessid());
    myfg = fg_register(myddl,getprocessid());
    fg_setforeground( myfg->id );


    clrscr();
    
    
          
    strcpy(last,"");
    
    
    if (console_first == 0) script_load("/icsos/autoexec.bat");
    
    console_first++;  
    do {
    textcolor(WHITE);
    textbackground(BLACK);
    prompt_parser(console_fmt,console_prompt);
    
    textcolor(LIGHTBLUE);
    printf("%s",console_prompt);
    textcolor(WHITE);
    
    if (strcmp(s,"@@")!=0&&
        strcmp(s,"!!")!=0)
    strcpy(last,s);
    
    getstring(s,myddl);
   
    if (strcmp(s,"!")==0)
               sendtokeyb(last,&_q);
               else
    if (strcmp(s,"!!")==0)
              {
               sendtokeyb(last,&_q);
               sendtokeyb("\r",&_q);
              }
               else   
    console_execute(s);
    } while (1);
  ;};

