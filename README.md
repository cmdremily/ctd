# Cmdr sTanDard library (CTD)
CTD provides two things, functionality often used in my personal projects and a partial, but compliant implementation of the `std` library for platforms where none is provided. Like for example `avr-libc`.

The functionality that is provided in addition to the `std` implementation is designed to work on microcontrollers with small program memory and tight RAM. This means heavy use of compile time constants and computations to allow the compiler as much potential as possible to produce small code. As well as the ability to use small types for 8-bit micros and careful handling of overflow conditions.
