//
//  filestruct.h
//  SimpleThreadedFS
//
//  Created by Sankarsan Seal on 23/07/16.
//  Copyright © 2016 Sankarsan Seal. All rights reserved.
//

#ifndef filestruct_h
#define filestruct_h

typedef struct  fshead
{
    int blocksize;
    int free_block_index;
    int root_block_index;
};

typedef struct block_headed
{
    int type; // 0--empty, 1--directory, 2--file, 3--extension 
    int ismore;
    int next_index;
    
} BH;

typedef struct details
{
    int index;
    time_t created;
    time_t modified;
    char name[32];
    
} DETAILS;


#endif /* filestruct_h */
