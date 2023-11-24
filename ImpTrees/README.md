# CS-240 Project 2022 - Public Subscribe System

## Introduction

This project implements a Public Subscribe System as part of the CS-240 course project using trees. The system is designed to handle events such as information insertion, subscriber registration, consumption, subscriber deletion, and printing of system data structures. 

## Implementation Details
The main logic of the system is implemented in the main.c file, and function definitions are provided in pss.c. The implementation includes functions for initializing, freeing resources, inserting information, subscriber registration, consumption, subscriber deletion, and printing system data.

## Event Formats
```bash
Insert_Info: I <itm> <iId> <gId1> <gId2> ... <gIdk> -1
Subscriber_Registration: S <sΤΜ> <sId> <gId1> <gId2> ... <gIdm> -1
Prune: R <tm>
Consume: C <sId>
Delete_Subscriber: D <sId>
Print: P
Lines starting with '#' are treated as comments.
