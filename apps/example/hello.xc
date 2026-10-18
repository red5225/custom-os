# A first XC program
program "Hello CustomOS"
say "Welcome to CustomOS!"
say "Let's do a little math."

let score = 10
add score 5
say "Your score is now:"
show score

ask name
if name == "Nolan"
  say "Welcome back!"
else
  say "Hello, new user!"
end

finish
