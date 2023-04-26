#include "root.h"

/**
 * _copyenv - creates a copy of the environment
 *
 * Return: If an error occurs - NULL
 *         Otherwise - A double pointer to the new copy
 */
char **_copyenv(void)
{
	int i, env_size;
	char **new_env = NULL;

	/* count the size of the environment */
	for (env_size = 0; environ[env_size] != NULL; env_size++)
		continue;

	/* allocate memory for the new environment */
	new_env = malloc(sizeof(char *) * (env_size + 1));
	if (new_env == NULL)
		return (NULL);

	/* copy each environment variable into the new environment */
	for (i = 0; i < env_size; i++)
	{
		new_env[i] = _strdup(environ[i]);
		if (new_env[i] == NULL)
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
	}

	/* terminate the new environment with a NULL pointer */
	new_env[i] = NULL;

	return (new_env);
}

/**
 * free_env - function to free the environment
 */
void free_env(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * _getenv - displays environmental variable
 * @var: the environmental variable to obtain
 *
 * Return: If the environmental variable does not exist - NULL.
 *         Otherwise - a pointer to the environmental variable.
 */
char *_getenv(const char *var)
{
	int index, len;

	len = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (environ[index]);
	}

	return (NULL);
}
