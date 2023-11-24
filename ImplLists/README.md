# CS-240 Project 2022 - Public Subscribe System

## Introduction

This project implements a Public Subscribe System as part of the CS-240 course project in 2 Phases, one with lists and the other with trees. The system is designed to handle events such as information insertion, subscriber registration, consumption, subscriber deletion, and printing of system data structures.

## Compilation 

To compile the project, use the following command:

## Usage

Execute the compiled program with the input file as a command-line argument:

# CS-240 Project 2022 - Public Subscribe System

## Introduction

This project implements a Public Subscribe System as part of the CS-240 course project in 2 Phases, one with lists and the other with trees. The system is designed to handle events such as information insertion, subscriber registration, consumption, subscriber deletion, and printing of system data structures.

## Compilation 

To compile the project, use the following command:

## Data Structures
The project uses various data structures such as groups, subscribers, subscriptions, and information entries. These are defined in pss.h.

## Implementation Details

The main logic of the system is implemented in the main.c file, and function definitions are provided in pss.c. The implementation includes functions for initializing, freeing resources, inserting information, subscriber registration, consumption, subscriber deletion, and printing system data.


## Data Structures
The project uses various data structures such as groups, subscribers, subscriptions, and information entries. These are defined in pss.h.

## Implementation Details

The main logic of the system is implemented in the main.c file, and function definitions are provided in pss.c. The implementation includes functions for initializing, freeing resources, inserting information, subscriber registration, consumption, subscriber deletion, and printing system data.

## Events Format

Execute the compiled program with the input file as a command-line argument:

```bash
Insert_Info: I <itm> <iId> <gId1> <gId2> ... <gIdk> -1
Subscriber_Registration: S <sΤΜ> <sId> <gId1> <gId2> ... <gIdm> -1
Consume: C <sId>
Delete_Subscriber: D <sId>
Print: P
Lines starting with '#' are treated as comments.
