////////////////////////////////////////////////////////////////////////////////
//
// Shell (Generic)
//
// Desc: main.h
// Main program code.
//
// 17/04/2022 (José Benavente)
// File inception.
//
///////////////////////////////////////////////////////////////////////////////

#include "main.h"

////////////////////////////////////////////////////////////////////////////////
// Macros:
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Types:
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes:
////////////////////////////////////////////////////////////////////////////////

int main(void);
void cwd(void);
void seconds(void);
void stats(void);
void cd(char *path);
void makedir(char *path);
void listdir(void);
void prompt(void);
void parse(void);
int eval(void);
int randomInRange(int min, int max);
void resetColor(void);
void randomColor(void);
void randomBackgroundColor(void);

////////////////////////////////////////////////////////////////////////////////
// Globals:
////////////////////////////////////////////////////////////////////////////////

char command[CMD_SIZE];           // command input by user
char *args[ARGS_SIZE];            // command arguments
char lastKnownDirectory[256];     // last known directory. Used in case cwd() returns an error.
bool MAKEITGOOD = false;          // are we in MAKEITGOOD mode?
bool JOKER = false;               // are we in JOKER mode?
char * welcomeMsg  = "\n"
        "              ¿\n"
        "              |\n"
        "              |      SHELL SIMULATOR\n"
        "              |           by\n"
        "              |      José Benavente\n"
        "              |            &\n"
        "     ______   |     Vicente Bascuñán\n"
        "    \\ _____\\  |\n"
        "        / / ,.:..-----▄▄▄▄\n"
        "       | | |            █████▄▄|¯`':.._ ,---.\n"
        "      .---. \\__                       .---.  \\\n"
        "     /     \\-- \\___=================-/     \\  :\n"
        "    :   o   : /                     :   o   :/\n"
        "     \\     /-´                       \\     /´\n"
        "      `---´                           `---´   BGM\n"
        "\n";

////////////////////////////////////////////////////////////////////////////////
// Procedures:
////////////////////////////////////////////////////////////////////////////////

int main() {
    int status;
    srand(time(NULL));

    printf("%s", welcomeMsg);
    printf("Type 'exit' to terminate.\n\n");
    do {
        prompt();               // We send the prompt to the user
        parse();                // We read the command, and store it in "command"
        status = eval();        // We evaluate the command, and assign the response to "status"
    } while (status != EXIT_SUCCESS);

    return 0;
}

///
/// the prompt of our shell
///

void prompt() {
    cwd();
    printf("$ ");
}

///
/// parse the commands input by the user
///

void parse() {
    // wait until the user inputs a command, then capture it
    fgets(command, CMD_SIZE, stdin);
    if ((strlen(command) > 0) && (command[strlen(command) - 1] == '\n')) {
      // if it wasn't an empty command and it had a linebreak at the end, we remove said linebreak
      command[strlen(command) - 1] = '\0';
    }

    char *p;
    int i = 0;
    p = strtok(command, " "); // split
    while (p != NULL) {
        if (i > 0) args[i - 1] = p; // we exclude the command, and only add its arguments to "args[]"

        p = strtok(NULL, " ");
        i++;
    }
}

///
/// evaluate the commands input by the user
///

int eval() {
    if (MAKEITGOOD) randomColor();
    if (JOKER) randomBackgroundColor();

    if(!strcmp("", command)) return CONTINUE;

    if (!strcmp(EXIT_CMD, command)) {
        return EXIT_SUCCESS;
    } else if (!strcmp(PWD_CMD, command)) {
        cwd();
        printf("\n");
    } else if (!strcmp(TIME_CMD, command)) {
        seconds();
        printf("\n");
    } else if (!strcmp(CHDIR_CMD, command)) {
        cd(args[0]);
    } else if (!strcmp(SYSINFO_CMD, command)) {
        stats();
    } else if (!strcmp(MAKEDIR_CMD, command)) {
        makedir(args[0]);
    } else if (!strcmp(LS_CMD, command)) {
        listdir();
    } else if (!strcmp(MAKEITGOOD_CMD, command)) {
        if (MAKEITGOOD) {
          resetColor();
        } else {
          randomColor();
        }
        MAKEITGOOD = !MAKEITGOOD;
    } else if (!strcmp(JOKER_CMD, command)) {
        if (JOKER) {
          resetColor();
        } else {
          randomBackgroundColor();
        }
        JOKER = !JOKER;
    } else {
        printf("%s: Command not found\n", command);
    }

    return CONTINUE;
}

///
/// "$ cwd"
///

void cwd() {
    char CWD[256];

    if (getcwd(CWD, sizeof(CWD)) != NULL) {
        strcpy(lastKnownDirectory, CWD);
        printf("%s", CWD);
        return;
    }

    printf("%s", lastKnownDirectory);
    switch (errno) {
        case EACCES:
            printf("Insufficient permissions\n");
            break;
        case EFAULT:
        case EINVAL:
            printf("Unable to get current directory\n");
            break;
        case ENAMETOOLONG:
            printf("Unable to get current directory: Path name is too long\n");
            break;
        case ENONET:
            printf("Unable to get current directory: The directory has been unlinked\n");
            break;
        case ENOMEM:
            printf("Unable to get current directory: Insufficient memory\n");
            break;
        case ERANGE:
            printf("Unable to get current directory: Internal memory assignment error\n");
            break;
        default:
            printf("Unable to get current directory: Unknown error\n");
            break;
    }
}

