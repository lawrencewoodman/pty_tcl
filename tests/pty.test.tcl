package require tcltest
namespace import tcltest::*

# Add module dir to tm paths
set ThisScriptDir [file dirname [info script]]
load [file join $ThisScriptDir .. libpty-tcl[info sharedlibextension]]


test open-1 {Check can read/w through pseudo tty} -setup {
  lassign [pty::open] masterChan slaveName
  set slaveChan [open $slaveName r+]
  set chatLog [list]
} -body {
  puts $masterChan "message from master"
  flush $masterChan
  lappend chatLog [list m2s [gets $slaveChan]]
  puts $slaveChan "message from slave"
  flush $slaveChan
  lappend chatLog [list s2m [gets $masterChan]]
} -cleanup {
  close $slaveChan
  close $masterChan
} -result {{m2s {message from master}} {s2m {message from slave}}}


test open-2 {Check will accept -nogrant option} -setup {
  lassign [pty::open -nogrant] masterChan slaveName
  set slaveChan [open $slaveName r+]
  set chatLog [list]
} -body {
  puts $masterChan "message from master"
  flush $masterChan
  lappend chatLog [list m2s [gets $slaveChan]]
  puts $slaveChan "message from slave"
  flush $slaveChan
  lappend chatLog [list s2m [gets $masterChan]]
} -cleanup {
  close $slaveChan
  close $masterChan
} -result {{m2s {message from master}} {s2m {message from slave}}}


test open-3 {Check will return an error if invalid option passed} {
  catch {lassign [pty::open -fred]} result
  set result
} {bad option "-fred": must be -nogrant}


test open-4 {Check will return an error too many arguments passed} {
  catch {lassign [pty::open fred]} result
  set result
} {wrong # args: should be "pty::open ?options?"}


cleanupTests
