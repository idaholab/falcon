!alert construction title=[Initialize Documentation #43 [!icon!link]](https://github.com/idaholab/falcon/issues/43) prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

# Theory Manual

Lorem ipsum...

## Example Section

We can automatically denote an acronym key listed in `falcon/doc/acronyms.yml` like [!ac](THMC),
or use one listed in `moose/framework/doc/acronyms.yml` like [!ac](JFNK).
Then when we mention these acronyms again, only their keys will be rendered, e.g., [!ac](THMC) or [!ac](JFNK).

We can automatically generate in-line citations and an alphabetized reference list at the end of this page based on citations listed in `falcon/doc/content/bib/falcon.bib` or those in `moose/framework/doc/content/bib/*`. For example:

- The theory supporting the C++ classes available in the FALCON repository have been described in [!citet](xia2017).
- FALCON is based on the MOOSE Framework (see [!citet](gaston2014continuous) and [!citet](gaston2015physics)).
