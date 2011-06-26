#!/bin/sh
./vidanueva --docroot="static-content;/resources" --deploy-path="/" --no-compression --http-address=0.0.0.0 --http-port=8000 -c wt-config.xml
