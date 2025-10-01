#!/bin/bash
git fetch 
git checkout log
git checkout dev -- /datafiles
git commit -m "automatic log update"
git push 
git checkout dev