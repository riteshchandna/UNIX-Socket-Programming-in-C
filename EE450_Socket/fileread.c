#include<stdio.h>

    int main()
    {
            FILE *fp;
            char buf[1000];

            fp = fopen("server3.txt","r");
            if (!fp)
                return 1;

            while (fgets(buf,1000, fp)!=NULL)
                printf("%s",buf);

        fclose(fp);
            return 0;
    }
