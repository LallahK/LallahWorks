#include <stdio.h>
#include <stdlib.h> 

int daysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int ordinalMonths[] = {13, 14, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int validDate(char *date, short int *dateValues);
int dayOfWeek(short int *dateValues);

int main(int argc, char *argv[]) {
    char date[20];
    short int dateValues[3];

    printf("%s", "Enter date: ");
    scanf("%s", date);
    
    if (!validDate(date, dateValues)){
        printf("Invalid Date");
        return 0;
    }
    
    printf("%d", dayOfWeek(dateValues));
        
}

int validDate(char *date, short int *dateValues) {
	char *sp = date;
    char cyear[] = {date[0], date[1], date[2], date[3]};
    char cmonth[] = {date[5], date[6]};
    char cday[] = {date[8], date[9]};
    
    while (*sp != '\0') {
        switch (sp - date) {
            case 4 : if (*sp != '/') return 0;
                     break;              
            case 7 : if (*sp != '/') return 0;
					 break;
            default : if (!(*sp > 47 && *sp < 58)) return 0;
        }
        sp++;
    }
    if (sp - date != 10) return 0;  

    dateValues[0] = atoi(cyear);
    dateValues[1] = atoi(cmonth);
    dateValues[2] = atoi(cday);
    if (!(dateValues[0] % 4 == 0 || (dateValues[0] % 100 == 0 && !(dateValues[0] % 400 == 0)))) {
        if (dateValues[2] > 29 && dateValues[1] == 2) return 0;
    }
    if (dateValues[2] > daysInMonths[dateValues[1] + 1]) return 0;
    if (dateValues[1] > 12) return 0;
    
    return 1;
}

int dayOfWeek(short int *d) {
    return (d[2] + (int)(26 * (ordinalMonths[d[1] - 1] + 1) / 10.0) + d[0] + (int)(d[0] / 4.0) + 6 * (int)(d[0] / 100.0) + (int)(d[0] / 400.0)) % 7;
}


