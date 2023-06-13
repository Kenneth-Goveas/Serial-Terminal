/** @defgroup   sleep   Sleep
 *
 *  @brief      Sleep mode.
 *
 *  This module contains functions to put the program to sleep until one or more
 *  wakeup events occur.
 */

#ifndef __SLEEP_H__
#define __SLEEP_H__

#include <poll.h>

/** @ingroup    sleep
 *
 *  @brief      Register wakeup event.
 *
 *  Registers the specified wakeup event. Subsequently, every occurrance of this
 *  event will wake the program up from any sleep initiated by calling
 *  sleep_wait_for_wakeup_evt(). The relevant wakeup event structures can be
 *  obtained by calling console_get_wakeup_evt() and serial_get_wakeup_evt().
 *
 *  @param      evt     Wakeup event to be registered.
 */

void sleep_register_wakeup_evt (struct pollfd evt);

/** @ingroup    sleep
 *
 *  @brief      Sleep until wakeup event occurs.
 *
 *  Puts the program to sleep until one or more wakeup events registered with
 *  sleep_register_wakeup_evt() occur or if the program is interrupted by a
 *  `SIGINT` signal.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int sleep_wait_for_wakeup_evt (void);

#endif
