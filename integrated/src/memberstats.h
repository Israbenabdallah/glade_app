#ifndef MEMBERSTATS_H
#define MEMBERSTATS_H

typedef struct {
    int num_classes_attended;
    char favorite_class[50];
    char coach_name[50];
    int performance;          // 1 = bad, 2 = good, 3 = excellent
    int consistency_score;    // 0-10
} MemberStats;

// Function declarations
MemberStats load_member_stats(const char *filename, const char *id);
void save_member_stats(const char *filename, const char *id, MemberStats ms);

#endif
