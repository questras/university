#!/bin/bash

echo "Initializing permissions..."

# Directories permissions.
setfacl -m g:clients:r-x deposits
setfacl -m g:clients:r-x credits

setfacl -m g:officers:rwx deposits
setfacl -m g:officers:rwx credits

# New files in directories permissions.
setfacl -d -m g::--- deposits
setfacl -d -m g::--- credits

setfacl -d -m g:clients:--- deposits
setfacl -d -m g:clients:--- credits

setfacl -d -m g:officers:rw- deposits
setfacl -d -m g:officers:rw- credits
