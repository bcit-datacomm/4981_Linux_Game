# Code Style

## File Extensions
---
- for c++ source files use the extension .cpp
- for c source files use the extension .c
- for c++ header files as well as c header files use .h
    note: this is to maintain backwards consistancy with c as some c++ headers may be used in c

## Objects and Structs
---
- start with a capital letter
- decending visibility ie public then protected then private
- visibility same level as the struct or class keyword
- set defaults in the headers in the ctors specifically, dont do it in the member list
- use member initializers as much as possible, its a waste of memory and time to not
- abstract parent classes should still provid the ctors to initialize the variables that the child is inheriting at a minimum
- use smartpointers where you can and references even more so but if you _need_ to make a new object manually, make sure it is being deleted later and verify that it is actually needed.
- ctor and dtor should be the first two functions in the class or struct when they exist

```
struct Bla {
public:
    Bla(int xVal = 5);
    
    ~Bla();
    
    void thing();
    //protected: would go here if needed
private:
    int x;  //dont do 
            //int x = 5;
            //here it goes in the ctor
}
```
implementation file
```
Bla::Bla(int xVal):x(xVal){
    printf("some thing was made");
}
```


## Braces and Indenting
---
- space between if and first parentheses
- space after last parentheses and opening brace
- brace on the same line as the if
- same goes for all other flow control
- else encapsulated by a brace on either side on the same line with spaces either side

### Dont use tabs ever period. Use 4 spaces!
all major IDEs support pressing tab but writing 4 spaces
    
```
if (stuff == otherStuff) {
    doStuff();
} else {
    dont();
}
```

- the elements inside of for loops must be spaced as follows

```
for (int i = 0; i < 10; ++i) {
    doStuff();
}
```

- the cases in switch statements follow the same increase in indentation levels as other braces
- breaks go on the same level as the code inside the case
- switches must have it stated that a fall through is intentional

```
switch (i) {
    case 5:
        stuff();
        break;
    case 6:
        otherStuff();
        break;
    case 7:
        moreStuff();
        //intentional fall through
    default:
        thing();
        break;
}
```

## Line Limit
---
- the soft max line limit is 100 characters while the hard max is at 115 characters
    -if your line goes to 105 and is annoying to break into multiple lines then thats ok
    -if its easy to make multi lines break it up
- for very large or unclear parameter list breakups you can have the closing parentheses on a new line
- where it is clear where the parameter list ends the closing parentheses can be on the final line
- for function parameter lists you do NOT need to line up space for space with the first argument
- the indent level must at least be two indentation levels(2 x 4spaces)
    but additional levels can be added to make code more readable

```
//very simple function with lots of args
void foobar(int someIntForSomeReason,
            struct serveraddr_in *listenSocketTCP, 
            Style whoKnowsWhatAStyleIsDoingHere, 
            int abc, 
            int def, 
            int hig) {

//a large function param list with multiple teirs(unlikely to happen)
std::swap(
    vectorTable[
    (startCoords.first * OUTER_BOARD_SIZE) 
        + startCoords.second + (col * OUTER_BOARD_SIZE) + row]
    ,
    vectorTable[
    count + (startCoords.first * OUTER_BOARD_SIZE) 
        + startCoords.second + (col * OUTER_BOARD_SIZE) + row]
);

//a smaller function param list
//split into logical groupings
plugBoard = Arrays.stream(keyValue[1].split("#"))
    .map(pair -> pair.chars()
        .mapToObj(c -> (char) c)
        .collect(Collectors.toList()))
    .collect(Collectors.toList());
```

## Comments
---
- quick notes saying what you did can be single line comments

```
    //my quick note
```

- for documentation use double * multi line comments

``` 
    /**
     *Some Documentation
     */
```

- single line comments go before the statement they are about

```
    //this checks if thing is true
    if (thing) {
        ...
```

- the exception to the single line comments is for switches. switches should have clear enough cases that this 
    is not needed but in the event that a case does need commenting comment on the case line
- switches must have it stated that a fall through is intentional

````
    switch(getchar()){
        case 'x'://for some reason i needed to state here
            break;
        case 7:
            moreStuff();
            //intentional fall through
        default:
            thing();
            break;
    }
```
