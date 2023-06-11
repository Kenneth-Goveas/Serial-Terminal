/** @defgroup   option  Option
 *
 *  @brief      Command line option parsing.
 *
 *  This module contains functions for parsing command line options.
 */

#ifndef __OPTION_H__
#define __OPTION_H__

#include <stdbool.h>

/** @ingroup    option
 *
 *  @brief      Register boolean flag option.
 *
 *  Registers a boolean flag option with the specified name. The value of this
 *  option is considered `true` if it is passed on the command line, and `false`
 *  otherwise.
 *
 *  @param      name    Option name as expected in short form on the command
 *                      line.
 *  @param      value   Pointer to variable into which the option value will be
 *                      written.
 */

void option_register_flag (char name, bool * value);

/** @ingroup    option
 *
 *  @brief      Register string-valued parameter option.
 *
 *  Registers a string-valued parameter option with the specified name. The
 *  value of this option is the corresponding argument passed on the command
 *  line.
 *
 *  @param      name    Option name as expected in short form on the command
 *                      line.
 *  @param      value   Pointer to buffer which will be reallocated and filled
 *                      in with the option value.
 */

void option_register_param (char name, char ** value);

/** @ingroup    option
 *
 *  @brief      Parse command line arguments for registered options.
 *
 *  Parses the command line arguments for options registered with
 *  option_register_flag() and option_register_param() and fills their values
 *  into the corresponding variables.
 *
 *  @param      argc    Command line argument count.
 *  @param      argv    Command line argument vector.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int option_parse_args (int argc, char ** argv);

/** @ingroup    option
 *
 *  @brief      Assert boolean flag option.
 *
 *  Asserts that a given boolean flag option was passed on the command line.
 *  This function call results in a failure if the desired option wasn't passed.
 *
 *  @param      name    Boolean flag option name.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int option_assert_flag (char name);

/** @ingroup    option
 *
 *  @brief      Assert string-valued parameter option.
 *
 *  Asserts that a given string-valued parameter option was passed on the
 *  command line. This function call results in a failure if the desired option
 *  wasn't passed.
 *
 *  @param      name    String-valued parameter option name.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int option_assert_param (char name);

#endif
