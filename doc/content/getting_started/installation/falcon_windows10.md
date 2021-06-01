!!!
NOTE: Here, we're simply copying and pasting the content from
      `moose/modules/doc/content/getting_started/installation/windows10.md` so that we can set the
      appropriate links to FALCON documentation content. It would be nice if we could simply use the
      `!include` command with the `replace` argument, but the autolinks would still issue an error
      upon initial tokenization of the page (unless the links were made optional: `optional=True`).
      Thus, this page ought to be kept up-to-date as its framework counterpart evolves.
!!!

!alert construction title=[Initialize Documentation #43 [!icon!link]](https://github.com/idaholab/falcon/issues/43) prefix=False
All pages on this website are currently under development and do not yet serve as general documentation of the [FALCON repository](https://github.com/idaholab/falcon).

# Windows 10

!alert! warning
Using MOOSE on Windows 10 is experimental and not fully supported.

Caveats:

- Peacock does not work correctly (artifacts during rendering: surface normals are flipped).
- Different flavors of Linux are available.

    - Be sure to choose an OS in which we support. While MOOSE will ultimately work on just about every flavor of Linux, this document assumes you have chosen Ubuntu 20.04.
!alert-end!

!include installation/wsl.md

## Close the WSL terminal

With the above complete, close the WSL terminal, and re-open it. Then proceed to our [Conda Install Instructions](getting_started/installation/falcon_conda.md).

!alert! note
Your Download's folder, while using WSL, is located at: `/mnt/c/Users/<Your User Name>/Downloads`
!alert-end!
