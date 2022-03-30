/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stropts.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <nanvix/accounts.h>
#include <nanvix/config.h>
#include <dev/tty.h>
#include <fcntl.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */



#if (MULTIUSER == 1)


static int change(const char* name, const char* password){

    int ret;          /* Return value.    */
	int file;         /* Passwords file.  */
	struct account a; /* Working account. */
	
	ret = 1;
	
	/* Open passwords file. */
	if ((file = open("/etc/passwords",O_RDWR)) == -1)
	{
		fprintf(stderr, "cannot open password file\n");
		return (0);
	}
	
	/* Search in the  passwords file. */
	while (read(file, &a, sizeof(struct account)))
	{
		account_decrypt(a.name, USERNAME_MAX, KERNEL_HASH);
	

		/* No this user. */
		if (strcmp(name, a.name))
			continue;
		
		/* Found. */
        lseek(file,(- sizeof(struct account) + USERNAME_MAX),SEEK_CUR);
		write(file,password,PASSWORD_MAX);
        account_encrypt(a.password, PASSWORD_MAX, KERNEL_HASH);
        goto found;
        
	}

	ret = 0;
	fprintf(stderr, "\nwrong username \n\n");

found:

	/* House keeping. */
	close(file);

	return (ret);
}





/*
 * Prints program version and exits.
 */
static void version(void)
{
	printf("passwd () %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright(C) 2022 CAILLES Maxime\n");
	
	exit(EXIT_SUCCESS);
}

/*
 * Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: passwd name password\n\n");
	printf("Brief: Change password.\n\n");
	printf("Options:\n");
	printf("  --help    Display this information and exit\n");
	printf("  --version Display program version and exit\n");
	
	exit(EXIT_SUCCESS);
}

/*
 * Gets program arguments.
 */
static void getargs(int argc, char *const argv[])
{
	int i;     /* Loop index.       */
	char *arg; /* Current argument. */
		
	/* Read command line arguments. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		
		/* Parse command line argument. */
		if (!strcmp(arg, "--help")) {
			usage();
		}
		else if (!strcmp(arg, "--version")) {
			version();
		}
	}
}


/**
 * @brief Attempts to change password.
 * 
 * @returns One if successful change and false otherwise.
 */
static int do_passwd(void)
{
	
    char name[USERNAME_MAX];
	char password[PASSWORD_MAX];
	
	printf("Username : ");
	fgets(name, USERNAME_MAX, stdin);
	printf("password : ");
	fgets(password, PASSWORD_MAX, stdin);
	
	return (change(name, password));
    
}

#endif


/*
 * Changes the password of a user.
 */
int main(int argc, char *const argv[])
{

    getargs(argc, argv);
	/* Failed to passwd(). */
	if(do_passwd() < 0)
	{
		fprintf(stderr, "passwd: cannot passwd()\n");
		return (EXIT_FAILURE);
	}
	
	return (EXIT_SUCCESS);
}




  


