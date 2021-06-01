!alert construction title=[Initialize Documentation #43 [!icon!link]](https://github.com/idaholab/falcon/issues/43) prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

# Example 1: The first FALCON example

lorem ipsum...

## Problem Statement

lorem ipsum...

## Input File

lorem ipsum...

### Results

We can use next/previous buttons to make it easy to navigate through example pages:

!content pagination previous=examples/index.md
                    next=examples/example02.md

We can also render the buttons with page headers:

!content pagination use_title=True
                    previous=examples/index.md
                    next=examples/example02.md
