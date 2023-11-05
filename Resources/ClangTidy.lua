-- Manually validated which bugprone- checks apply to Unreal Engine code.
enable_checks("bugprone-bool-pointer-implicit-conversion")
enable_checks("bugprone-branch-clone")
enable_checks("bugprone-copy-constructor-init")
enable_checks("bugprone-forwarding-reference-overload")
enable_checks("bugprone-implicit-widening-of-multiplication-result")
enable_checks("bugprone-incorrect-roundings")
enable_checks("bugprone-infinite-loop")
enable_checks("bugprone-integer-division")
enable_checks("bugprone-macro-repeated-side-effects")
enable_checks("bugprone-misplaced-widening-cast")
enable_checks("bugprone-parent-virtual-call")
enable_checks("bugprone-redundant-branch-condition")
enable_checks("bugprone-signed-char-misuse")
enable_checks("bugprone-sizeof-expression")
enable_checks("bugprone-suspicious-enum-usage")
enable_checks("bugprone-suspicious-semicolon")
enable_checks("bugprone-swapped-arguments")
enable_checks("bugprone-terminating-continue")
enable_checks("bugprone-too-small-loop-variable")
enable_checks("bugprone-undelegated-constructor")
enable_checks("bugprone-unhandled-self-assignment")
enable_checks("bugprone-virtual-near-miss") 

-- This will generate warnings in automation code, so you can only really enable it
-- for modules that are not using the automation macros.
-- enable_checks("bugprone-macro-parentheses")

-- Manually validated which performance- checks apply to Unreal Engine code.
enable_checks("performance-for-range-copy")
enable_checks("performance-implicit-conversion-in-loop")
enable_checks("performance-move-constructor-init")
enable_checks("performance-no-automatic-move")
enable_checks("performance-trivially-destructible")
enable_checks("performance-unnecessary-copy-initialization")

-- This check can be particular verbose if you've got lots of value parameters
-- (usually through functions that act as delegate callbacks). Still, it's usually
-- a good idea to enable this check and then go through and manually mark delegate
-- capture parameters with // NOLINTNEXTLINE(performance-unnecessary-value-param)
-- to catch all the remaining value parameters that should be by const-reference.
enable_checks("performance-unnecessary-value-param")

-- Unreal Engine checks.
enable_checks("unreal-*")

-- Treat certain bugs as errors.
treat_as_errors("bugprone-*")
treat_as_errors("performance-*") 
treat_as_errors("unreal-*")