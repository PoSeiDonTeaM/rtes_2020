#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>

using namespace std; 

int main(int argc, char *argv[])
{
    struct timeval now;
    int rc;

    rc=gettimeofday(&now, NULL);
    if(rc==0) {
        
        cout << "time = " << now.tv_sec << endl;
    }
    else {
        printf("gettimeofday() failed, errno = %d\n",
                errno);
        return -1;
    }

    return 0;
}
