# `mtfind(1)`
![version: 0.1.0](https://img.shields.io/badge/version-1.0-blue.svg)
![license: GNU GPL v2](https://img.shields.io/badge/license-GNU_GPL_v2-brightgreen.svg)
![works: in my machine](https://img.shields.io/badge/works-in_my_machine-brightgreen.svg)
#### Half-assed multithread grep clone
> 🔎 Find a sequence of characters within a file using multiple threads

## What's this?
`mtfind(1)` is a cross-platform utility that searches for a given sequence of ASCII characters in a file spawning 
the supplied number of threads.

It is also my own approach for the second assignment for my Operating Systems II course.

## Quick start
```sh
# Clone this repository
git clone https://github.com/anpep/mtfind
# Build rzctl
cd mtfind && make
# Try it out!
./mtfind
```

## Open-source code
`mtfind(1)` is licensed under the GNU General Public License v2.

```
mtfind(1) -- Half-assed multithread grep clone
Copyright (c) 2021 Ángel Pérez <angel@ttm.sh>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
```