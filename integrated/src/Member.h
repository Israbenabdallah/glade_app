/* Member.h copied from isra project */
#ifndef MEMBER_H
#define MEMBER_H

typedef struct {
    char id_member[20];
    char first_name[30];
    char last_name[30];
    int age;
    int gender;         // 0 = Female, 1 = Male
    char phone[15];
    char email[50];
    char subscription_type[30];
    int private_coach;
    int request_show_stats;
    int join_course;
} Member;
// Member functions
int ajouter_member(char *filename, Member m);
int modifier_member(char *filename, char *id, Member new_member);
int supprimer_member(char *filename, char *id);
Member chercher_member(char *filename, char *id);
Member chercher_member_by_firstname(char *filename, char *first_name);


#endif
