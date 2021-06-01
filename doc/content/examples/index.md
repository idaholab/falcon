!alert construction title=[Initialize Documentation #43 [!icon!link]](https://github.com/idaholab/falcon/issues/43) prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

# Falcon Examples id=examples

A list of simulations using FALCON is provided below.

## Table of Contents id=contents

We could manually list hyperlinks to example pages, like how MASTODON does it:

- [Example 1](examples/example01.md): The first FALCON example
- [Example 2](examples/example02.md): The second FALCON example

Or, we could invoke `!content outline` in a variety of ways. For example, with only first-level headers in alphabetical order (by default):

!content outline location=examples
                 no_prefix=1

With numbered first- and second-level headers in a specific order, but skip counting on certain ones:

!content outline max_level=2
                 no_count=examples contents
                 pages=examples/index.md
                       examples/example01.md
                       examples/example02.md

All headers in the directory with page headers listed in alphabetical order, with certain ones hidden, and with only subheaders numbered:

!content outline location=examples
                 max_level=6
                 hide=examples contents
                 no_prefix=1

!content pagination next=examples/example01.md
