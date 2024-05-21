## shunting
This program is a comprehensive mathematical tool that can perform various numerical methods and calculus operations. The core functionality of the calculator is based on the Shunting Yard algorithm, which is used to parse mathematical expressions and convert them into Reverse Polish Notation (RPN). This README file will explain the code, its functions, and the concept behind shunting.

The program is designed to provide various numerical methods and calculus operations, including:
-Bisection Method
-Regula-Falsi Method
-Newton-Raphson Method
-Matrix Inversion
-Gaussian Elimination
-Gauss-Seidel Method
-Numerical Derivatives
-Simpson's Rule
-Trapezoidal Rule
-Gregory-Newton Interpolation
## Shunting Yard Algorithm
The Shunting Yard algorithm, developed by Edsger Dijkstra, is used to parse mathematical expressions specified in infix notation and convert them into postfix notation (RPN). This conversion simplifies the evaluation of the expression. The algorithm uses two primary data structures: a stack for operators and a queue for the output.

## Usage
To use the program, compile the code and run the executable. You will be greeted with a menu to choose from various numerical methods and operations. Follow the prompts to input your expressions and parameters.


gcc -o math_calculator main.c
./math_calculator
