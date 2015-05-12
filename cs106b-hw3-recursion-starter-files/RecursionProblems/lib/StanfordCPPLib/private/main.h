/*
 * File: main.h
 * ------------
 * This file renames the <code>main</code> method in the client's
 * program to <code>Main</code>, thereby allowing a custom
 * <code>main</code> function in the libraries to take control
 * before passing control back to the client program.  The main macro
 * also defines a function getMainFlags that returns an int whose bits
 * indicate which of the various interfaces have been loaded by this
 * definition of main.
 *
 * Note: This file can be loaded more than once and must therefore
 * check to see what has already been defined.
 *
 * If the AUTOGRADER_FLAG is defined, main is instead renamed to studentMain,
 * allowing an autograder program to become the real main that performs
 * its own initialization code and then calls the student's main function.
 */

// uncomment these lines to enter autograder mode
//#ifndef AUTOGRADER_FLAG
//#  define AUTOGRADER_FLAG 1
//#endif

#ifdef main
#  undef main
#  undef CONSOLE_FLAG
#  undef GRAPHICS_FLAG
#else
#  define MAIN_USES_CONSOLE (1<<0)
#  define MAIN_USES_GRAPHICS (1<<1)
#endif

#ifdef _console_h
#  define CONSOLE_FLAG MAIN_USES_CONSOLE
#else
#  define CONSOLE_FLAG 0
#endif

#ifdef _gwindow_h
#  define GRAPHICS_FLAG MAIN_USES_GRAPHICS
#else
#  define GRAPHICS_FLAG 0
#endif

#if CONSOLE_FLAG | GRAPHICS_FLAG

#  ifdef AUTOGRADER_FLAG
#define main studentMain(int argc, char **argv) { \
                extern int _mainFlags; \
                _mainFlags = GRAPHICS_FLAG + CONSOLE_FLAG; \
                return startupMain(argc, argv); \
             } \
             int Main
#  else  // not AUTOGRADER_FLAG
#define main main(int argc, char **argv) { \
                extern int _mainFlags; \
                _mainFlags = GRAPHICS_FLAG + CONSOLE_FLAG; \
                return startupMain(argc, argv); \
             } \
             int Main
#  endif  // AUTOGRADER_FLAG

extern int startupMain(int argc, char **argv);

#else  // not CONSOLE_FLAG | GRAPHICS_FLAG

#  ifdef AUTOGRADER_FLAG
#define main studentMain(int argc, char **argv) { \
                extern int _mainFlags; \
                _mainFlags = GRAPHICS_FLAG + CONSOLE_FLAG; \
                return mainWrapper(argc, argv); } \
             int Main
#  else  // not AUTOGRADER_FLAG
#define main main(int argc, char **argv) { \
                extern int _mainFlags; \
                _mainFlags = GRAPHICS_FLAG + CONSOLE_FLAG; \
                return mainWrapper(argc, argv); } \
             int Main
#  endif  // AUTOGRADER_FLAG
extern int mainWrapper(int argc, char **argv);

#endif  // CONSOLE_FLAG | GRAPHICS_FLAG
