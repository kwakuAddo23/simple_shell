#include "root.h"

/**
 * builtin_alias - an in-built command to print either all aliases, specific
 * aliases, or sets an alias.
 * @args: An array of arguments.
 * @front: pointer to the node
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int builtin_alias(char **args, __silent char **front)
{
	if (!args[0])
	{
		/* Print all aliases */
		for (alias_t *alias = aliases; alias; alias = alias->next)
		{
			printf("%s=%s\n", alias->name, alias->value)
		}
	}
	else
	{
		/* Process each argument */
		for (int i = 0; args[i]; i++)
		{
			/* Check if argument has an equals sign */
			char *equals = strchr(args[i], '=');

			if (equals)
			{
				/* Set an alias */
				*equals = '\0';
				set_alias(args[i], equals + 1);
			}
			else
			{
				/* Print a specific alias */
				alias_t *alias = find_alias(args[i]);

				if (alias)
				{
					printf("%s=%s\n", alias->name, alias->value);
				}
				else
				{
					fprintf(stderr, "alias: %s: not found\n", args[i]);
					return (-1);
				}
			}
		}
	}
	return (0);
}

/**
 * set_alias - sets an already present alias 'name' with a new value,
 * 'value' or creates a new alias with 'name' and 'value'.
 * @var_name: alias name
 * @value: Value of the alias. First character is a '='.
 *
 * Return: void.
 */
void set_alias(char *var_name, char *value)
{
	alias_t *temp = aliases;
	int len = 0, i = 0, j = 0;
	char *new_value = NULL;

	*value = '\0';
	value++;
	len = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (new_value == NULL)
		return;

	for (i = 0, j = 0; value[i]; i++)
	{
		if (value[i] != '\'' && value[i] != '"')
			new_value[j++] = value[i];
	}
	new_value[j] = '\0';

	while (temp != NULL)
	{
		if (_strcmp(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			return;
		}
		temp = temp->next;
	}

	add_alias_end(&aliases, var_name, new_value);
}

/**
 * print_alias - prints the alias in the format name='value'.
 * @alias: points to an alias.
 */
void print_alias(alias_t *alias)
{
	char *alias_string;
	int len;

	len = _strlen(alias->name) + _strlen(alias->value) + 4;
	alias_string = malloc(sizeof(char) * len);
	if (!alias_string)
		return;
	_snprintf(alias_string, len, "%s='%s'\n", alias->name, alias->value);
	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}

/**
 * replace_aliases - Replaces any matching aliases with their value in args.
 * @args: Array of strings representing arguments.
 *
 * Return: New array of strings with aliases replaced with their value.
 */
char **replace_aliases(char **args)
{
	alias_t *alias;
	int i, j, len;
	char *new_val;

	if (_strcmp(args[0], "alias") == 0)
		return (args);

	for (i = 0; args[i] != NULL; i++)
	{
		alias = aliases;
		while (alias)
		{
			if (_strcmp(alias->name, args[i]) == 0)
			{
				len = _strlen(alias->value);
				new_val = malloc(sizeof(char) * (len + 1));
				if (!new_val)
					return (NULL);
				for (j = 0; j < len; j++)
					new_val[j] = alias->value[j];
				new_val[j] = '\0';
				free(args[i]);
				args[i] = new_val;
				break;
			}
			alias = alias->next;
		}
	}

	return (args);
}
