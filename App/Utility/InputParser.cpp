#include "InputParser.h"
#include "GlobalVariables.h"
#include "inttypes.h"
#include "Constants.h"

wamr_config_t WAMR_INPUT_CONFIG = {
    .log_verbose_lvl = DEFAULT_LOG_LEVEL,
    .max_thread_num = DEFAULT_MAX_THREAD_NUM,
    .stack_size = DEFAULT_STACK_SIZE,
    .heap_size  = DEFAULT_HEAP_SIZE
};

static const char *TOOL_USAGE = "Usage: iwasm [-options] wasm_file [args...]\n"
                                "options:\n"
                                "  -f|--function name               Specify a function name of the module to run rather than main\n"
                                "  -l --log-level    [#]            Set log verbose level (0 to 5, default is 2) larger level with more log\n"
                                "  -s --stack-size   [#]            Set maximum stack size in bytes, default is 16 KB\n"
                                "  -p --heap-size    [#]            Set maximum heap size in bytes, default is 16 KB\n"
                                "  -e --env  [key=value]            Pass wasi environment variables with \"key=value\"\n"
                                "                                   to the program, for example:\n"
                                "                                    --env \"key1=value1\" --env \"key2=value2\"\n"
                                "  -d --dir [path_to_directory]              Grant wasi access to the given host directories\n"
                                "                                    to the program, for example:\n"
                                "                                     --dir <dir1> --dir <dir2>\n"
                                "  --max-threads     [#]            Set maximum thread number per cluster, default is 4\n";


void say_usage()
{
    fprintf(stderr, "%s\n", TOOL_USAGE);
}

void say_help(void)
{
    fprintf(stderr, "use '-h' for help.\n");
}

#define FLAG_IS_SET 1
#define FLAG_NOT_SET -1

void parseInput(int argc, char **argv)
{
    int l_flag = FLAG_NOT_SET, s_flag = FLAG_NOT_SET, p_flag = FLAG_NOT_SET, t_flag = FLAG_NOT_SET, e_flag = FLAG_NOT_SET, d_flag = FLAG_NOT_SET, h_flag = FLAG_NOT_SET;

    int c;
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
                {"log-level"        ,  required_argument, 0, 'l'},
                {"stack-size"       ,  required_argument, 0, 's'},
                {"heap-size"        ,  required_argument, 0, 'p'},
                {"thread-num"       ,  required_argument, 0, 't'},
                {"env"              ,  required_argument, 0, 'e'},
                {"dir"              ,  required_argument, 0, 'd'},
                {"help"             ,  no_argument      , 0, 'h'},
                {      0            ,         0         , 0,  0 }
        };

        const char * allowedOptions = "l:s:p:t:e:d:h";

        c = getopt_long(argc, argv, allowedOptions, long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)  break;
        switch (c) {
            case 'l':
                if (l_flag  == FLAG_NOT_SET) {
                    int log_level = atoi(optarg);
                    if (log_level < 0 || log_level > 4) {
                        fprintf(stderr, "Error: Invalid log level value!.\n");
                        say_help();
                        exit(EXIT_FAILURE);
                    }
                    WAMR_INPUT_CONFIG.log_verbose_lvl = log_level;
                    l_flag = FLAG_IS_SET;
                }
                else {
                    fprintf(stderr, "Error: log level value is set more than once.\n");
                    say_help();
                    exit(EXIT_FAILURE);
                }
                break;

            case 's':
                if (s_flag  == FLAG_NOT_SET) {
                    int stack_size = atoi(optarg);
                    if (stack_size < 0) {
                        fprintf(stderr, "Error: Stack size value should be positive!\n");
                        say_help();
                        exit(EXIT_FAILURE);
                    }
                    WAMR_INPUT_CONFIG.stack_size = stack_size;
                    s_flag = FLAG_IS_SET;
                }
                else {
                    fprintf(stderr, "Error: Stack size value is set more than once.\n");
                    say_help();
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                if (p_flag == FLAG_NOT_SET) {
                    int heap_size = atoi(optarg);
                    if (heap_size < 0) {
                        fprintf(stderr, "Error: Heap size value should be positive!\n");
                        say_help();
                        exit(EXIT_FAILURE);
                    }
                    WAMR_INPUT_CONFIG.heap_size = heap_size;
                    p_flag = FLAG_IS_SET;
                }
                else {
                    fprintf(stderr, "Error: Heap size value is set more than once.\n");
                    say_help();
                    exit(EXIT_FAILURE);
                }
                break;

            case 't':
                if (t_flag  == FLAG_NOT_SET) {
                    int thread_num = atoi(optarg);
                    if (thread_num < 0) {
                        fprintf(stderr, "Error: Invalid number of threads!.\n");
                        say_help();
                        exit(EXIT_FAILURE);
                    }
                    WAMR_INPUT_CONFIG.max_thread_num = thread_num;
                    t_flag = FLAG_IS_SET;
                }
                else {
                    fprintf(stderr, "Error: number of threads is set more than once.\n");
                    say_help();
                    exit(EXIT_FAILURE);
                }
                break;

                //TODO env and dir should be parsed and add accordingly
            case 'h':
                say_usage();
                exit(EXIT_SUCCESS);

            default:
                break;
        }
    }
}