///
/// "$ seconds"
///

void seconds() {
    time_t seconds;
    seconds = time(NULL);
    if (seconds == ((time_t) -1)) {
        perror("time() error");
    } else {
        printf("Seconds elapsed since the EPOCH (January 1st, 1970): %ld", seconds);
    }
}

///
/// "$ stats"
///

void stats() {
    struct sysinfo systemInfo;
    if (sysinfo(&systemInfo) != -1) {
        const long minute = 60;
        const long hour = minute * 60;
        const long day = hour * 24;

        const long sysDay = systemInfo.uptime / day;
        const long sysHour = (systemInfo.uptime % day) / hour;
        const long sysMinutes = (systemInfo.uptime % hour) / minute;
        const long sysSeconds = systemInfo.uptime % minute;

        printf("=== System stats ===\n");
        printf("=> Uptime: %ld %s, %02ld:%02ld:%02ld\n",
               sysDay,
               sysDay == 1 ? "day" : "days",
               sysHour,
               sysMinutes,
               sysSeconds
        );

        printf("=> Total RAM: %lu MB\n", systemInfo.totalram / (1024 * 1024));
        printf("=> Free RAM: %lu MB\n", systemInfo.freeram / (1024 * 1024));
        printf("=> Active processes: %d\n", systemInfo.procs);

        return;
    }

    switch (errno) {
        case EFAULT:
            printf("Invalid 'info' address");
            break;
        default:
            printf("Unable to get system info: Unknown error\n");
            break;
    }
}

///
/// "$ cd <path>"
/// \param path where we want to move into

void cd(char *path) {
    if (chdir(path) != -1) return;

    switch (errno) {
        case EACCES:
            printf("Insufficient permissions");
            break;
        case EFAULT:
            printf("Path points outside the accessible space address\n");
            break;
        case EIO:
            printf("I/O Error\n");
            break;
        case ELOOP:
            printf("Too many symlinks when resolving the path\n");
            break;
        case ENAMETOOLONG:
            printf("Unable to get current directory: Path name is too long\n");
            break;
        case ENOENT:
        case ENOTDIR:
            printf("%s: The directory does not exist\n", path);
            break;
        case ENOMEM:
            printf("Insufficient Kernel memory\n");
            break;
        default:
            printf("Unable to create directory: Unknown error\n");
            break;
    }
}

///
/// "$ mkdir <path>"
/// \param path path or name of the directory we want to create

void makedir(char *path) {
    if (mkdir(path, S_IRWXU) != -1) return;

    switch (errno) {
        case EACCES:
            printf("Insufficient permissions\n");
            break;
        case EDQUOT:
            printf("Disc quota exceeded\n");
            break;
        case EEXIST:
            printf("Unable to create directory: File already exists\n");
            break;
        case EFAULT:
            printf("Invalid directory name\n");
            break;
        case EINVAL:
            printf("Directory path contains invalid characters\n");
            break;
        case ELOOP:
            printf("Too many symlinks when resolving the path\n");
            break;
        case EMLINK:
            printf("Too many symlinks to parent directory\n");
            break;
        case ENAMETOOLONG:
            printf("Path name is too long\n");
            break;
        case ENOMEM:
            printf("Insufficient Kernel memory\n");
            break;
        case ENOSPC:
            printf("Unable to create directory: Insufficient space");
            break;
        case ENONET:
        case ENOTDIR:
            printf("Unable to create directory: Path is invalid\n");
            break;
        case EPERM:
            printf("Unable to create directory: El sistema no soporta la creación de directorios\n");
            break;
        case EROFS:
            printf("Unable to create directory: Path points to a read-only file\n");
            break;
        default:
            printf("Unable to create directory: Unknown error\n");
            break;
    }
}

///
/// "$ ls"
///

void listdir() {
    struct dirent *entry;
    struct stat fileStat;

    DIR *dir = opendir(".");

    if (dir == NULL) {
        perror("opendir() error");
        return;
    }

    if ((entry = readdir(dir)) == NULL) {
        printf("Error listing directory (it may no longer exist)\n");
        return;
    }

    do {
        stat(entry->d_name, &fileStat);
        if (S_ISDIR(fileStat.st_mode)) {
            printf("%5s %s\n", "DIR", entry->d_name);
        } else {
            printf("%5s %s\n", "FILE", entry->d_name);
        }
    } while ((entry = readdir(dir)) != NULL);

    closedir(dir);
}

///
/// resets the Shell colours to their defaults
///

void resetColor() {
  printf("\033[0m");
}

///
/// makes the Shell's text colour change randomly
///

void randomColor() {
    int random = randomInRange(30, 37);
    printf("\033[1;%im", random);
}

///
/// makes the Shell's background colour change randomly
///

void randomBackgroundColor() {
    int random = randomInRange(40, 47);
    printf("\033[%im", random);
}

///
/// returns a random number between min and max
///

int randomInRange(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
