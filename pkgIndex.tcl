set ThisScriptDir [file dirname [info script]]
package ifneeded "pty" \
                 0.1 \
                 [list load [file join $ThisScriptDir \
                                       libpty-tcl[info sharedlibextension]]]
