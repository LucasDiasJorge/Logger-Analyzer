#ifndef DECISION_H
#define DECISION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decision_tree.h"

#define ACTION_RESTART 1
#define ACTION_SHUTDOWN 2
#define ACTION_ALERT 3
#define ACTION_LOG 4
#define ACTION_REPAIR 5
#define ACTION_SLOWDOWN 6

int make_decision(const char *log_message, DecisionNode *root);

#endif