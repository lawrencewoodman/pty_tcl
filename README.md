pty
===
A Tcl package to handle pseudo TTYs.


Package Usage
-------------
To open a pseudo TTY use the `pty::open` command which will return the master channel and the name of the slave device file such as `/dev/pts/3`:

    package require pty

    lassign [pty::open] masterChan slaveName
    puts "Slave device file name: $slaveName"
    set slaveChan [open $slaveName r+]

    puts $masterChan "message from master"
    flush $masterChan
    puts "Received at slave: \"[gets $slaveChan]\""

    puts $slaveChan "message from slave"
    flush $slaveChan
    puts "Received at master: \"[gets $masterChan]\""

    close $slaveChan
    close $masterChan


Exported Commands
-----------------
**pty::open** _?-option?_ <br />
Opens a pseudo TTY master slave pair and returns the pair as a list with the first element being the master channel and the second being the name of the slave device file.  There is a single _option_:
<dl>
  <dt>-nogrant</dt>
    <dd>Prevents <a href="http://pubs.opengroup.org/onlinepubs/9699919799//functions/grantpt.html">grantpt()</a> from being run to change the mode and ownership of the slave pseudo-terminal device.</dd>
</dl>


Requirements
------------
*  A C/C++ compiler (Currently tested with gcc and g++)
*  Tcl 8.5+ and development files
*  CMake 2.8+ (To compile)


Compiling and Installing
------------------------
`pty` is designed to be run under Unix and has been most thoroughly tested on Linux.

From the root directory of the repository, the directory this file is in, create the Makefiles:

    $ cmake .

Then compile the project:

    $ make

To install the package to a sensible location run the following as root.  Make a note of the directories installed to in case of problems; these locations locations are also recorded in `install_manifest.txt`:

    $ make install


Testing
-------
There is a test suite in `tests/`.  To run it:

    $ tclsh tests/pty.test.tcl


Troubleshooting
---------------

If you find after installing that the package can't be found when `package require` is called, then add part of the installation path to the environmental variable `TCLLIBPATH`.  For bash you could put the following in your `.bashrc`:

    export TCLLIBPATH="$TCLLIBPATH /usr/local/lib"

Alternatively you could copy the files for the package (`pkgIndex.tcl` and `libpty-tcl.so`) to a directory such as `pty-tcl0.1/` off of one of the directories in Tcl's `auto_path` variable.  To find the directories in the `auto_path` from the command line:

    $ echo "puts \$auto_path" | tclsh


Contributions
-------------
If you want to improve this package make a pull request to the [repo](https://github.com/LawrenceWoodman/pty_tcl) on github.  Please put any pull requests in a separate branch to ease integration and add a test to prove that it works.


Licence
-------
Copyright (C) 2015, Lawrence Woodman <lwoodman@vlifesystems.com>

This software is licensed under an MIT Licence.  Please see the file, LICENCE.md, for details.
