//
//  main.c
//  SimpleThreadedFS
//
//  Created by Sankarsan Seal on 23/07/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "filestruct.h"
char path_to_fsfile[1024];
char path_to_lockfile[1024];
int size_in_MB;
int size_in_bytes;
int blocksize;
int no_of_blocks;
int no_of_directory_entry;
BH empty_block_header;
DETAILS empty_details;

void initialize_bh()
{
    empty_block_header.ismore=0;
    empty_block_header.next_index=0;
    empty_block_header.type=0;
}

void initialize_details()
{
    empty_details.created=time(NULL);
    empty_details.index=0;
    empty_details.modified=time(NULL);
    empty_details.name[0]='\0';
    
}


void  cleanup(int signaltype)
{
    
    printf("Exiting..\n");
   // return NULL;
}

void user_home_creation(FILE * fs)
{
    int i;
    int no_of_user;
    char name[32];
    fseek(fs,0,SEEK_SET);
    initialize_bh();
    empty_block_header.type=1;
    fwrite(&empty_block_header,sizeof(BH),1,fs);
    printf("Enter the number of users you going to create :");
    scanf("%d",&no_of_user);
    
    printf("Entered no for user %d and No of entries in each block %d\n",no_of_user,no_of_directory_entry);
    for(i=0;i<no_of_directory_entry&&i<no_of_user;i++)
    {
        initialize_details();
        printf("Enter the new user id :");
        scanf("%s",empty_details.name);
    }
    
}



int main(int argc, const char * argv[]) {
    
    int c;
    FILE * fsfile;
    FILE * fslockfile;
    char * blank;
    // insert code here...

    signal(SIGINT,cleanup);
    
    if(argc!=4)
        printf("usage:%s <filesize in MB> <block size in bytes> <filesystem file>",argv[0]);
    else
    {
        size_in_MB=atoi(argv[1]);
        size_in_bytes=size_in_MB<<20;
        
        blocksize=atoi(argv[2]);
        no_of_directory_entry=(blocksize-sizeof(BH))/sizeof(DETAILS);
        
        printf("Size in MB %d Size in bytes %d Block size %d \n",size_in_MB,size_in_bytes,blocksize);
        strcpy(path_to_fsfile,argv[3]);
        sprintf(path_to_lockfile,"%s.lock",argv[3]);
        printf("Path to fs file %s and Path to Lock file %s\n",path_to_fsfile,path_to_lockfile);
        if(!access(path_to_fsfile, F_OK)&&!access(path_to_lockfile,F_OK))
        {
            
            printf("File and Lock file both exist.\n");
            cleanup(SIGINT);
        }
        
        else if(!access(path_to_fsfile,F_OK)&&access(path_to_lockfile,F_OK))
        {
            printf("File exists but lock does not\n");
        }
        else if(access(path_to_fsfile, F_OK)&&!access(path_to_lockfile, F_OK))
        {
            printf("FS File does not exists but lock file exists\n");
            printf("Do you like to remove the lock file?\n 1. Yes\n 2. No\n");
            printf("Your choice:");
            scanf("%d",&c);
            
            
        }
        else
        {
            printf("Both files do not exist.\n");
            fsfile=fopen(path_to_fsfile,"wb+");
            fslockfile=fopen(path_to_lockfile,"wb+");
            blank=(char *)malloc(blocksize*sizeof(char));
            
            fseek(fsfile,size_in_bytes-blocksize,SEEK_SET);
            fwrite(blank,blocksize,1,fsfile);
            
            user_home_creation(fsfile);
            
            
            
            
        }
        
            
        
    }
    
    return 0;
}
