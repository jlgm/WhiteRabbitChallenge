# White Rabbit Challenge

Solution for the "Follow the White Rabbit" Challenge promoved by Trust Pilot.

## Requirements
* gcc version 5.4.0
* openssl
* libssl-dev

Check this link on how to install openssl: http://www.askyb.com/cpp/openssl-md5-hashing-example-in-cpp

## Running

Use `make filter` to filter the words from dictionary.

Then, run with `make run` - the program will start decoding.

### Using Docker

It is possible to run the program with docker. Use `make docker-setup` to build the image and then `make docker-run` to run.

## Algorithm

### Observations

There are a few key observations that we can make in order to make the computation for this problem easier. It follows:

i) A word is an anagram of another if and only if it they have the same characters.
ii) There are only 18 characters in `"poultry outwits ants"` and only 12 **different** characters.
iii) Character `t` repeats 4 times. 
iv) Characters `o, s and u` repeat 2 times.
v) All other characters appear only 1 time.

### 1) Filtering

From the wordlist that is provided, we can filter out words that can't be an anagram for the text `"poultry outwits ants"` (let's call it T). These are:

* Words that have characters not in T
* Words that repeat a character more times than it appears in T

### 2) Encoding words

As discussed in the *Observations* part, there are only 12 different characters in `"poultry outwits ants"`, and a few of them repeats. Let's associate each different letter to a different prime number, but smaller primes will be associated with letters that repeat the most.

In this case, we'll have:

|        Character       | Repeat |              Prime             |
|:----------------------:|:------:|:------------------------------:|
|            t           |    4   |                2               |
|         o, s, u        |    2   |             3, 5, 7            |
| a, i, l, n, p, r, w, y |    1   | 11, 13, 17, 19, 23, 29, 31, 37 |

Now, we can encode a word as the multiplication of it's characters values. It's easy to notice that anagrams will have the same code. Also, because we're associating smaller numbers to characters that repeat more, any word that was not removed will have its code small enough to fit a **long long** type. In fact, the sentence `"poultry outwits ants"` (and any of it's anagrams) encodes to 6233420033240400.

*A set of words will be anagram of T if the multiplication of their codes results in the code of T.*

For example, if we define our encode function as hash(), then `hash("poultry")\*hash("outwits")\*hash("ants") = hash("poultryoutwitsants")`

### 3) Grouping anagrams

Now we have enough to describe our algorithm.

We firstly encode all words and create a dictionary that associates these codes (keys) to vectors of strings. Words that share the same code will then be on the same vector. 

E.g.:

```
24909:[sits]
25194:[pairs paris parsi]
25195:[artist strait traits]
```

### 4) Complement of word

We also define the complement of a word W in T as all the characters that are in T but are not in W. For example, for the word `"tryoutwits"` in the word `"poultryoutwitsants"`, the complement would be `"poulants"`. For every word that we calculate the code, we also calculate the code for its complement. We use this code as key in a dictionary that points to the code of the original word.
E.g.: when calculating code for `"tryoutwits"` and finding value X, we say `comp[hash("poulants")] = X`.

Now, to calculate the code for the complement, we just need to divide the hash value of the whole word (in this case, `"poultryoutwitsants"`) by the hash of the current word (in this case, `"tryoutwits"`). On the example above, `hash("poulants") = hash("poultryoutwitsants") / hash("tryoutwits"`)

By doing this, we reduce the search space considerably. If we want to look for groups of 3 words that can be solution, we only need to generate combinations of two words (and in each group, we look for its complement, to find the code for the missing word).

To keep using the same example, if the group we're trying is `"poultry", "outwits"`, we know only an anagram of `"ants"` can be considered, because it's the complement of `"poultryoutwits"` in relation to `"poultryoutwitsants"`. So when we find for `comp[hash("poultryoutwits")]` we'll find the code for `"ants"`.

### 5) Finding the solution

The program tries groups of two words, then groups of three, then groups of four, then groups of five. It'll stop only when it finds a solution or if the answer has a group of more than five words.

It'll try every possible combination for the keys and, whenever a potential candidate appears (when the code for the group is the same as the group of the original word that we're looking for the anagram), it will try every permutation of words this current set has. 

## Results

Tested on a 2013 Laptop with the following specs:

    * 16.04.1-Ubuntu SMP 
    * Processor: Intel(R) Core(TM) i7-3537U CPU @ 2.00GHz
    * RAM: 8GB
    * SSD 256GB

The program was able to crack the three MD5 Hashes:

|               Hash               	|        Anagram        	|    Time   	|
|:--------------------------------:	|:---------------------:	|:---------:	|
| e4820b45d2277f3844eac66c903e84be 	| printout stout yawls  	| 0.033719s 	|
| 23170acc097c24edb98fc5488ab033fe 	| ty outlaws printouts  	| 0.014192s 	|
| 665e5bcb0c20062fe8abaaf4628bb154 	| wu lisp not statutory 	| 64.582s   	|

## Potential pitfalls

* This solution is tightly coupled with the inputs of this problem. If another set of words is given to find anagrams for, it might not work since multiplying the primes might not fit a long long. (although, modules could be used here for approximate solutions)
* It stores all the words in memory. Perhaps using a trie would be better
* Too many shared resources between functions, which, although more efficient, is hard to debug
* It might be a bit heavy on mathematics and code is not too easy to follow

## Potential improvements

* When a potential solution is found, we could check asynchronously (using parallelization) if it's a solution. Perhaps using some kind of job queue and workers
* Using other data structures (such as char arrays instead of string class, arrays instead of vectors etc.) could improve performance
* We can generate the combinations iteratively (which might improve performance)
* User Interaction could be better
* Unit tests