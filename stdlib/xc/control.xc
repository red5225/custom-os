# Control-flow example for the currently supported if/else/end syntax.
program "XC Control Example"
let mode = "safe"
if mode == "safe"
  say "safe mode"
else
  say "other mode"
end
finish
