/*********************************************************
 Authors: Adrian Ward-Manthey, Colin Parente
 Purpose: Use Python library with C to control Grove Pi
**********************************************************/

#include <python2.7/Python.h>
#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>

/******************
  Signal Handling 
*******************/
void sigHandler();
void set_crmode();
int tty_mode(int how);

void processPython(int, char**, char*, char*);
/***************************************
 Constants to files and corresponding 
 functions
*****************************************/
char *automatedFile = "grove_light_sensor";
char *fAuto         = "LightSensor";

char *brightness    = "grove_bright";
char *fBright       = "bright";

char *blinkF        = "grove_led_blink";
char *fBlink        = "blink";

char *fadeSample = "led_fade";
char *fFade = "Fade";

int main(int argc, char *argv[])
{
    /**************************************
     Signal handling for Crtl + C
    **************************************/
      tty_mode(0);       //save tty mode
      set_crmode();      //set chr-by-chr mode
      signal(SIGINT, sigHandler);

    printf("\nWelcome to the Light Control System Prototype!!\n");
    printf("This simple demo illustrates a basic UI for our system\n");
    printf("To interrupt a process please hit Ctrl+C\n\n");
    printf("");

    /***********************************************
      Basic User Input Loop: 
      Needs to hand interruptions by sending user 
      back to menu.
    ************************************************/
    int input;
    printf("Please Select from the following options (type in number of desired option):\n\n");
    printf("1) AutoMode\n");
    printf("2) BlinkMode\n");
    printf("3) Adjust Brightness (Not yet available)\n");
    printf("4) Quit\n");
    while(1){
        switch(input = getchar() ){
            case '1':
               processPython(argc, argv, automatedFile, fAuto);
            case '2':
               processPython(argc, argv, blinkF, fBlink);
               exit(1);
            case '3':
               processPython(argc, argv, brightness, fBright);
               //printf("\nOption is unavailable at this time. Please choose another option:");
               //continue;
            case '4':
               printf("\nThanks for using our system:) Goodbye!!!\n");
               tty_mode(1);
               exit(0);
            default:
              printf("\nInput not recognized. Please pick from available options:\n");
              continue;
   }
 }

   // processPython(argc, argv, blinkF, fBlink);
    return 0;
}

/*****************************************************************
 This function processes and runs python scripts and functions that
 are used to interact with grove pi. Will need to modify to send
 data from Python to C.
******************************************************************/
void processPython(int argc, char *argv[], char *pfile, char *pfunction){
PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;
    /*
    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        exit(1);
        //return 1;
    }
    */
    Py_Initialize();

    PyRun_SimpleString("import sys"); 
    PyRun_SimpleString("sys.path.append(\"/home/pi/Desktop/LightDemo\")");



    pName = PyString_FromString(pfile);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, pfunction);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    exit(1);
                    //return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                tty_mode(1);
                exit(1);
                //return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", pfunction);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", pfile);
        tty_mode(1);
         exit(1);
        //return 1;
    }
    Py_Finalize();

}

/***************************************************************
 purpose: put file descriptor 0 (i.e stdin) into chr-by-chr mode
 method: use bits in termios 
*****************************************************************/
void set_crmode(){
  struct termios ttystate;
  
  tcgetattr(0, &ttystate);           //read curr setting
  ttystate.c_lflag      &=~ICANON;   //no buffering
  ttystate.c_lflag     &=~ECHO;     //no echo either
  ttystate.c_cc[VMIN]   = 1;         //get 1 char at a time
  tcsetattr(0, TCSANOW, &ttystate);  //install settings
}

//how==0 => save current mode, how==1 => restore mode
int tty_mode(int how){

  static struct termios original_mode;
  if(how==0)
    tcgetattr(0, &original_mode);
  else
    return tcsetattr(0, TCSANOW, &original_mode);
}

/*******************************************************
 Handle User Input when signal interrupt is triggered
*******************************************************/
void sigHandler(){
  printf("\nExit Program? [y/n]");
  int input;
  //fix so that user does NOT have to hit enter
  while(1){
   switch(input = getchar() ){
         case 'y':
         case 'Y':
            tty_mode(1);
            putchar(input);
            printf("\n");
            exit(1);
         case 'n':
         case 'N':
            putchar(input);
            return;
         default:
            putchar(input);
            return;
   }
 }
}