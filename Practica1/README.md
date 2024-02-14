# Manual técnico
---
### Diego Andres Huite Alvarez - 202003585
---

## Descripción del programa
El ejecutable llamado 'exec' que consiste en un programa con tres procesos: un proceso principal con dos procesos hijos, los cuales son la ejecución del ejecutable
'childprocess.bin'. Cada proceso hijo tiene como tarea hacer tres operaciones de manera aleatoria sobre el archivo 'practica1.txt', estas operaciones son:
- Escribir 8 bytes aleatorios en el archivo
- Leer 8 bytes en el archivo
- Mover el puntero del archivo al inicio
Todo esto con el fin de que el proceso padre monitorée a los dos procesos hijos y escriba en el archivo syscalls.log cada una de las llamadas al sistema que
realicen los dos procesos hijos, todo esto a través de un script de systemtap.

## Proceso principal (main.c & fileHandler.c)
El método main de este proceso primeramente establece una función a ejecutarse cuando sea recibida la señal de SIGINT, para posteriormente 
llama a la función 'createInitialFiles' del archivo fileHandler.c para crear los dos archivos donde los procesos estarán
efectuando sus operaciones. Para luego crear los dos procesos hijos con una llamada a fork() y ejecutar el programa 'childprocess.bin' por medio de la función
execv().
```C
int main(){
    // Set up the custom signal handler
    signal(SIGINT, handleCtrlC);
    createInitialFiles(); // creates syscall log file and practica1 file
    char *argPtr[2] = {"./practica1.txt", NULL};
    // Command
    char command[100];

    // pointer to syscall log file
    FILE *sysCallLogFile = fopen("./syscalls.log", "w+");

    int childProcess1 = fork();

    if (childProcess1 == 0) {
        // Child process 1 logic goes here  
        execv("./childProcess.bin", argPtr);
        
    } else {

        int childProcess2 = fork();

        if (childProcess2 == 0) {
            // Child process 2 logic goes here
            execv("./childProcess.bin", argPtr);
        }
        pidChildProcess1 = childProcess1;
        pidChildProcess2 = childProcess2;

        printf("Parent process running with %d child processes.\n", NUMOFCHILDRENPROCESSES);
        printf("Child process 1 PID: %d\n", pidChildProcess1);
        printf("Child process 2 PID: %d\n", pidChildProcess2);

        sprintf(command, "%s %d %d %s", "sudo stap ./src/trace.stp", pidChildProcess1, pidChildProcess2, " > syscalls.log");
        system(command);

        for (int i = 0; i < NUMOFCHILDRENPROCESSES; ++i) {
            wait(NULL);
        }
    }

    fclose(sysCallLogFile);

    return 0;
}
```
![image](https://github.com/Dgo45921/SO2_202003585/assets/80992782/40abde5f-adaf-4749-b453-84767a9dcf2d)
## Procesos hijo (childProcess.c)
Este proceso tiene un método main que consiste en primeramente abrir el archivo 'practica1.txt' para empezar a realizar las operaciones sobre este, para ello 
genera dos números aleatorios para realizar operaciones, el primer número aleatorio define que operación va a
realizar, las posibles operaciones son:
- 0 write:
  Escribe ocho caracteres aleatorios en el archivo practica1.txt
- 1 read
  Lee ocho caracteres en el archivo practica1.txt
- 2 seek
  Mueve el puntero del archivo al inicio de este.
Luego, el otro número aleatorio se encarga de darnos un timeout de 1 a 3 segundos para realizar una una de las operaciones anteriores en un lapso aleatorio. Para
finalmente cerrar el archivo y que los cambios hechos sean aplicados.
```C
int main(int argc, char *argv[])
{
    // Seed for random generation based on the current time
    srand((unsigned int)time(NULL));
    const char *filename = argv[0];
    
    if(access(filename, F_OK) != -1){

        while (1){
            int randomSleepSeconds = rand() % 3 + 1;
            sleep(randomSleepSeconds);
            int randomOperation = getRandomOperationIndex();
            FILE *practicaFile = fopen(filename,"r+");
            
            if (randomOperation == 0){
                writeOperation(practicaFile);
            }
            else if (randomOperation == 1){
                // read 8 chars
                readEightChars(practicaFile);
            } else{
                // lseek 
                resetFilePointer(practicaFile);
            }
            fclose(practicaFile);
        }
    } 
    else return -1;
    

    return 0;
}



```


