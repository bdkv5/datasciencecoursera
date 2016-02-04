#ifndef _Play_Song_H
#define _Play_Song_H

#include "Main.H"


//------- Public Constant definitions --------------------------------
typedef enum {data_send_1,load_buf_2,find_cluster_2,data_idle_1,data_send_2,load_buf_1,find_cluster_1,data_idle_2} state;


// ------ Public function prototypes -------------------------------
void Play_Song(uint32_t Start_Cluster);

#endif