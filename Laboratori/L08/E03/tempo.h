#ifndef TIME_DEFINED
#define TIME_DEFINED

typedef struct s_data {
    int year;
    int month;
    int day;
} t_data;

typedef struct t_orario {
    int hours;
    int minutes;
} t_orario;

t_data DATEsetNull();
int DATEcmp(t_data a, t_data b);

t_orario TIMEsetNull();
int TIMEcmp(t_orario a, t_orario b);

#endif