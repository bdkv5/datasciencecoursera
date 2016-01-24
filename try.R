add3 <- function(x,y,z)
{
    return(x + y+z);
}


above10 <- function(x){
    
    use <- x > 10
    x[use]
}

above <- function(x,n){
    
    use <- x > n
    x[use]
}

colmeanfun <- function(x){
    col <- ncol(x)
    meancol <-numeric(col)
    for(i in 1:col)
    {
        meancol[i] <- mean(x[,i])  
    }
    return(meancol);
}

f <- function(x) {
    g <- function(y) {
        
        y + z
    }
    z <- 4
    x + g(x)
}

