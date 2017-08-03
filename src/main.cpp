#include <python2.7/Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

//the name of this program
const char* program_name;

//call python function
void process_expression(const char* filename,const char *func_name, const char* arg)
{
    FILE*        exp_file;
    PyObject*    main_module, * global_dict, * expression;

    // Initialize a global variable for
    // display of expression results
    //PyRun_SimpleString("x = 0");

    // Open and execute the Python file
    exp_file = fopen(filename, "r");
    PyRun_SimpleFile(exp_file, filename);

    // Get a reference to the main module
    // and global dictionary
    main_module = PyImport_AddModule("__main__");
    global_dict = PyModule_GetDict(main_module);

    // Extract a reference to the function "func_name"
    // from the global dictionary
    expression =
        PyDict_GetItemString(global_dict, func_name);
    
    PyObject_CallFunction(expression,"s",arg);

    //PyRun_SimpleString("print x");
}

//
/*int call_func(char* filename, char *func_name, char* arg)
{
    PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    Py_Initialize();
    pName = PyString_FromString(filename);
    // Error checking of pName left out

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func_name);
	// pFunc is a new reference

        if (pFunc && PyCallable_Check(pFunc)) {
                                                   
            pValue = PyObject_CallObject(pFunc, arg);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func_name);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", arg);
        return 1;
    }
    Py_Finalize();
    return 0;
}
*/
//print usage information
void print_usage(FILE* stream,int exit_code)
{
  fprintf(stream,"Usage: %s options [inputfile ...]\n",program_name);
  fprintf(stream,
	  " -h --help        Display this usage information.\n"
	  " -o --output --filename Wite output to file.\n"
	  " -v --verbose Print verbose messages.\n");
  exit (exit_code);
}

/*Main program entry point. Argc contains number pf argument list element,
  argv is an array of ponter to them*/

int main(int argc, char *argv[])
{
  int next_option;

  //a string list short option letter
  const char* const short_options="ho:v";
  //an array describing valid long option
  const struct option long_options[]={
    {"help", 0, NULL, 'h'},
    {"output", 1, NULL, 'o'},
    {"verbose", 0, NULL, 'v'},
    {NULL, 0, NULL, 0} //require at end of array
  };

  //name of the file to receive program output, or NULL for standard output
  const char* output_filename=NULL;
  //whether to display verbose message
  int verbose=0;

  //remember the name of the program to incoporate in message. The name is stored in argv[0]
  program_name=argv[0];

  do{
    next_option=getopt_long(argc,argv,short_options,long_options,NULL);
    switch(next_option)
      {
      case 'h': //--help
	print_usage(stdout,0);
      case 'o': //--output
	output_filename=optarg;
	break;
      case 'v': //--verbose
	verbose=1;
	break;
      case '?': //user specified an invalid option
	print_usage(stderr,1);
      case -1: //done with option
	break;
      default:
	abort();
      }
  }while(next_option !=-1);

  if(verbose)
    {
      for (int i = optind; i < argc; ++i)
	{
	  printf("Argument: %s\n",argv[i]);
	}
    }

  //main program goes there
  
  /* printf("Name of program: %s",argv[0]);
  printf("Nmber of argument of program: %d",argc-1);
  if(argc>1)
    {
      int i;
      printf("the argument are: \n");
      for(i=1;i<argc;i++)
	{

	  printf(" %s\n",argv[i]);
	}
    }
    return 0;*/
  /* Py_SetProgramName(argv[0]);
  Py_Initialize();
  PyRun_SimpleString(
		     "#from time import time,ctime\n"
		     "#print 'Today is', ctime(time())\n"
		     "exec(open(\"/home/phong/parse_log.py\").read(), globals())\n"
		     );
  Py_Finalize();

  */
  Py_Initialize();
  
  const char* filename = "/home/phong/jpter_notebook/engine.py";
  const char* func_name = "main";
  const char* arg = "/home/phong/jpter_notebook/dataset/conn_all.csv";
  process_expression(filename,func_name,arg);
  
  Py_Finalize();
  //call_func("/home/phong/jpter_notebook/engine.py","main","dataset/conn_all.log");
}
