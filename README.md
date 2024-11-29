# search

Tiny command line utility to recursively find text in the current directory.

## Usage
```
# search [keyword1] [keyword2] ...
search cout
```

## Compilation

```
make
```

## Installation

Adds the repo directory to $PATH in your ~/.bashrc
```
echo "PATH='\$PATH:$(pwd)'" >> ~/.bashrc
```

## TODO
- quotation mark support for finding specific word groups
- -d argument to select directory to search 
- colored console output
