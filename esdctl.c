#include "esd.h"

#include <stdio.h>
#include <string.h>
#include <getopt.h>

void exit_usage( const char *who, int errcode, const char *why, const char *what )
{
    /* what went wrong? */
    if ( why ) {
	fprintf( stderr, "%s error (%d):  %s", who, errcode, why );
	if ( what ) {
	    fprintf( stderr, " - %s", what );
	}
    }
    fprintf( stderr, "\n" );

    /* tell how it works */
    fprintf( stderr, "%s [options] [command]\n", who );
    fprintf( stderr, 
	     "\n"
	     "options:\n"
	     "-s, --server=host:port        contact esd server on host at port\n"
	     "\n"
	     "commands:\n"
	     "lock                          foreign clients may not use the server\n"
	     "unlock                        foreign clients may use the server\n"
	     "standby, off                  suspend sound output for other programs\n"
	     "resume, on                    resume sound output\n"
	     "cache sample                  cache a sample in the server\n"
	     "getid name                    retrieve a sample id from its name\n"
	     "free name                     uncache a sample in the server\n"
	     "play name                     play a cached sample once\n"
	     "loop name                     make a cached sample loop\n"
	     "stop name                     stop the looping sample at end\n"
	     "serverinfo                    get server info from server\n"
	     "allinfo                       get player and sample info from server\n"
	     "\n" );

    /* terminate with given error code */
    exit( errcode );    
}

int main(int argc, char **argv)
{
    int esd = -1, arg = 0, option_index = 0, ok;
    char *server = NULL;
    esd_server_info_t *server_info = NULL;
    esd_info_t *all_info = NULL;
  
    struct option opts[] = {
	{ "server", required_argument, NULL, 's' },
	{ "file", required_argument, NULL, 'f' },
	{ "help", no_argument, NULL, 'h' },
	{ "version", no_argument, NULL, 'v' },
	{ 0, 0, 0, 0 }
    };

    /* quick sanity check, check number of args */
    if ( argc == 1 ) 
	exit_usage( argv[0], 1, "command line", "not enough arguments" );

    /* check the command line areguments */
    do
    {
	arg = getopt_long(argc, argv, "s:hv", opts, &option_index);

	switch (arg)
	{
	case 's':
	    server = strdup( optarg );
	    break;

	case 'h':
	    exit_usage( argv[ 0 ], 0, NULL, NULL );
	    break;
	  
	case 'v':
	    printf( "%s %s\n", argv[ 0 ], "VERSION" );
	    exit( 0 );

	case '?':
	    /* `getopt_long' already printed an error message. */
	    fprintf(stderr,"Try `%s --help' for more information.\n", argv[ 0 ] );
	    exit( 2 );

	case EOF:
	    break;

	default:
	    arg = -1;
	    break;
	}
    } while ( arg != EOF );

    /* make sure we ate all the options */
    if ( optind == argc )
	exit_usage( argv[ 0 ], 3, "command line", "no command given");

    /* contact the server */
    esd = esd_open_sound( server );
    if ( esd <= 0 ) return 1;

    /* control the daemon */
    while ( optind < argc )
    {
	if ( !strcmp( "lock", argv[ optind ] ) ) {
	    esd_lock( esd );
	} 
	else if ( !strcmp( "unlock", argv[ optind ] ) ) {
	    esd_unlock( esd );
	} 
	else if ( !strcmp( "off", argv[ optind ] ) ) {
	    esd_standby( esd );
	}
	else if ( !strcmp( "standby", argv[ optind ] ) ) {
	    esd_standby( esd );
	}
	else if ( !strcmp( "on", argv[ optind ] ) ) {
	    esd_resume( esd );
	}
	else if ( !strcmp( "resume", argv[ optind ] ) ) {
	    esd_resume( esd );
	}
	else if ( !strcmp( "getid", argv[ optind ] ) ) {
	    ok = esd_sample_getid( esd, argv[ ++optind ] );
	    printf( "%d\n", ok );
	}
	else if ( !strcmp( "serverinfo", argv[ optind ] ) ) {
	    server_info = esd_get_server_info( esd );

	    if ( !server_info ) {
		fprintf( stderr, "serverinfo failed\n" );
	    } else {
		esd_print_server_info( server_info );
		esd_free_server_info( server_info );
	    }
	}
	else if ( !strcmp( "allinfo", argv[ optind ] ) ) {
	    all_info = esd_get_all_info( esd );

	    if ( !all_info ) {
		fprintf( stderr, "allinfo failed\n" );
	    } else {
		esd_print_all_info( all_info );
		esd_free_esd_info( all_info );
	    }
	}
	else {
	    exit_usage( argv[ 0 ], 3, "unknown command", argv[ optind ] );
	}

	optind++;
    }

    close( esd );
    return 0;
}
