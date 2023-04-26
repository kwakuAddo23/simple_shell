#include "root.h"

/**
 * get_builtin - function that matches a command with a corresponding
 *	shell by builtin function
 * @command: the command to match
 *
 * Return: the pointer to the corresponding builtin
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", builtin_exit },
		{ "env", builtin_env },
		{ "setenv", builtin_setenv },
		{ "unsetenv", builtin_unsetenv },
		{ "cd", builtin_cd },
		{ "alias", builtin_alias },
		{ "help", builtin_help },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (_strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * builtin_exit - Exits the shell
 * @args: An array of arguments containing the exit value.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If there are no arguments - -3.
 *         If the given exit value is invalid - 2.
 *         Otherwise, exits with the given status value.
 */
int builtin_exit(char **args, char **front)
{
	int i = 0, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			len_of_int++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= len_of_int && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (create_error(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (create_error(--args, 2));
	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(num);
}

/**
 * builtin_help - function gives information about shellby builtin commands.
 * @args: An array of arguments.
 *
 * Return: 0 on success, -1 on error.
 */
int builtin_help(char **args)
{
	char *builtin_cmds[] = {
		"alias",
		"cd",
		"env",
		"exit",
		"help",
		"setenv",
		"unsetenv",
		NULL
	};
	char **builtin_cmd_ptr = builtin_cmds;

	if (args == NULL)
		return (-1);

	if (*args == NULL)
		help_all();
	else
	{
		while (*builtin_cmd_ptr != NULL)
		{
			if (_strcmp(*builtin_cmd_ptr, *args) == 0)
			{
				write(STDOUT_FILENO, "Usage: ", 7);
				help_builtin(*args);
				return (0);
			}
			builtin_cmd_ptr++;
		}
		write(STDERR_FILENO, "No help topics match ", 22);
		write(STDERR_FILENO, *args, _strlen(*args));
		write(STDERR_FILENO, ". Try 'help' for more information.\n", 36);
		return (-1);
	}
	return (0);
}

/**
 * builtin_cd - function to change the current directory of
 *              the shellby process.
 * @args: An array of arguments.
 * @front: A double pointer to the node
 *
 * Return: 0 on success, -1 on failure.
 */
int builtin_cd(char **args, __silent char **front)
{
	char **dir_info = NULL;
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;
	int ret = 0;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (!args[0])
		ret = chdir(_getenv("HOME") ? (_getenv("HOME") + 5) : ".");
	else if (_strcmp(args[0], "-") == 0)
	{
		if (_getenv("OLDPWD"))
			ret = chdir(_getenv("OLDPWD") + 7);
		else
			ret = create_error(args, 2);
	}
	else if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode) && ((dir.st_mode & S_IXUSR) != 0))
		ret = chdir(args[0]);
	else
		ret = create_error(args, 2);

	if (ret != -1)
	{
		pwd = getcwd(pwd, 0);
		if (pwd)
		{
			dir_info = malloc(sizeof(char *) * 2);
			if (dir_info)
			{
				dir_info[0] = "OLDPWD";
				dir_info[1] = oldpwd;
				builtin_setenv(dir_info, dir_info);

				dir_info[0] = "PWD";
				dir_info[1] = pwd;
				builtin_setenv(dir_info, dir_info);

				free(dir_info);
			}
			else
			{
				ret = -1;
			}
			free(pwd);
		}
		else
		{
			ret = -1;
		}
	}

	free(oldpwd);
	return (ret);
}
