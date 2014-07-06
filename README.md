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

File Format
===

The BigFix archive format first creates a tape archive of all of the contents
of the directory, then compresses it. To extract an archive, you have to first
decompress the file, then parse the contents of the tape archive.

Compression Format
---

First there is a `8 byte` header that is always `##SC001` (including the null
terminator). In hex this is:

    0x23, 0x23, 0x53, 0x43, 0x30, 0x30, 0x31, 0x00
    
Next there is a `4 byte` file checksum. This can sometimes be all zero.

Finally, the remainder of the file is compressed using the zlib `deflate`
function. It can be decompressed using the zlib `inflate` function.

Archive Format
---

The archive is a list of file and directory entries.

* `1 byte` or `2 bytes`: Entry header. The possible values are:
  * `"21"` the path is UTF-8 and the file length is 8 bytes long
  * `"2_"` the path is UTF-8 and the file length is 4 bytes long
  * `"1"` the path is in the local encoding and the file length is 8 bytes long
  * `"_"` the path is in the local encoding and the file length is 4 bytes long
* `1 byte`: Path length. A length of `0` terminates the archive. This length
  includes the null terminator.
* `<path length> bytes`: File or directory path. If this ends with a `'/'`, then
  it's a directory.
* `1 byte`: Date length.
* `<date length> bytes`: The modification date.
* `4 bytes` or `8 bytes`: The file length. The length of this field depends on
  the entry header.
* `<file length> bytes`: The file contents.

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

You can then open the generated `BFArchive.sln` solution file in Visual Studio
to build.

Support
===
Any issues or questions regarding this software should be filed via
[GitHub issues](https://github.com/bigfix/bfarchive/issues).
