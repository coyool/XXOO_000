/**
* shellcmd.h -- √¸¡Ó––µ˜ ‘√¸¡Ó
*/

#ifndef _DEBUG_SHELLCMD_H
#define _DEBUG_SHELLCMD_H

typedef int (*shell_func)(int argc, char *argv[]);

struct shell_cmd {
	const char *name;
	shell_func pfunc;
	const char *info;
};

#define DECLARE_SHELL_CMD(cmdname, func, helpinfo) \
	static struct shell_cmd _shellcmd_##func \
	= {cmdname, func, helpinfo}
#define INIT_SHELL_CMD(func)	 \
	  ShellCmdRig(&_shellcmd_##func)

void ShellCmdRig(const struct shell_cmd *new_cmd);
void ShellCmdInit(void);

int ShellParseArg(const char *line, char *argv[], int argmax);
shell_func FindShellFunc(const char *command);
int TtyShellStart(void);
void shell_cms_init(void);

#endif /*_DEBUG_SHELLCMD_H*/

