# Serial Terminal

*Serial Terminal* is a simple command line tool for Linux, for communicating
with devices connected over a UART. Received data is printed on screen and user
input is transmitted to the connected device.

# Installation

- Install the following dependencies:
  ```
  gcc make git
  ```
  for example, by running the following command if you use a Debian-based
  system:
  ```
  sudo apt install gcc make git
  ```
- Clone this repository into the directory of your choice and enter it:
  ```
  git clone https://github.com/Kenneth-Goveas/Serial-Terminal.git <path>
  cd <path>
  ```
- Compile the program and install it:
  ```
  make
  sudo make install
  ```
- Once installed, you may remove the compiled binaries:
  ```
  make clean
  ```
- If you wish, you can easily uninstall the program:
  ```
  sudo make uninstall
  ```

# Usage

The following command opens a serial connection with a connected device:
```
serial-terminal <device> <baud>
```
where `<device>` is the path to the serial port, and `<baud>` is the serial bit
rate. For example, the following command:
```
serial-terminal /dev/ttyUSB0 115200
```
opens a serial connection with the device `/dev/ttyUSB0` at `115200` baud. The
program can be stopped at any point of time by pressing `Ctrl+C`.

The bit rates supported by this program are: `50` `75` `110` `134` `150` `200`
`300` `600` `1200` `1800` `2400` `4800` `9600` `19200` `38400` `57600` `115200`
`230400` `460800` `500000` `576000` `921600` `1000000` `1152000` `1500000`
`2000000` `2500000` `3000000` `3500000` `4000000`.

# Documentation

The documentation for the source code can be generated using
[Doxygen](https://www.doxygen.nl) which can be installed by following the
instructions on its [website](https://www.doxygen.nl/download.html).

From the directory where you have cloned this repository, simply run the
following command:
```
doxygen
```
which will generate the `doc` directory containing the documentation in HTML
format.
