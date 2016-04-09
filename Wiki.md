[TOC]

## 1. Summary

Engineering Calculator is a lightweight, yet handy math expression evaluation and unit conversion utility that is always at your fingertips.

[100% FREE award granted by Softpedia](http://www.softpedia.com/progClean/Engineering-Calculator-Clean-235312.html)

## 2. Installation

Unzip [the archive](https://sourceforge.net/projects/alwaysontopcalc/files/latest/download) to you favourite folder and run "bin\\EngCalc.exe".

### 2.1 Command-line Options
Some of the default options can be changed by creating a short-cut and adding command-line options. Some of these options toggle, as opposed to set, so use them only once.

Option       | Action
-------------|-------
Calculator   | Start in Calculator mode, as opposed to converter mode
Normal       | Start with Normal number format, instead of Engineering
Not-on-top   | Start with "Always on top" disabled
Degrees      | Start with Degrees instead of Radians
RightSpace=X | Change the start-up position by specifying the distance from the right of the screen (X is some integer value, in pixels)

As an example, you could create a short-cut that runs "bin\\EngCalc.exe calculator normal not-on-top rightspace=200", which will start Engineering Calculator in calculator mode, with normal form answers, not always on top and 200 pixels from the right of the screen.

## 3. Converter.xml File

Converter.xml stores all the conversion formulae.  Each conversion is an entity "Conversion" with attributes "From" (the "from" unit), "To" (the "to" unit) and "Formula" (the conversion formula in terms of 'x').  All three attributes must be present for the conversion to be valid.

The idea is that the user can use the given conversion file as a starting point to build his / her own conversion library.  Please feel free to [post](https://sourceforge.net/p/alwaysontopcalc/discussion/create_topic) your conversions on the discussions page (in the form of an XML file), so that other users can also benefit.

Please note that Converter.xml supports only the UTF-8 encoding.

## 4. Manual

Right-click near the '=' sign, or on the tray icon, for a pop-up menu.  This menu can change various modes and provides the key-board shortcuts.

The window can be dragged to where you want it by dragging close to the '=' sign.

Evaluation is performed in the normal mathematical order (brackets -> functions -> multiply and divide -> add and subtract -> logical operators).

All space and tab characters are ignored, even those within numbers.

### 4.1 Numbers

- Decimal numbers have any number of decimal digits and an optional decimal point (or comma).
- Hexadecimal numbers start with '0x' and is followed by any number of hexadecimal digits.
- Binary numbers start with '0b' and is followed by any number of 0's and 1's.
- Decimal, hexadecimal and binary numbers can have an optional decimal exponent, which starts with 'e' or 'E', followed by a decimal integer.
- Decimal, hexadecimal and binary numbers can have an optional binary exponent, which starts with 'p' or 'P', followed by a decimal integer.
- Numbers are represented internally in 80-bit extended precision floating-point format.  All the limits of this format therefore applies.

### 4.2 Operators

Operators are used in the form 'A op B', where 'A' and 'B' are numbers, or formulae themselves.

Operator|Meaning
--------|-------
+       | Add
-       | Subtract
\*      | Multiply
/       | Divide
%       | Remainder (modulus) after division
rem     | Same as '%'
^       | Raise to the power of
>       | Greater than
>=      | Greater than or equal to
<       | Less than
<=      | Less than or equal to
=       | Equal
~=      | Not equal
&       | Logical and
/       | Logical or (vertical bar)
:       | Logical exclusive or

For interest sake, the formula evaluation library is also used for drawing graphs in another application, where the logical operators are used to define piece-wise functions.

### 4.3 Functions

Functions are used either in the form 'funcA', where A is a number, or 'func(A)' where 'A' is a number or formula.

Function| Meaning
--------|--------
-       | Negate
~       | Logical Not
log     | Logarithm to base 10
lb      | Logarithm to base 2
ln      | Logarithm to base e
abs     | Absolute value
round   | Round
fix     | Drops the part after the decimal point
floor   | Floor
ceil    | Ceiling
rand    | Uniform random number between 0 and the number specified
sin     | Sine
asin    | Arcsine
cos     | Cosine
acos    | Arccosine
tan     | Tangent
atan    | Arctangent
sec     | Sec
asec    | Arcsec
cosec   | Cosec
acosec  | Arccosec
cot     | Cotangent
acot    | Arccotangent
sinh    | Hyperbolic sine
asin    | Hyperbolic arcsine
cosh    | Hyperbolic cosine
acosh   | Hyperbolic arccosine
tanh    | Hyperbolic tangent
atanh   | Hyperbolic arctangent
sech    | Hyperbolic sec
asech   | Hyperbolic arcsec
cosech  | Hyperbolic cosec
acosech | Hyperbolic arccosec
coth    | Hyperbolic cotangent
acoth   | Hyperbolic arccotangent

### 4.4 Other Functionality

Function| Meaning
--------|--------
(A)     | Evaluate 'A' first, then use the answer in subsequent evaluation steps.
A!      | 'A' faculty
\[A\]B  | if 'A' is true, evaluate B, otherwise search for the next '\[' and repeat.
d(A)dx  | Perform symbolic differentiation of 'A' with respect to 'x'.  In this application it would only make sense as part of a conversion formula, as 'x' will always take the value of 0 when used in the calculator.

### 4.5 Constants

All constants are in SI units.

Constant  | Meaning
----------|--------
e         |  Natural base
pi        |  &pi;
\\c       |  Speed of light
\\G       |  Gravitational constant
\\g       |  Gravitational acceleration of the earth
\\me      |  Electron mass
\\mp      |  Proton mass
\\mn      |  Neutron mass
\\u       |  Atomic mass constant
\\e       |  Electron charge (positive)
\\h       |  Planck constant
\\k       |  Boltzmann constant
\\mu      |  Permeability of free space
\\epsilon |  Permittivity of free space
\\Phi     |  Fluxon
\\NA      |  Avogadro's number
\\R       |  Gas constant
\\rhoAl   |  Resistivity of aluminium
\\rhoCu   |  Resistivity of copper
\\F       |  Faraday's constant
\\AU      |  Astronomical unit
\\pc      |  Parsec
\\mil     |  1 mil (25.4 micron)

If you'd like to have more constants, [post a suggestion](https://sourceforge.net/p/alwaysontopcalc/discussion/create_topic) in the "General Discussion" forum.
 
## 5. Future Version Ideas

If you have any ideas, please do not hesitate to [post them](https://sourceforge.net/p/alwaysontopcalc/discussion/create_topic) on the discussions page.

## 6. Updates

I often upload the same version more than once, with subsequent uploads incorporating some minor bug-fixes.  The file time-stamp can be used as an indication of version, but the build time-stamp, available in the "About" box, is more accurate.

## 7. Source

The source is available from the [git repository](https://sourceforge.net/p/alwaysontopcalc/code/).

Please [contact me](https://sourceforge.net/p/alwaysontopcalc/discussion/create_topic) if you are enthusiastic enough to have ported the project to Mac OS X, Linux or your favourite operating system.  I'd like to post the ported versions.

[[members limit=20]]
[[download_button]]

