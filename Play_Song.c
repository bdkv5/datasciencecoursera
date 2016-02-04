#include "stdio.h"
#include "main.h"
#include "PORT.H"
#include "SPI.h"
#include "File_System.h"
#include "Read_Sector.h"
#include "Play_Song.h"
#include "print_bytes.h"
extern uint8_t xdata buf1[512];
extern uint8_t xdata buf2[512]; 


state state_g;
void Play_Song(uint32_t Start_Cluster)
{
	
   uint16_t index1, index2;
   //uint8_t buffer1, buffer2;
   uint8_t temp8,play_status=0;
   uint32_t sector ;
   uint32_t sector_base,sector_offset;
   printf("Starting Cluster = %lu\n\r",Start_Cluster);

   sector=First_Sector(Start_Cluster);
   printf("Starting Sector = %lu\n\r",sector);
   sector_base=sector;
   sector_offset=0;
  // buffer1=1;
//   buffer2=0;
   //P3_2=ON;
   YELLOWLED=ON;
   index1=0;
   
   Read_Sector(sector_base+sector_offset, 512, buf1);
//print_memory(512,buf1);
   sector_offset++;
   YELLOWLED=OFF;
   AMBERLED=ON;
   index2=0;
   state_g=data_idle_1;
   
   Read_Sector(sector_base+sector_offset, 512, buf2);
  // print_memory(512,buf2);
   sector_offset++;
   AMBERLED=OFF;
   while((Start_Cluster != 0x0FFFFFFF) &&(SW1==1))
    {

	if ( SW2 == 0)
	 {
      while(SW3 == 1)
	  {
	  }
	 }
   	
	//printf("%ld",Start_Cluster);
	//	printf("\n");
	   //printf("\n check song");
       if ( state_g == load_buf_1)
	   {
	      if (sector_offset > 64)
	      {
		  
		  state_g = find_cluster_1;
		  }

	   }
       if ( state_g == load_buf_2)
	   {
	      if (sector_offset > 64)
	      {
		  
		  state_g = find_cluster_2;
		  }

	   }

    switch(state_g)
    {
 	  case data_idle_1:
	  {
	/*	YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=ON;
		REDLED=OFF;*/
		//	printf("\na");
		     if(DATA_REQ==0)
			{
				state_g=data_send_1;
			}
		break;
	  }
  
     	case data_send_1:
		{
		
		if( DATA_REQ == 0)
        {
		BIT_EN = 1;

		 SPI_Transfer(buf1[index1], &temp8); 
		 BIT_EN = 0;
  	     index1 = index1 + 1;
            if(index1 > 511)
			    {
				   if(index2 > 511)
				      {
						 
					    if( play_status == 3)
						   {
						      play_status = 0 ;
                           }
						else
						   {
						     state_g = load_buf_2;
							 index2=0;
                           }
					  }
			    

       			   else
	     		    {
	      	    
		     		 state_g = data_send_2;
					}
               }
		 }
 	    if( DATA_REQ == 1)
		 {
		    if(index2 > 511)
		     {
		      state_g = load_buf_2 ;
			  index2=0;
			 }
	     }
		
	   break ;
      } 
        

		
		case load_buf_2:
	    {
		  /*YELLOWLED=ON;
   		  AMBERLED=OFF;
		  GREENLED=OFF;
		  REDLED=OFF;*/
		  Read_Sector(sector+sector_offset,512,buf2);
		//  print_memory(512,buf2);
		  sector_offset = sector_offset + 1 ;
		  state_g=data_idle_1;
          break;
     	}

      	case find_cluster_2:
	    {
	/*	YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=OFF;
		REDLED=ON;*/
		Start_Cluster=Find_Next_Clus(Start_Cluster,buf2);
		printf("Cluster = %lu\n\r",Start_Cluster);
		//index2=0;  
		if(Start_Cluster==0x0FFFFFFF)
	     	{
			play_status=3;
			state_g=data_idle_1;
	      	}
	     	else
		    {
	      	sector=First_Sector(Start_Cluster);
			sector_offset=0;
			state_g=data_idle_1;
			}
		break;
  	   }

//===============New==============Entries=========================================================//
case data_idle_2:
	  {
	/*	YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=ON;
		REDLED=OFF;*/
		//	printf("\na");
		     if(DATA_REQ==0)
			{
				state_g=data_send_2;
			}
		break;
	  }
  
     	case data_send_2:
		{
		
		if( DATA_REQ == 0)
        {
		BIT_EN = 1;
		 SPI_Transfer(buf2[index2], &temp8); 
		 BIT_EN = 0;
  	     index2 = index2 + 1;
            if(index2 > 511)
			    {
				   if(index1 > 511)
				      {
					    if( play_status == 3)
						   {
						      play_status = 0 ;
                           }
						else
						   {
						     state_g = load_buf_1;
							 index1=0;
                           }
					  }
			    

       			   else
	     		    {
	      	    
		     		 state_g = data_send_1;
					}
               }
		 }
 	   if( DATA_REQ == 1)
		 {
		    if(index1 > 511)
		     {
		      state_g = load_buf_1 ;
			  index1=0;
			 }
	     }
	   break ;
	   
      } 
        

		
		case load_buf_1:
	    {
		 /* YELLOWLED=ON;
   		  AMBERLED=OFF;
		  GREENLED=OFF;
		  REDLED=OFF;*/
		  Read_Sector(sector+sector_offset,512,buf1);
   		//	print_memory(512,buf1);
		  sector_offset = sector_offset + 1 ;
		  state_g=data_idle_2;
          break;
     	}

      	case find_cluster_1:
	    {
	/*	YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=OFF;
		REDLED=ON;*/
		Start_Cluster=Find_Next_Clus(Start_Cluster,buf1);
		printf("Cluster = %lu\n\r",Start_Cluster);
		  if(Start_Cluster==0x0FFFFFFF)
	     	{
			play_status=3;
			state_g=data_idle_2;
	      	}
	     	else
		    {
	      	sector=First_Sector(Start_Cluster);
			sector_offset=0;
			state_g=data_idle_2;
			}
		break;
  	   }
    }



    }
  }
  
	  /*	
	while((Start_Cluster != 0x0FFFFFFF) && (SW1==1))
	{
		if(state_g == load_buf_1)
		{
			if(sector_offset>=64)
			{
				state_g=find_cluster_1;
			}
		}
		if(state_g == load_buf_2)
		{
			if(sector_offset>=64)
			{
				state_g=find_cluster_2;
			}
		}
	}

	switch(state_g)
	{
		case data_send_1:
		{
		YELLOWLED=OFF;
   		AMBERLED=ON;
		GREENLED=OFF;
		REDLED=OFF;
   		do 
		{
		    if(DATA_REQ == 0)
			{
			BIT_EN=1;
			SPI_Transfer(buf1[index1], &temp8);
			BIT_EN=0;
			index1++;
				if(index1>511)
				{
					if(index2>511)
					{
						if(play_status==3)
						{
						play_status=0;
						}
						else
						{
						state_g=load_buf_2;
						}
					}
					else
					{
					state_g=data_send_2;
					}
				}
				if((DATA_REQ==0)&&(state_g==data_send_1))
				{
					if(index2>511)
					{
					state_g=load_buf_1;
					}
					else
					{
					state_g=data_idle_1;
					}
				}
			}
		  }while(index1<512);
		break;
		}
	case load_buf_2:
	{
		YELLOWLED=ON;
   		AMBERLED=OFF;
		GREENLED=OFF;
		REDLED=OFF;
		Read_Sector(sector+sector_offset,512,buf2);
		sector_offset++;
		state_g=data_idle_1;
	break;
	}
	case data_idle_1:
	{
				YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=ON;
		REDLED=OFF;
			if(DATA_REQ==0)
			{
				state_g=data_send_1;
			}
		break;
	}
	case find_cluster_2:
	{
										YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=OFF;
		REDLED=ON;
		Start_Cluster=Find_Next_Clus(Start_Cluster,buf2);
		if(Start_Cluster==0xFFFFFFFF)
		{
			play_status=3;
			state_g=data_idle_1;
		}
		else
		{
		sector=First_Sector(Start_Cluster);
		sector_offset=0;
		state_g=data_idle_1;
		}
		break;
	}
	case data_send_2:
		{
		YELLOWLED=OFF;
   		AMBERLED=ON;
		GREENLED=OFF;
		REDLED=OFF;
		if((DATA_REQ==0))
		{
		BIT_EN=1;
		SPI_Transfer(buf2[index2], &temp8);
		BIT_EN=0;
		index2++;
			if(index2>511)
			{
				if(index1>511)
				{
					if(play_status==3)
					{
					play_status=0;
					}
					else
					{
					state_g=load_buf_1;
					}
				}
				else
				{
				state_g=data_send_1;
				}
			}
			if((DATA_REQ==0)&&(state_g==data_send_2))
			{
				if(index1>511)
				{
				state_g=load_buf_1;
				}
				else
				{
				state_g=data_idle_2;
				}
			}
		}
		break;
		}
	case load_buf_1:
	{
				YELLOWLED=ON;
   		AMBERLED=OFF;
		GREENLED=OFF;
		REDLED=OFF;
		Read_Sector(sector+sector_offset,512,buf2);
		sector_offset++;
		state_g=data_idle_2;
	break;
	}
	case data_idle_2:
	{
						YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=ON;
		REDLED=OFF;
		if(DATA_REQ==0)
		{
			state_g=data_send_2;
		}
		
	break;
	}
	case find_cluster_1:
	{
								YELLOWLED=OFF;
   		AMBERLED=OFF;
		GREENLED=OFF;
		REDLED=ON;
		Start_Cluster=Find_Next_Clus(Start_Cluster,buf1);
		if(Start_Cluster==0xFFFFFFFF)
		{
			play_status=3;
			state_g=data_idle_2;
		}
		else
		{
		sector=First_Sector(Start_Cluster);
		sector_offset=0;
		state_g=data_idle_2;
		}
		break;
	}
	default:
	{
		state_g=data_idle_2;
	}
	}
}


*/
//////////////////////
 /*  do
  {      
     do
     {
        if(DATA_REQ==0)
        {
           GREENLED=ON;
           BIT_EN=1;
           SPI_Transfer(buf1[index1], &temp8);
	       GREENLED=OFF;
	       index1++;
           if(index1>511)
           {
              if(index2>511)
              {
                  BIT_EN=0;              
                  AMBERLED=ON;
				  index2=0;
				  
				  Read_Sector(sector+sector_offset, 512, buf2);
				  sector_offset++;
                  AMBERLED=OFF;
              }
              buffer1=0;
              buffer2=1;

          }
       }
       else
       {
          if(index2>511)
          {
              BIT_EN=0;
              AMBERLED=ON;
			  index2=0;
			  
			  Read_Sector(sector+sector_offset, 512, buf2);
			  sector_offset++;
              AMBERLED=OFF;
          }
          else
          {
              if(index1>511)
              {
                  buffer1=0;
                  buffer2=1;
              }
          }
      }
   }while(buffer1==1);
   do
   {
      if(DATA_REQ==0)
      {
          REDLED=ON;
          BIT_EN=1;
          SPI_Transfer(buf2[index2], &temp8);
          REDLED=OFF;
          index2++;
          if(index2>511)
          {
              if(index1>511)
              {
                  BIT_EN=0; 
                  YELLOWLED=ON;
				  index1=0;
				  
				  Read_Sector(sector+sector_offset, 512, buf1);
				  sector_offset++;
                  YELLOWLED=OFF;
              }
              buffer2=0;
              buffer1=1;
         
           }
        }
        else
        {
           if(index1>511)
           {
              BIT_EN=0; 
              YELLOWLED=ON;
			  index1=0;
			  
			  Read_Sector(sector+sector_offset, 512, buf1);
			  sector_offset++;
              YELLOWLED=OFF;
           }
           else
           {
               if(index2>511)
               {
                  buffer2=0;
                  buffer1=1;
               }
           }
        }
      }while(buffer2==1);
  }while(sector_offset<512);
//P3_2=OFF;
} 
*/
/*

void Play_Song2(uint32_t Start_Cluster)
{
   uint16_t index1;
   uint8_t buff_emp, * p_out;
   uint32_t sector, sector_offset;
printf("Starting Cluster = %lu\n\r",Start_Cluster);
sector=First_Sector(Start_Cluster);
printf("Starting Sector = %lu\n\r",sector);
//P3_2=ON;
sector_offset=0;
YELLOWLED=ON;
nCS0=0;
SEND_COMMAND(17,sector+sector_offset);
read_block(512,buf1);
index1=0;
sector_offset++;
nCS0=1;
YELLOWLED=OFF;
//AMBERLED=ON;
//nCS0=0;
//SEND_COMMAND(17,sector+sector_offset);
//read_block(buf2,512);
//sector_offset++;
//nCS0=1;
//AMBERLED=OFF;


   p_out=buf1;
   buff_emp=0;
   do
   {
      if(DATA_REQ==0)
      {
         GREENLED=ON;
         BIT_EN=1;
         while((SPSTA&0x80)!=0x80);
         SPDAT=*(buf1+index1);
         index1++;
         if(index1==512)
         {
            buff_emp|=1;           
         }
		 if(index1==1024)
		 {
		    index1=0;
			buff_emp|=2;
	     }
         if(index1==768)
         {
           BIT_EN=0;
           GREENLED=OFF;
           if((buff_emp & 0x01)==0x01)
           {
              YELLOWLED=ON;
              nCS0=0;
              SEND_COMMAND(17,sector+sector_offset);
              read_block(512,buf1);
              nCS0=1;             
              YELLOWLED=OFF;
              buff_emp &= 0xFE;
              sector_offset++;            
           }
         }
         if(index1==256)
         {
           BIT_EN=0;
           GREENLED=OFF;
           if((buff_emp & 0x02)==0x02)
           {
              AMBERLED=ON;
              nCS0=0;
              SEND_COMMAND(17,sector+sector_offset);
              read_block(512,buf2);
              nCS0=1;             
              AMBERLED=OFF;
              buff_emp &= 0xFD;
              sector_offset++;            
           }
         }                        
       }
       else
       {
          GREENLED=OFF;
          BIT_EN=0;
          if((buff_emp & 0x01)==0x01)
          {
             YELLOWLED=ON;
             nCS0=0;
             SEND_COMMAND(17,sector+sector_offset);
             read_block(512,buf1);
             nCS0=1;             
             YELLOWLED=OFF;
             buff_emp &= 0xFE;
             sector_offset++;
 //            print_hex(1);
 //            print_hex(i);
            
          }
          else if((buff_emp & 0x02)==0x02)
          {
             AMBERLED=ON;
             nCS0=0;
             SEND_COMMAND(17,sector+sector_offset);
             read_block(512,buf2);
             nCS0=1;             
             AMBERLED=OFF;
             buff_emp &= 0xFD;
             sector_offset++;
          }
       }
   }while(sector_offset<128);   
   GREENLED=1;
//   P3_2=OFF;
  }  

 */
 
