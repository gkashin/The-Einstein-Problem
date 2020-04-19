# The Einstein Problem
Solving the Einstein Problem using the *[BuDDy](https://sourceforge.net/projects/buddy/)* library

## Formulation of the Problem
Let there be N = 9 objects (in our case these are financial reports of the companies). The objects are located as follows:

|  |   |  |
:-:|:-:|:-:
0 | 1 | 2
3 | 4 | 5
6 | 7 | 8

Set the neighbor relations like this (marked with an asterisk):
|  |   |  |
:-:|:-:|:-:
\- | \- | \-
\- | 0 | \-
\* | \- | *

|  |   |  |
:-:|:-:|:-:
\- | \- | 0
\- | \* | \-
\- | \- | \-

Each financial report has 4 properties:
* Name of the company
* Its profit
* Annual turnover
* Event happened with the company

It is needed to do the following:
- [X] Select M = 4 properties that take N = 9 different values 
- [ ] Set constraints
- [ ] Find one right solution (out of all the 2^144 possible ones)

## Constraints
We have 7 constraints that limit the scope of all possible answers:
1. Direct property assignment to an object
2. The relationship of the properties of two objects
3. Location constraint (all directions)
4. Location constraint (nearby)
5. For two different objects, the values of any property do not match
6. The values of the properties must be less than N
7. The sum of properties of neighboring objects should not be more than some value (say 15)

## Interpretation
*"At the end of 2019, 9 different companies published their financial statements. These reports were printed in Forbes in a specific order. Each report contains the following information - the name of the company, its net profit and turnover, as well as the main event that happened to it during this year. Unfortunately, the dog chewed on the magazine, so some lines were spoiled."*

But the following is known:
* First type of constraints:
  * The first financial report says: Apple has replaced the CEO and has a turnover of $90 billion;
  * The second financial report says: Netflix entered the international market and has a net profit of $18 billion;
  * In the third financial report: Spotify, with a profit of $70 billion, has a turnover of $300 billion;
  * In the fourth financial report: Amazon, with a profit of $104 billion, entered an IPO;
  * In the seventh financial report: Uber with a profit of $10 billion, has a turnover of $92 billion;
* Second type of constraints:
  * A company with a profit of $12 billion has a turnover of $180 billion;
  * The company that fired all employees has a profit of $60 billion;
  * A company with a turnover of $59 billion entered the international market;
  * A company with a profit of $18 billion bought a startup;
  * A company with a turnover of $360 billion has a net profit of $104 billion;
  * A company with a profit of $1 billion was sold to another company;
* Third type:
  * The financial report containing information about the company Facebook is located to the left-bottom of the report with the company with a profit of $60 billion;
  * The financial report, which describes the company with a profit of $60 billion, is located at the bottom left of the report with a company with a turnover of $360 billion;
  * The financial report with the company with a profit of $360 billion lies to the bottom right of the report with the company that received the investment;
  * The financial report of the company that received the investment is located to the right-bottom of the report of Uber;
* Fourth:
  * The financial report of Google is next to the report of Facebook;
  * The report of the company with a profit of $18 billion next to the report of the company with a profit of $60 billion;
  * A company report with a turnover of $241 billion next to a company report with a turnover of $360 billion;
  * The report of the company that fired all employees lies next to the report of the company that received the investment;
  * The Twitter report is next to the Uber report.
  
Based on known information, it is required to restore all the data of financial reports

## Output
```
1 solution:

0: 0 8 5 3 
1: 4 5 6 7 
2: 8 0 1 8 
3: 3 4 4 0 
4: 7 1 2 2 
5: 5 3 3 4 
6: 6 7 7 5 
7: 2 2 8 1 
8: 1 6 0 6 
```

It can be interpreted as follows:
Report number|Company name|Profit, $B|Turnover, $B|Event
:-----------:|:----------:|:--------:|:----------:|:---:
0 | Apple | 0 | 90 | Replaced CEO
1 | Netflix | 20 | 59 | Entered the international market
2 | Spotify | 70 | 300 | Launched its own production
3 | Amazon | 104 | 360 | IPO
4 | Airbnb | 12 | 180 | Went bankrupt
5 | Twitter | 60 | 241 | Fired all employees
6 | Uber | 10 | 84 | Got an investment
7 | Facebook | 18 | 92 | Bought a startup
8 | Google | 1 | 265 | Was sold to another company
