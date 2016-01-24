##aa <- pollutantmean("specdata", "nitrate", 70:72)
pollutantmean <-function(direc, pollutant, id)
{
dummy_total <- 0
dummy_count <- 0
poll<- pollutant
      
    for(i in id){
        
           if (i < 10)
               file <- paste("00",i,".csv",sep="")
           
           
           if ((i > 9)&&(i<100)) 
               file <- paste("0",i,".csv",sep="")
           
           if( i>99 )
           {
                 file <- paste(i,".csv",sep="")
                    
           }
           
            mydata <- read.csv(file)
          
            my_data_pollutant <- mydata[,pollutant]
            my_data_pollutant_NA <- my_data_pollutant[!is.na(my_data_pollutant)]
            
            dummy_total<-dummy_total+ mean(my_data_pollutant_NA)
            dummy_count <- dummy_count + nrows(my_data_pollutant_NA)      
      }
    
    return(dummy_total / dummy_count)
    
}