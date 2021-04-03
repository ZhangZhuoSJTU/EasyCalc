# EasyCalc

EasyCalc is a simple calcuator which supports 4 register variables (_a_, _b_, _c_, and _d_), and a stack variable (_s_) with 5 elements. It supports addition and subtraction. All the constants should be in the range [0, 9].

Following is the grammar of input files:

```c
Program    :: Statements
Statements :: Expr | Statements Expr
Expr       :: LHS '=' RHS ';'
LHS        :: Variable
RHS        :: Variable | Operator Variable | RHS Operator Variable
Variable   :: Register | Stack | Constant
Register   :: 'a' | 'b' | 'c' | 'd'
Stack      :: 's' '[' Constant ']'
Constant   :: '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
Operator   :: '+' | '-'
```

Additionally, EasyCalc has three semantic constraints:

+ Out-of-bounds indexing of stack is not allowed (0 <= idx < 5) 
+ LHS cannot be a constant
+ Every variable should be defined before using


## Build

```bash
$ cd parser
make all
```

## Usage

```bash
$ cd parser
./easy-calc <filename>
```

## Example

```bash
$ cat test.ec
s[1] =   9;
a    = + 1 + 2 + s[1];
s[2] =   5 + a;
b    = - 1 + 2 + s[2];
s[3] = - 1 - 1 - 2 - 3;

$ ./easy-calc test.ec
  Register A: 12
  Register B: 18
  Register C: undef
  Register D: undef
    Stack[0]: undef
    Stack[1]: 9
    Stack[2]: 17
    Stack[3]: -7
    Stack[4]: undef
```
