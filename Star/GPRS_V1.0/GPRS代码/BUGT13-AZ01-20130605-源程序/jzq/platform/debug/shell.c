/**
* shell.c -- 命令行调试
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-26
* 最后修改时间: 2009-5-26
*/

#include <stdio.h>
#include <string.h>

#include "plat_include/shellcmd.h "
#include "plat_include/debug.h"

#define INODE_NUM		5
#define HANSHNODE_NUM	26
#define INDEX_SIZE		(HANSHNODE_NUM*INODE_NUM+INODE_NUM)
static const struct shell_cmd *cmd_index[INDEX_SIZE] = {NULL};


#define ERROR_PROMPT	"\033[1;31m!error! \033[0m"


void ShellCmdRig(const struct shell_cmd *new_cmd)
{
	int i;	
	if(new_cmd->name[0] < 'a' || new_cmd->name[0] > 'z') 
	{
		PrintLog(LOGTYPE_DEBUG,ERROR_PROMPT "command %s invalid\n", new_cmd->name);
		return;
	}

	i = (int)(new_cmd->name[0] - 'a')&0xff;
	i *= INODE_NUM;
	for(; i<INDEX_SIZE; i++) 
	{
		if(NULL == cmd_index[i])
		{
			cmd_index[i] = new_cmd;
			break;
		}
	}
	if(i >= INDEX_SIZE)
	{ 
		PrintLog(LOGTYPE_DEBUG,ERROR_PROMPT "can not index command %s\n", new_cmd->name);
	}
}

shell_func FindShellFunc(const char *command)
{
	int i;

	if(command[0] < 'a' || command[0] > 'z') return NULL;

	i = (int)(command[0] - 'a')&0xff;
	i *= INODE_NUM;
	for(; i<INDEX_SIZE; i++) {
		if(NULL == cmd_index[i]) return NULL;

		if(0 == strcmp(cmd_index[i]->name, command)) {
			return cmd_index[i]->pfunc;
		}
	}

	return NULL;
}

int ShellParseArg(const char *line, char *argv[], int argmax)
{
	int rtn = 0;
	char *p = argv[0];
	int arglen;

	*p = 0;
	arglen = 0;
	for(; 0!=*line; line++) {
		if(' ' == *line) {
			if(arglen) {
			   *p = 0;
			   rtn++;
			   if(rtn >= argmax) return rtn;
			   p = argv[rtn];
			   *p = 0;
			   arglen = 0;
			}
		}
		else if('\r' == *line || '\n' == *line) break;
		else {
			*p++ = *line;
			arglen++;
			if(arglen >= 128) {
			   p = argv[rtn];
			   *p = 0;
			   arglen = 0;
			}
		}
	}

	if(arglen) {
		*p = 0;
		rtn++;
	}

	return rtn;
}

static int shell_help(int argc, char *argv[])
{
	int i;

	for(i=0; i<INDEX_SIZE; i++) {
		if(NULL != cmd_index[i]) PrintLog(LOGTYPE_DEBUG,"%s: \t%s\r\n", cmd_index[i]->name, cmd_index[i]->info);
	}

	return 0;
}
DECLARE_SHELL_CMD("help",shell_help, "help");

static int shell_empty(int argc, char *argv[])
{
	return 0;
}

DECLARE_SHELL_CMD("link", shell_empty, "empty command");
void ShellCmdInit(void)
{
	INIT_SHELL_CMD(shell_help);
	INIT_SHELL_CMD(shell_empty);
}



