Usage
===

BFArchive is a tool that can list, create, and extract the contents of BigFix
archives. It is simliar in function to the unix `tar` utility.

### Listing the contents of an archive

To list the contents of an archive, use the `-l` or `--list` argument:

    $ bfarchive --list archive
    
### Creating a new archive

To create a new archive, use the `-a` or `--create` argument:

    $ bfarchive --create /some/folder some_folder_archive
    
### Extracting an archive

To extract the contents of an archive, use the `-x` or `--extract` argument:

    $ bfarchive --extract some_folder_archive /some/folder
    
If no destination folder is specified, BFArchive will extract the archive into
the current directory.
    
### Reading from stdin

If the archive file argument is `-`, then BFArchive will read the archive
contents from stdin. For example, you can list the contents of the defunct games
site like this:

    $ curl --silent http://sync.bigfix.com/bfsites/games_29/__fullsite | BFArchive --list -
    
### Verbose output

If you add `v` or `--verbose` when creating or extracting an archive,
BFArchive will output the name of every file and directory as it is processing
it.

    $ bfarchive -xv some_archive /some/folder

Building
===

The build requires that you have [CMake](http://cmake.org/) installed. Although
you can build within the source directory, your life will be easier if you build
in a separate directory to avoid polluting the source directory.

### Linux & Mac

    $ mkdir build
    $ cd build
    $ cmake /path/to/bfarchive
    $ make
    $ make test

### Windows

    $ mkdir build
    $ cd build
    $ cmake /path/to/bfarchive
    
You can then open the generated `BFArchive.sln` in Visual Studio to build.

Support
===
Any issues or questions regarding this software should be filed via
[GitHub issues](https://github.com/bigfix/bfarchive/issues).
