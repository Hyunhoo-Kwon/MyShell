#include <stdio.h>
#include <stdlib.h>//malloc ..
#include <signal.h>
#include <string.h>
#include <unistd.h>//chdir ..
#include <dirent.h>//readdir ..

#define BSIZE 1024//buffer size

//commands struct
struct COMMAND{
	char* name;
	int (*func)(const char* option);
};

//linked list data struct
struct data {
	char line[BSIZE];
	struct data *prev;
	struct data *next;
};
typedef struct data Node;

//declare functions
void initList();
void insertHistory(const char *line);
void displayHistory();
int run(const char *cmd, const char *opt);
int myls(const char *opt);
int mycd(const char *cdpath);
//my commands
struct COMMAND my_cmds[] = 
{
	{"ls", myls},
	{"cd", mycd},
	{"history", displayHistory}
};

//set history - use linked list
Node *head;
void initList() {
	head = (Node *)malloc(sizeof(Node));
	head->prev = NULL;
	head->next = NULL;
}

void insertHistory(const char *line) {	
	Node *new;
	Node *right;
	Node *target;

	target = head;
	new = (Node *)malloc(sizeof(Node));
	right = target->next;
	new->next = right;
	new->prev = target;
	target->next = new;
	if(right)
		right->prev = new;

	strcpy(new->line, line);
}

void displayHistory(const char *opt) {
	Node *tmp;
	int i=0;
	tmp = head->next;
	while(tmp != NULL) {
		i++;
		printf("%d	%s\n",i,tmp->line);
		tmp = tmp->next;
	}
}

//run command
int run(const char *cmd, const char *opt) {
	if(strcmp(cmd, my_cmds[0].name) == 0) {
		myls(opt);
	}
	else if(strcmp(cmd, my_cmds[1].name) == 0) {
		mycd(opt);
	}
	else if(strcmp(cmd, my_cmds[2].name) == 0) {
		displayHistory(opt);
	}
	else {
		printf("%s: command not found\n",cmd);
	}
}

//list files and directories
int myls(const char *opt) {
	//get present working directory
	char path[BSIZE];
	getcwd(path, BSIZE);
	//print present directory files
	DIR* dirptr;
	struct dirent* dir;
	dirptr=opendir(path);
	while(dir=readdir(dirptr)) {
		printf("%s\t", dir->d_name);
	}
	printf("\n");
}

//change directory
int mycd(const char *cdpath) {
	//path error
	if(chdir(cdpath)) {
		perror("cd");
	}
}

int main() {
	char path[BSIZE];
	char line[BSIZE];
	char *line_tmp;
	char *commands[5];
	//signal block
	sigset_t blockset;
	sigemptyset(&blockset);
	sigaddset(&blockset, SIGINT);
	sigaddset(&blockset, SIGQUIT);
	sigprocmask(SIG_BLOCK, &blockset, NULL);
	//init history list
	initList();
	printf("Start mini Shell. Shell Exit message: exit\n\n"); 
	while(1) {
		//get present working directory
		getcwd(path, BSIZE);
		//get line
		printf("[%s]$ ", path);
		fgets(line, BSIZE, stdin);
		fflush(stdin);
		line[strlen(line)-1]='\0';
		//skip blank(only input enter)
		if(strlen(line)==0)
			continue;
		//exit program
		if(strcmp(line, "exit")==0) {
			printf("Program Exit!\n");
			break;
		}
		//add history
		insertHistory(line);
		//line copy
		line_tmp = (char *)malloc(strlen(line)+1);
		strcpy(line_tmp, line);
		//get commands ";"
		int i=0;
		int cmd_count=0;
		for(i=0; i<5; i++) {
			commands[i] = NULL;
		}
		commands[0]=strtok(line_tmp, ";");
		cmd_count++;
		while(commands[cmd_count-1] != NULL) {
			commands[cmd_count]=strtok(NULL, ";");
			if(commands[cmd_count] ==NULL || (cmd_count+1) >= 5)
				break;
			cmd_count++;
		}
		//line token " " - cmd , opt
		for(i=0; i<cmd_count; i++) {
			char *cmd;
			char *opt;
			//skip only input " "
			if(strtok(commands[i], " ") != NULL) {
				cmd=strtok(commands[i], " ");
				opt=strtok(NULL, " ");
				//run program
				run(cmd, opt);
			}
		}
		free(line_tmp);
	}
}
