/** @defgroup   line    Line
 *
 *  @brief      Line termination processing.
 *
 *  This module contains functions to translate between different line
 *  termination characters for serial data.
 */

#ifndef __LINE_H__
#define __LINE_H__

/** @ingroup    line
 *
 *  @brief      Configure line termination characters.
 *
 *  Configures the expected line termination characters for serial I/O. Line
 *  terminations may be one of CR, LF, and CR+LF, and may be different for
 *  serial input and serial output.
 *
 *  @param      iterm   Line termination for serial input. Should be equal to
 *                      `"cr"`, `"lf"`, or `"crlf"`.
 *  @param      oterm   Line termination for serial output. Should be equal to
 *                      `"cr"`, `"lf"`, or `"crlf"`.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int line_set_term (const char * iterm, const char * oterm);

/** @ingroup    line
 *
 *  @brief      Translate serial input data.
 *
 *  Processes the specified serial input buffer and replaces it in-place with a
 *  translated one, reallocating the buffer if necessary. All line terminations
 *  are replaced by line feed characters.
 *
 *  @note       This function must not be called before the line terminations
 *              are configured with line_set_term().
 *
 *  @param      data    Pointer to serial input buffer to be translated.
 */

void line_process_input_data (char ** data);

/** @ingroup    line
 *
 *  @brief      Translate serial output data.
 *
 *  Processes the specified serial output buffer and replaces it in-place with a
 *  translated one, reallocating the buffer if necessary. All line feed
 *  characters are replaced with line terminations.
 *
 *  @note       This function must not be called before the line terminations
 *              are configured with line_set_term().
 *
 *  @param      data    Pointer to serial output buffer to be translated.
 */

void line_process_output_data (char ** data);

#endif
