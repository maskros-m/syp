Good lord, I've finally made sense of this mess of a braintwister T.T

The goal is to create a simple calculator that can perform basic arithmetic operations between only two operands.
The algorithm for the programme is as follows:

:: State 0: OP1_START
This is the starting point of the programme. State (or $stateInx in the code) is set to 0.
At this stage, if the user enters a digit --> programme will continue to run, and state is set to 1. 
If the user messes up (whether intentionally or otherwise) and hit any other button that is not a digit, the programme enters straight into State 5 (ERROR).

:: State 1: OP1
In this stage, the programme continues to take in digit input.
Because the calculator uses decimal number system, so with every new digit added to the input string, the previous "number" must be multiplied by 10 first, before the new input number is added to it.
E.g. the user first enters '1', then '2' (which makes '12'), so the result must be 
    1 * 10 + 2 = 12
to reflect the value intended by the user.
and if the user continues to input '2' (which makes '123'), so 12 must be multiplied by 10 first before 3 is added to it:
    12 * 10 + 3 = 123
and so on.

This whole process would immediately stop as soon as the user inputs an operator (+, -, * or /), as this signifies they're happy and done with the first operand (let's call it 'a').
The value of 'a' is then saved to $op1 for use later.
When an operator is input, the programme enters the next state, which is set to 2.

:: State 2: OP2_START
Basically a repetition of State 0 under a different name. 
As soon as a digit is entered --> programme will continue to run, and state is set to 3.
Any error would prompt the state to jump to 5 (ERROR).

:: State 3: OP2
Just as in State 1, the programme continues to take digit input.
With any new digit entered, the previous 'number' will need to be multiplied by 10 and the value of the new digit entered will be added to that 'number'.

This process will stop when the user hits '=', signifying that no more digits will be entered and the other operand (let's call it 'b') is all set and done.
When '=' is hit --> the programme takes the left operand 'a', the right operand 'b' and the operator previously entered and does appropriate calculation before returning the result to the screen.

If all is successful, the programme enters state 4.

:: State 4: FINISHED
Basically this state is just a 'standby' phase before the programme enters a new cycle.
If the user is ready for a new arithmetic operation, he/she can hit 'C' and the programme cycles the state back to 0.
Now the input is checked again and the cycle begins anew.

:: State 5: ERROR
If anytime during the programme invalid input occurs, the programme enters this state where 'Error' is displayed on the screen.
The user gets out of this mode by pressing 'C' to reset the result to 0 and the state back to 0 (which means 'ready', sort of).

-----------------------------
END OF NOTE
