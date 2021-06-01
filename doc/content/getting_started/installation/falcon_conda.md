!!!
NOTE: Here, we're simply copying and pasting the content from
      `moose/modules/doc/content/getting_started/installation/conda.md` so that we can set the
      appropriate links to FALCON documentation content. It would be nice if we could simply use the
      `!include` command with the `replace` argument, but the autolinks would still issue an error
      upon initial tokenization of the page (unless the links were made optional: `optional=True`).
      Thus, this page ought to be kept up-to-date as its framework counterpart evolves.
!!!

!alert construction title=[Initialize Documentation #43 [!icon!link]](https://github.com/idaholab/falcon/issues/43) prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

# Conda MOOSE Environment

Our preferred method for obtaining dependencies necessary for MOOSE-based
application development is via Conda's myriad array of libraries. Follow these
instructions to create an environment on your machine using Conda. At this time,
an option to install MOOSE directly on a Windows system is not yet supported.
On-going efforts are being made to add a conda installation option for Windows,
and an experimental [WSL](installation/falcon_windows10.md) option is available.

!include getting_started/installation/install_miniconda.md

!include getting_started/installation/install_conda_moose.md
         replace=['(troubleshooting.md#condaissues optional\=True)', '(https://mooseframework.inl.gov/moose/help/troubleshooting.html#condaissues) on the main MOOSE website']

Once you have successfully completed the MOOSE installation, head back over to the [Installation](getting_started/installation/index.md) page to proceed with cloning and testing the FALCON repository.

!include getting_started/installation/uninstall_conda.md
