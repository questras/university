#!/bin/bash

/etc/init.d/ssh restart
./scripts/init_permissions.sh
gunicorn --bind 0.0.0.0:8080 --certfile=bsk1.crt --keyfile=bsk1.key webapp.run:app
